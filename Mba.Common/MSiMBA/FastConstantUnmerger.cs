using Mba.Ast;
using Mba.Common.Minimization;
using Mba.Parsing;
using Mba.Testing;
using Mba.Utility;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.MSiMBA
{
    // Represents a performed rewrite operation.
    public struct Rewrite
    {
        public readonly byte varCount;
        public readonly ushort ttIndex;
        public readonly long c1;
        public readonly long c2;
        public readonly long c3;

        public Rewrite(byte varCount, ushort ttIndex, long c1, long c2, long c3)
        {
            this.varCount = varCount;
            this.ttIndex = ttIndex;
            this.c1 = c1;
            this.c2 = c2;
            this.c3 = c3;
        }

        public IReadOnlyList<long> GetConstants()
        {
            if (varCount == 1)
                return new List<long>() { c1 };
            if (varCount == 2)
                return new List<long>() { c1, c2 };
            if (varCount == 3)
                return new List<long>() { c1, c2, c3 };
            throw new InvalidOperationException();
        }
    }

    public class FastConstantUnmerger
    {
        // Optional debug logging.
        const bool dbg = false;

        private readonly uint bitWidth;

        private readonly long moduloMask;

        // List of constants that (a) have not been rewritten as a linear combination of other constants,
        // and (b) have not been used inside of a different linear combination.
        public OrderedSet<long> notYetRewrittenPool;

        // Set that keeps track of the order the rewrites were applied in.
        public OrderedSet<long> orderedRewrites = new();

        // List of constants that have been expressed as a linear combination of other constants
        public Dictionary<long, Rewrite> rewrittenPool = new();

        // Set of constants that can no longer be used during matching, because they are being used to
        // express other constants.
        public OrderedSet<long> conflictPool = new();

        // Identifies whether trivial negations should be matched.
        private readonly bool again;

        public FastConstantUnmerger(uint bitWidth, List<long> constants, bool again = true)
        {
            this.bitWidth = bitWidth;
            moduloMask = (long)(ulong)ModuloReducer.GetMask(bitWidth);

            // Order the constants by their signed value. We prefer positive numbers over negative numbers,
            // so applying this order allows us to first rewrite negative constants as negations of positive constants.
            notYetRewrittenPool = new(constants.OrderByDescending(x => x));
            this.again = again;
        }

        private void ApplyRewrite(long elem, Rewrite rewrite)
        {
            notYetRewrittenPool.Remove(elem);
            orderedRewrites.Add(elem);
            rewrittenPool.Add(elem, rewrite);

            var varCount = rewrite.varCount;
            if (varCount >= 1)
            {
                conflictPool.Add(rewrite.c1);
                notYetRewrittenPool.Remove(rewrite.c1);
            }

            if (varCount >= 2)
            {
                conflictPool.Add(rewrite.c2);
                notYetRewrittenPool.Remove(rewrite.c2);
            }
        }

        public AstNode BuildAstForConstant(long constant)
        {
            // If we haven't rewritten this constant, keep it.
            if (notYetRewrittenPool.Contains(constant))
            {
                return EnforceConstantRelationships(constant);
            }

            // If this is a negation, just emit it.
            if (!rewrittenPool.TryGetValue(constant, out var rewrite))
                return new ConstNode(constant, bitWidth);

            if (rewrite.varCount == 1)
            {
                return new NegNode(BuildAstForConstant(rewrite.c1));
            }

            // If this is a two var rewrite, fetch it.
            else if (rewrite.varCount == 2)
            {
                // Fetch ASTs for both constants.
                var op0 = BuildAstForConstant(rewrite.c1);
                var op1 = BuildAstForConstant(rewrite.c2);

                // Fetch the table entry for this function.
                var tableEntry = TwoVarTruthTable.TwoVarTruthTableEntries[rewrite.ttIndex];
                var variables = InputVariableUtility.GetVariables(tableEntry);

                var dict = new Dictionary<VarNode, AstNode>();
                dict.Add(variables[0], op0);
                dict.Add(variables[1], op1);

                var clone = AstCloner.CloneAst(tableEntry, dict, bitWidth);
                return clone;
            }

            else
            {
                throw new InvalidOperationException($"Unrecognized variable count: {rewrite.varCount}");
            }
        }

        // Identify relationships between the constants and enforce them.
        // This allows simba to take advantage of linear relationships between the constants.
        private AstNode EnforceConstantRelationships(long constant)
        {
            AstNode ast = new ConstNode(constant, bitWidth);
            foreach (var other in notYetRewrittenPool)
            {
                if (other == constant)
                    continue;

                // If c1&c2 == 0, rewrite the constant as c1&(~c2).
                var otherConst = new ConstNode(other, bitWidth);
                var anded = constant & other;
                if (anded == 0)
                {
                    ast = new AndNode(ast, new NegNode(otherConst));
                    continue;
                }

                // If c1&c2 == c1, rewrite c1 as c1&c2
                if (anded == constant)
                {
                    ast = new AndNode(ast, otherConst);
                    continue;
                }

                // If c1|c2 == c1, rewrite c1 as c1|c2
                var ored = constant | other;
                if (ored == constant)
                {
                    ast = new OrNode(ast, otherConst);
                    continue;
                }
            }

            return ast;
        }

        public void Unmerge()
        {
            // Remove trivial negations.
            // Note that we only bother with this once,
            // because all trivial negations will be identified immediately.
            if (again)
                RemoveNegations();

            // Try to rewrite constants as two variable linear combinations of other constants.
            RemoveTwoVarPermutations();

            // Do fixed point iteration.
            if (again)
            {
                // TODO: Fixed point iterate instead of iterating to N.
                for (int i = 0; i < 3; i++)
                {
                    // Apply constant unmerging 
                    var workingSet = notYetRewrittenPool.Concat(conflictPool).ToList();
                    var unmerger = new FastConstantUnmerger(bitWidth, workingSet, false);
                    unmerger.Unmerge();

                    // Union the information
                    UnionWith(unmerger);
                }

                Log("Failed to rewrite: ");
                foreach (var c in notYetRewrittenPool)
                    Log($"    {c}");
            }
        }

        // Identify constants that are negations of other constants.
        // This is the most profitable case, and it's the most easy to handle
        // because of a lack of potential circular dependencies.
        private void RemoveNegations()
        {
            foreach (var elem in notYetRewrittenPool.ToList())
            {
                // Skip if a negation does not exist.
                var negation = ~elem;
                if (!notYetRewrittenPool.Contains(negation))
                    continue;

                Log($"{negation} => ~{elem}");
                var rewrite = new Rewrite(1, 0, elem, 0, 0);
                ApplyRewrite(negation, rewrite);
            }
        }

        private void RemoveTwoVarPermutations()
        {
            bool changed = true;

            start:
            while (changed)
            {
                changed = false;
                var clone = notYetRewrittenPool.ToList();
                var len = clone.Count;
                for (int a = 0; a < len; a++)
                {
                    var v1 = clone[a];
                    for (int b = a + 1; b < len; b++)
                    {
                        var v2 = clone[b];
                        if (TryMatchTwoVarPermutation(v1, v2))
                        {
                            changed = true;
                            goto start;
                        }
                    }
                }
            }
        }

        // Unions the information in this instance with the information inside of the other instance.
        // Note that we take special care to avoid circular dependencies.
        private void UnionWith(FastConstantUnmerger other)
        {
            // Clear the conflict and not yet rewritten pool.
            conflictPool.Clear();
            notYetRewrittenPool.Clear();

            // Walk through the rewrites in the order they were applied.
            foreach (var currConstant in other.orderedRewrites)
            {
                // Fetch the rewrite.
                var rewrite = other.rewrittenPool[currConstant];

                // Add this constant to the rewrite pool.
                ApplyRewrite(currConstant, rewrite);
            }

            // Add back the remaining conflicts and not yet written cases.
            foreach (var c in other.conflictPool)
                conflictPool.Add(c);
            foreach (var c in other.notYetRewrittenPool)
                notYetRewrittenPool.Add(c);
        }

        public bool TryMatchTwoVarTt0(long x, long y)
        {
            var v0 = ReduceModulo(~(x | y));
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"~({x}|{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 0, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt1(long x, long y)
        {
            var v0 = ReduceModulo(~(x | ~y));
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"~({x}|~{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 1, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt2(long x, long y)
        {
            var v0 = ReduceModulo(x & ~y);
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"({x}&~{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 2, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt3(long x, long y)
        {
            var v0 = ReduceModulo(x ^ y);
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"({x}^{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 3, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt4(long x, long y)
        {
            var v0 = ReduceModulo(~(x & y));
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"~({x}&{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 4, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt5(long x, long y)
        {
            var v0 = ReduceModulo(x & y);
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"({x}&{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 5, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt6(long x, long y)
        {
            var v0 = ReduceModulo(~(x ^ y));
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"~({x}^{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 6, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt7(long x, long y)
        {
            var v0 = ReduceModulo(~(x & ~y));
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"~({x}&~{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 7, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt8(long x, long y)
        {
            var v0 = ReduceModulo(x | ~y);
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"({x}|~{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 8, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }
        public bool TryMatchTwoVarTt9(long x, long y)
        {
            var v0 = ReduceModulo(x | y);
            if (!notYetRewrittenPool.Contains(v0) || x == v0 || y == v0)
                return false;
            var str = $"({x}|{y})";
            Log($"{v0} => {str}");
            var rewrite = new Rewrite(2, 9, x, y, 0);
            ApplyRewrite(v0, rewrite);
            return true;
        }

        bool TryMatchTwoVarPermutation(long x, long y)
        {
            bool changed = TryMatchTwoVarTt0(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt1(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt2(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt3(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt4(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt5(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt6(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt7(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt8(x, y);
            if (changed)
                return true;
            changed = TryMatchTwoVarTt9(x, y);
            if (changed)
                return true;
            return false;
        }

        private long ReduceModulo(long value)
            => moduloMask & value;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void Log(string msg)
        {
            if (!dbg)
                return;
            Console.WriteLine(msg);
        }
    }

    public static class TwoVarTruthTable
    {
        private const string strTable = @"[0 0 0 0],(x&~x)
        [1 0 0 0],~(x|y)
        [0 1 0 0],~(x|~y)
        [1 1 0 0],~x
        [0 0 1 0],(x&~y)
        [1 0 1 0],~y
        [0 1 1 0],(x^y)
        [1 1 1 0],~(x&y)
        [0 0 0 1],(x&y)
        [1 0 0 1],~(x^y)
        [0 1 0 1],y
        [1 1 0 1],~(x&~y)
        [0 0 1 1],x
        [1 0 1 1],(x|~y)
        [0 1 1 1],(x|y)
        [1 1 1 1],~(x&~x)";

        public static IReadOnlyList<AstNode> TwoVarTruthTableEntries;

        static TwoVarTruthTable()
        {
            var truthTableEntries = strTable.Split(Environment.NewLine).ToList();
            // Take only the expression
            truthTableEntries = truthTableEntries.Select(x => x.Split(",", StringSplitOptions.RemoveEmptyEntries)[1]).ToList();
            truthTableEntries = truthTableEntries.Where(x => x.Contains("x") && x.Contains("y")).ToList();
            TwoVarTruthTableEntries = truthTableEntries.Select(x => AstParser.Parse(x, 64)).ToList();
        }
    }
}
