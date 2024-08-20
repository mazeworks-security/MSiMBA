using Antlr4.Runtime.Atn;
using Mba.Ast;
using Mba.Interop;
using Mba.Parsing;
using Mba.Testing;
using Mba.Utility;
using Iced.Intel;
using Mba.Common;
using Microsoft.Z3;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Reflection.Metadata;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Xml.XPath;
using static Iced.Intel.AssemblerRegisters;
using ApInt = System.UInt64;
using Mba.Common.Interop;
using Mba.Common.Minimization;

namespace Mba.Common.MSiMBA
{
    public enum Metric : int
    {
        Alteration = 0,
        Terms = 1,
        String = 2,
        BitwiseNodes = 3,
        Count = 4,
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct CoeffWithMask : IComparable<CoeffWithMask>
    {
        [FieldOffset(0)]
        public ApInt coeff;

        // Must be set to 16 if using Uint128s
        [FieldOffset(8)]
        public ApInt mask;

        public CoeffWithMask(ApInt coeff, ApInt mask)
        {
            this.coeff = coeff;
            this.mask = mask;
        }

        public unsafe int CompareTo(CoeffWithMask other)
        {
            return coeff.CompareTo(other.coeff);
        }

        public override string ToString()
        {
            return $"{coeff} * (bitop&{mask})";
        }
    }

    public class MultibitSiMBA
    {
        // Allocate a mutable read-write-execute page on a per-thread basis.
        public static readonly ThreadLocal<nint> JitPage =
            new ThreadLocal<nint>(() => JitUtils.AllocateExecutablePage(1000));

        private readonly Metric metric = Metric.String;

        private readonly AstNode ast;

        private readonly IReadOnlyList<VarNode> variables;

        private readonly uint bitSize;

        private readonly bool refine;

        // If enabled, we perform multi-bit simba.
        private bool multiBit;

        // If enabled, we try to find a simpler representation of grouping of basis expressions.
        private readonly bool tryDecomposeMultiBitBases;

        private readonly ApInt moduloMask = 0;

        // Number of combinations of input variables(2^n), for a single bit index.
        // For multi-bit this must be multiplied by the bit width.
        private ApInt numCombinations;

        private readonly List<int> groupSizes;

        private ApInt[] resultVector;

        private readonly MultibitRefiner refiner;

        private AstNode res = null;

        private List<int?> __compl = null;

        private int? lincombTerms = null;

        private readonly bool newLine = false;

        private readonly bool dbg = false;

        public static AstNode Run(AstNode ast, bool alreadySplit = false, bool multibit = false, bool tryDecomposeMultiBitBases = false)
        {
            var variables = InputVariableUtility.GetVars(ast);
            return new MultibitSiMBA(ast, variables, ast.BitSize, true, multibit, tryDecomposeMultiBitBases).Simplify(false, alreadySplit);
        }

        public MultibitSiMBA(AstNode ast, IReadOnlyList<VarNode> variables, uint bitSize, bool refine = true, bool multiBit = false, bool tryDecomposeMultiBitBases = true)
        {
            int limit = 25;
            if (variables.Count > limit)
                throw new InvalidOperationException($"Cannot simplify MBAs with more than {limit} variables.");

            this.ast = ast;
            this.variables = variables;
            this.bitSize = bitSize;
            this.refine = refine;
            this.multiBit = multiBit;
            this.tryDecomposeMultiBitBases = tryDecomposeMultiBitBases;
            moduloMask = (ApInt)ModuloReducer.GetMask(bitSize);
            groupSizes = GetGroupSizes(variables.Count);
            numCombinations = (ApInt)Math.Pow(2, variables.Count);
            resultVector = BuildResultVector(moduloMask, variables, ast, multiBit, numCombinations);

            // If multi-bit simba is requested, and the result vector is uniform across all bit indices,
            // we can disable multi-bit simplification and treat the function as a classical linear MBA.
            if (multiBit && IsLinearResultVector(moduloMask, bitSize, resultVector, variables, numCombinations))
            {
                this.multiBit = false;
                resultVector = BuildResultVector(moduloMask, variables, ast, false, numCombinations);
            }

            refiner = new MultibitRefiner(bitSize, moduloMask);
        }

        // Initialize the group sizes of the various variables, i.e., their numbers
        // of subsequent occurences in the truth table.
        public static List<int> GetGroupSizes(int varCount)
        {
            var groupSizes = new List<int>() { 1 };
            for (int i = 0; i < varCount; i++)
                groupSizes.Add(2 * groupSizes[groupSizes.Count - 1]);

            return groupSizes;
        }

        public static unsafe ApInt[] BuildResultVector(ApInt mask, IReadOnlyList<VarNode> variables, AstNode ast, bool multiBit, ApInt numCombinations)
        {
            // Note that the jit based evaluator is an order of magnitude faster, 
            // but is only available for windows x64. 
            if(JitCompatibility.IsSupported)
                return JitResultVector(mask, variables, ast, multiBit, numCombinations);

            return InterpretResultVector(mask, variables, ast, multiBit, numCombinations);
        }

        // Evaluate for all possible combinations of zeroes and ones.
        public static unsafe ApInt[] JitResultVector(ApInt mask, IReadOnlyList<VarNode> variables, AstNode ast, bool multiBit, ApInt numCombinations)
        {
            return new AstJit((byte*)JitPage.Value, mask, variables, ast, multiBit, numCombinations).BuildResultVector();
        }

        // Evaluate for all possible combinations of zeroes and ones.
        public static ApInt[] InterpretResultVector(ApInt mask, IReadOnlyList<VarNode> variables, AstNode ast, bool multiBit, ApInt numCombinations)
        {
            // Compute the number of result vector entries.
            // For single bit SiMBA, this is 2^n. For multi-bit, it is (2^n) * bit_width.
            ApInt varCount = (ApInt)variables.Count;

            // Build the result vector.
            var values = new ApInt[(int)varCount];
            var resultVec = new ApInt[(int)(numCombinations * (multiBit ? ast.BitSize : 1))];
            int arrIdx = 0;
            for (ushort bitIndex = 0; bitIndex < GetNumBitIterations(multiBit, ast.BitSize); bitIndex++)
            {
                for (ApInt i = 0; i < numCombinations; i++)
                {
                    // Set each variable's value.
                    for (ApInt v = 0; v < varCount; v++)
                    {
                        // Compute a mask for this this variable.
                        ApInt varMask = (ApInt)1 << (ushort)v;

                        // Compute the value of this specific variable.
                        ApInt varValue = i & varMask;

                        // Shift the value back down to bit zero.
                        varValue = varValue >> (ushort)v;

                        // Shift the variable value(which is either zero or one) up to the current bitIndex.
                        varValue <<= bitIndex;

                        values[(int)v] = varValue;
                    }

                    // Evaluate the AST for it's combination of zeroes and ones.
                    // Also apply a mask to truncate the result down to our bitwidth.
                    var eval = (ApInt)new AstEvaluator(values.Select(x => (UInt128)x).ToArray(), (int)ast.BitSize).Eval(ast);
                    eval = mask & eval;

                    // For the N-bit to 1 bit transform to hold for an arbitrary bit index,
                    // we need to shift the result down by N bits.
                    eval >>= bitIndex;
                    resultVec[arrIdx] = eval;
                    arrIdx++;
                }
            }

            return resultVec;
        }

        // If multibit is requested, return the bit width of the expression.
        // Otherwise return 1 for single bit MBA simplification.
        public static int GetNumBitIterations(bool multiBit, uint bitSize)
        {
            return (int)(multiBit ? bitSize : 1);
        }

        // Check if the result vector corresponds to a linear expression.
        private static bool IsLinearResultVector(ApInt mask, uint bitSize, ApInt[] resultVec, IReadOnlyList<VarNode> variables, ApInt numCombinations)
        {
            var varCount = (uint)variables.Count;
            AstNode expr = null;

            // Fetch the constant offset at vector entry zero.
            // Then convert the result vector entry for bit index 0 into a linear combination of basis expressions,
            // with the constant offset removed.
            var constant = resultVec[0];
            for (uint i = 0; i < (uint)numCombinations; i++)
            {
                // Skip if the coefficient is zero.
                var coeff = mask & resultVec[(int)i] - constant;
                if (coeff == 0)
                    continue;

                // Set each variable's value.
                AstNode term = null;
                for (ApInt v = 0; v < varCount; v++)
                {
                    // Compute a mask for this this variable.
                    ApInt varMask = (ApInt)1 << (ushort)v;

                    // Compute the value of this specific variable.
                    ApInt varValue = i & varMask;

                    // Shift the value back down to bit zero.
                    varValue = varValue >> (ushort)v;

                    // Update the conjunction depending on whether the variable is set or not.
                    AstNode varNode = varValue != 0 ? variables[(int)v] : new NegNode(variables[(int)v]);
                    if (term == null)
                        term = varNode;
                    else
                        term = new AndNode(term, varNode);
                }

                // Append the term.
                term = new MulNode(new ConstNode(coeff, bitSize), term);
                if (expr == null)
                    expr = term;
                else
                    expr = new AddNode(expr, term);
            }

            // If the expression is null then all entries are zero.
            // TODO: If result vec at idx 0 is uniformly zero, skip the expensive evaluation logic
            // and directly compare the input result vector to zero.
            if (expr == null)
                expr = new ConstNode((ApInt)0, bitSize);

            // Build a result vector for the linear combination.
            var otherVec = BuildResultVector(mask, variables, expr, true, numCombinations);

            // Compare the result vectors after removing the constant offset.
            // Note that doing this on the multi-bit level requires that you shift 
            // the constant offset down, accordingly to the bit index you are operating at.
            for (int comb = 0; comb < resultVec.Length; comb += (int)numCombinations)
            {
                var shiftBy = (ushort)((uint)comb / numCombinations);
                ApInt constantOffset = mask & constant >> shiftBy;
                for (int i = 0; i < (int)numCombinations; i++)
                {
                    var v0 = mask & (resultVec[comb + i] - constantOffset);
                    var v1 = mask & (otherVec[comb + i]);
                    if (v0 != v1)
                        return false;
                }
            }

            // Otherwise the N bit to 1 bit transform holds across all bits,
            // so we can simplify this using single bit SiMBA.
            return true;
        }

        private AstNode Simplify(bool useZ3 = false, bool alreadySplit = false)
        {
            if (multiBit)
            {
                return SimplifyMultibit();
            }

            return SimplifyOneBit(alreadySplit);
        }

        private AstNode SimplifyMultibit()
        {
            // Find an initial solution.
            SimplifyGeneric();

            // Try to improve the solution using one-bit SiMBA.
            // First collect all bitwise constants.
            var solution = res;

            // Apply constant unmerging and substitution of constants.
            // Note that if there are too many bitwise constants, 
            // we bail out.
            var maybeUnmerged = ConstantSubstituter.Apply(solution, variables.Select(x => x.Name).ToHashSet());
            if (maybeUnmerged != null)
            {
                var (unmerged, substitutionMapping) = maybeUnmerged.Value;

                // If there are more than constants, skip this step. It would take too long.
                if (substitutionMapping.Count > 5)
                    return solution;

                // Run SiMBA on the substituted expression.
                var result = Run(unmerged);

                // Back-substitute the constants
                var final = ConstantSubstituter.ApplyBackSubstitution(result, substitutionMapping.ToDictionary(x => x.Value, x => x.Key));

                // Apply constant folding.
                final = new ConstantFolder(bitSize, (long)moduloMask).TryFold(final);

                var c1 = GetCost(solution, false);
                var c2 = GetCost(final, false);

                // If SiMBA returns something that is simpler or at least as simple, use it.
                if (c2 <= c1)
                    return final;
            }

            // Otherwise prefer our solution.
            return solution;
        }

        // Find an initial linear combination of conjunctions.
        private void SimplifyGeneric()
        {
            var l = resultVector.Length;

            // Fetch the constant offset.
            var constant = resultVector[0];

            // Subtract the constant offset from the result vector.
            // Note that doing this on the multi-bit level requires that you shift 
            // the constant offset down, accordingly to the bit index you are operating at.
            unsafe
            {
                fixed (ApInt* ptr = &resultVector[0])
                {
                    for (int comb = 0; comb < l; comb += (int)numCombinations)
                    {
                        var shiftBy = (ushort)((uint)comb / numCombinations);
                        ApInt constantOffset = moduloMask & (constant >> shiftBy);
                        for (int i = 0; i < (int)numCombinations; i++)
                        {
                            ptr[comb + i] = moduloMask & (ptr[comb + i] - constantOffset);
                        }
                    }
                }
            }

            // Short circuit if we can find a single term solution.
            if (multiBit)
            {
                var asBoolean = AsPureBoolean();
                if (asBoolean != null)
                {
                    CheckSolutionComplexity(asBoolean, 1, new ConstNode(constant, bitSize));
                    return;
                }
            }

            var first = true;
            int termCount = 0;
            AstNode expr = null;
            if (constant != 0)
            {
                expr = new ConstNode(constant, bitSize);
                termCount += 1;
            }

            // Get all combinations of variables.
            var variableCombinations = GetVariableCombinations(variables.Count);

            // Linear combination, where the index can be seen as an index into `variableCombinations`,
            // and the element at that index is a list of terms operating over that boolean combination.
            // Term = coeff*(bitMask&basisExpression).
            var linearCombinations = new List<List<(ApInt coeff, ApInt bitMask)>>(variableCombinations.Length);
            for (int i = 0; i < variableCombinations.Length; i++)
                linearCombinations.Add(new((int)bitSize));

            // Keep track of which variables are demanded by which combination,
            // as well as which result vector idx corresponds to which combination.
            List<(ulong trueMask, int resultVecIdx)> combToMaskAndIdx = new();
            for (int i = 0; i < variableCombinations.Length; i++)
            {
                var myMask = variableCombinations[i];
                var myIndex = GetGroupSizeIndex(groupSizes, myMask);
                combToMaskAndIdx.Add((myMask, (int)myIndex));
            }

            bool allZeroes = true;
            var varCount = variables.Count;
            bool onlyOneVar = varCount == 1;
            int width = (int)(varCount == 1 ? 1 : 2u << (ushort)(varCount - 1));

            unsafe
            {
                fixed (ApInt* ptr = &resultVector[0])
                {
                    for (ushort bitIndex = 0; bitIndex < GetNumBitIterations(multiBit, bitSize); bitIndex++)
                    {
                        // If multi-bit simba is enabled, we need to take our base expression
                        // and isolate only the bit at the current bit index.
                        var maskForIndex = multiBit ? (ApInt)1 << bitIndex : moduloMask;
                        // Offset the result vector index such that we are fetching entries for the current bit index.
                        var offset = bitIndex * numCombinations;
                        for (int i = 0; i < variableCombinations.Length; i++)
                        {
                            // Fetch the result vector index for this conjunction.
                            // If the coefficient is zero, we can skip it.
                            var comb = variableCombinations[i];
                            var (trueMask, index) = combToMaskAndIdx[i];
                            var coeff = ptr[(int)offset + index];
                            if (coeff == 0)
                                continue;

                            // Subtract the coefficient from the result vector.
                            allZeroes = false;
                            SubtractCoeff(moduloMask, ptr, bitIndex, coeff, index, width, varCount, onlyOneVar, trueMask);

                            // Add an entry to the linear combination list.
                            linearCombinations[i].Add((coeff, maskForIndex));
                        }
                    }
                }
            }

            // Simplify the linear combination and turn it back into an expression.
            if (multiBit)
            {
                var (simplified, numTerms) = SimplifyMultiBitGeneric(constant, variableCombinations, linearCombinations);
                CheckSolutionComplexity(simplified);
                lincombTerms = termCount;
                return;
            }

            else
            {
                var (simplified, numTerms) = SimplifyOneBitGeneric(variableCombinations, linearCombinations);

                termCount += numTerms;
                if (expr == null)
                    expr = simplified;
                else
                    expr = simplified == null ? expr : new AddNode(expr, simplified); 

                // If the expression is null, we have an all zeroes entry.
                if (expr == null)
                {
                    Debug.Assert(allZeroes);
                    expr = new ConstNode((ulong)0, bitSize);
                }

                CheckSolutionComplexity(expr, termCount);
                lincombTerms = termCount;
            }
        }

        // Try to find a single boolean term fitting the result vector.
        private AstNode? AsPureBoolean()
        {
            var rv = resultVector;

            // Keep track of the bits being demanded of each base expression.
            List<ApInt> demandedBits = Enumerable.Range(0, (int)numCombinations).Select(x => (ApInt)0).ToList();
            // Single non-zero coefficient being multiplied over the base expression.
            ApInt firstNonzero = 0;
            for (int comb = 0; comb < resultVector.Length; comb += (int)numCombinations)
            {
                var bitIndex = (ushort)((uint)comb / numCombinations);
                var bitMask = (ApInt)1 << bitIndex;
                for (int i = 0; i < (int)numCombinations; i++)
                {
                    // Skip zero entries.
                    var entry = resultVector[comb + i];
                    if (entry == 0)
                        continue;

                    // If this is the first non-zero entry, cache it.
                    if (firstNonzero == 0)
                    {
                        firstNonzero = entry;
                        demandedBits[i] |= bitMask;
                        continue;
                    }

                    // If the entry is equivalent to all other non-zero entries, skip it.
                    if (entry == firstNonzero)
                    {
                        demandedBits[i] |= bitMask;
                        continue;
                    }

                    // Because bit masks are involved, there is a degree of flexibility
                    // in the coefficient being multiplied over the boolean function. 
                    // If we can legally change the coefficient to the first non-zero coefficient,
                    // then two entries are still equivalent.
                    bool canChangeCoefficient = refiner.CanChangeCoefficientTo(entry, firstNonzero, bitMask);
                    if (!canChangeCoefficient)
                        return null;
                    demandedBits[i] |= bitMask;
                }
            }

            // If we only found one unique entry, we have a single term solution.
            AstNode result = null;
            for (int i = 0; i < (int)numCombinations; i++)
            {
                // Skip this entry if no bits are demanded.
                var demandedMask = demandedBits[i];
                if (demandedMask == 0)
                    continue;

                // Try to minimize the number of set bits in a mask.
                demandedMask = refiner.FindMinimalMask(firstNonzero, demandedMask);

                // Check if we can remove the bitmask altogether.
                bool canRemoveMask = refiner.CanRemoveMask(firstNonzero, demandedMask);
                ApInt? conjMask = canRemoveMask ? null : demandedMask;
                var conj = GetBooleanForIndex(i);
                var term = Conjunction(1, conj, conjMask);
                result = Or(result, term);
            }

            result = Term(result, firstNonzero);
            if (dbg)
                Console.WriteLine(result);
            return result;
        }

        // Get a naive boolean function for the given result vector idx.
        private AstNode GetBooleanForIndex(int resultVecIdx)
        {
            AstNode ast = null;
            for (int varIdx = 0; varIdx < variables.Count; varIdx++)
            {
                uint vMask = 1u << (ushort)varIdx;
                bool isSet = (resultVecIdx & vMask) != 0;
                AstNode term = isSet ? variables[varIdx] : new NegNode(variables[varIdx]);
                if (ast == null)
                    ast = term;
                else
                    ast = new AndNode(ast, term);
            }

            return ast;
        }

        public static ulong[] GetVariableCombinations(int varCount)
        {
            var numEntries = (1 << (ushort)varCount) - 1;
            var outputs = new ulong[numEntries];
            for (int i = 0; i < numEntries; i++)
                outputs[i] = 0;
            for (int i = 0; i < varCount; i++)
                outputs[i] = 1u << (ushort)i;

            int combCount = varCount;
            var _new = varCount;
            for (int count = 1; count < varCount; count++)
            {
                var size = combCount;
                var nnew = 0;
                var from = size - _new;
                for (int ei = from; ei < from + (size - from); ei++)
                {
                    var e = outputs[ei];
                    var lastIdx = 64 - BitOperations.LeadingZeroCount((ulong)e);
                    for (int v = lastIdx; v < varCount; v++)
                    {
                        outputs[combCount] |= (1u << (ushort)v);
                        outputs[combCount] |= e;
                        combCount += 1;
                        nnew += 1;
                    }
                }

                _new = nnew;
            }

            return outputs;
        }

        public static uint GetGroupSizeIndex(List<int> groupSizes, ulong varMask)
        {
            uint sum = 0;
            while (varMask != 0)
            {
                var lsb = BitOperations.TrailingZeroCount(varMask);
                sum += (uint)groupSizes[lsb];

                varMask ^= (1ul << (ushort)lsb);
            }

            return sum;
        }

        public static unsafe void SubtractCoeff(ApInt moduloMask, ApInt* pResultVec, ushort bitIndex, ApInt coeff, int firstStart, int width, int varCount, bool onlyOneVar, ulong trueMask)
        {
            var offset = bitIndex * width;

            var v0 = BitOperations.TrailingZeroCount(trueMask);
            var groupSize1 = (int)((ulong)1 << (ushort)v0);
            var period1 = 2 * groupSize1;
            for (int start = firstStart; start < width; start += period1)
            {
                for (int i = start; i < start + groupSize1; i++)
                {
                    var castedI = (ulong)(uint)i;
                    var isTrue2 = (castedI & trueMask) == trueMask;
                    if (i != firstStart && (onlyOneVar || isTrue2))
                        pResultVec[offset + i] = moduloMask & (pResultVec[offset + i] - coeff);
                }
            }
        }

        // Returns true if the given variable indices are set to true within the result vector entry.
        private static bool AreVariablesTrue(ApInt n, List<int> variables)
        {
            ApInt prev = 0;
            foreach (var v in variables)
            {
                n >>= (ushort)((uint)v - prev);
                prev = (ApInt)v;
                if ((n & 1) == 0)
                    return false;
            }

            return true;
        }

        // Convert a N-bit result vector into a linear combination.
        private (AstNode expr, int termCount) SimplifyMultiBitGeneric(ApInt constantOffset, ulong[] variableCombinations, List<List<(ApInt coeff, ApInt bitMask)>> linearCombinations)
        {
            Log("Initial solution: ");
            if (constantOffset != 0)
                Log(new ConstNode(constantOffset, bitSize));

            // Construct the lin comb vector.
            List<Dictionary<ApInt, ApInt>> results = new();
            for (int i = 0; i < linearCombinations.Count; i++)
            {
                // Skip if there are no entries for this basis expression.
                var entries = linearCombinations[i];
                if (entries.Count == 0)
                {
                    results.Add(new());
                    continue;
                }

                // Try to simplify the linear combination.
                var varComb = variableCombinations[i];
                var coeffToMask = refiner.SimplifyMultibitEntry(entries);
                results.Add(coeffToMask);

                // TODO: Comment
                foreach (var (coeff, mask) in coeffToMask)
                {
                    if (coeff == 0 || mask == 0)
                        continue;
                    var conj = ConjunctionFromVarMask(coeff, varComb, mask);
                    Log(conj);
                }
            }

            // We have a near-optimal linear combination of conjunctions.
            // Try to apply several more refinements, that are simpler but don't necessarily only result in a linear combinaton of conjunctions(e.g. we now allow XORs).
            var final = TryRefineMultibit(constantOffset, variableCombinations, results);

            return (final, GetCost(final, false));
        }

        private AstNode TryRefineMultibit(ApInt constantOffset, ulong[] variableCombinations, List<Dictionary<ApInt, ApInt>> linearCombinations)
        {
            List<List<AstNode>> extractedTerms = new();
            for (int i = 0; i < linearCombinations.Count; i++)
            {
                (constantOffset, var newTerms) = TryRefineMultibitEntry(constantOffset, variableCombinations[i], linearCombinations[i]);
                extractedTerms.Add(newTerms);
            }

            // Construct the linear combination.
            AstNode final = constantOffset != 0 ? new ConstNode(constantOffset, bitSize) : null;
            for (int i = 0; i < linearCombinations.Count; i++)
            {
                var newTerms = extractedTerms[i];
                foreach (var t in newTerms)
                {
                    if (final == null)
                        final = t;
                    else
                        final = new AddNode(final, t);
                }

                foreach (var (coeff, mask) in linearCombinations[i])
                {
                    if (coeff == 0)
                        continue;
                    if (mask == 0)
                        continue;

                    var newTerm = ConjunctionFromVarMask(coeff, variableCombinations[i], mask);
                    if (final == null)
                        final = newTerm;
                    else
                        final = new AddNode(final, newTerm);
                }
            }

            if (dbg)
                Log($"New solution: {final}");

            return final;
        }

        // Given a linear combination where every node is using the same basis expression, try to find the simplest possible representation.
        private (ApInt newConstantOffset, List<AstNode> newTerms) TryRefineMultibitEntry(ApInt constantOffset, ulong varComb, Dictionary<ApInt, ApInt> coeffToMask)
        {
            refiner.TryReduceMasks(coeffToMask);

            // If we succeeded, update accordingly.
            List<AstNode> newTerms = new();
            var processXor = ((ApInt adjustedConstantOffset, ApInt coeff, ApInt xorMask)? maybeXor) =>
            {
                if (maybeXor == null)
                    return;
                // Update the constant offset.
                var xor = maybeXor.Value;
                constantOffset = xor.adjustedConstantOffset;

                // Add a new XOR term.
                var xorConst = new ConstNode(xor.xorMask, ast.BitSize);
                var xorNode = new XorNode(xorConst, ConjunctionFromVarMask(1, varComb, null));
                var newTerm = new MulNode(new ConstNode(xor.coeff, bitSize), xorNode);
                newTerms.Add(newTerm);
            };

            // If we succeeded, update accordingly.
            var processOr = ((ApInt adjustedConstantOffset, ApInt coeff, ApInt orMask)? maybeOr) =>
            {
                if (maybeOr == null)
                    return;
                // Update the constant offset.
                var or = maybeOr.Value;
                constantOffset = or.adjustedConstantOffset;

                // Add a new OR term.
                // If the mask is -1 then we ignore it.
                var conj = ConjunctionFromVarMask(1, varComb, null);
                AstNode orNode = null;
                if (or.orMask != moduloMask)
                {
                    var orConst = new ConstNode(or.orMask, ast.BitSize);
                    orNode = new OrNode(orConst, conj);
                }

                else
                {
                    orNode = conj;
                }

                var newTerm = new MulNode(new ConstNode(or.coeff, bitSize), orNode);
                newTerms.Add(newTerm);
            };

            var processIsolation = (ApInt? coeff) =>
            {
                if (coeff == null || coeff.Value == 0)
                    return;

                var conj = ConjunctionFromVarMask(coeff.Value, varComb, null);
                newTerms.Add(conj);
            };

            // Try to identify trivial XORs by constant.
            processXor(refiner.TrySimplifyXor(constantOffset, coeffToMask));

            // Try to identify whole instances of variables.
            var maybeIsolated = refiner.TryIsolateSingleVariableConjunction(coeffToMask);
            processIsolation(maybeIsolated);

            // If we succeeded in isolating out a whole term(no bit mask), try again to factor out a XOR by constant.
            if (maybeIsolated != null)
                processXor(refiner.TrySimplifyXor(constantOffset, coeffToMask));

            // While recovering XORs is almost guaranteed to be profitable, 
            // recovering bitwise ORs is generally not. The constant offset gets partitioned into
            // too many terms, which increases the complexity.
            // There may be a heuristic that can be used here.
            while (false)
            {
                var maybeOr = refiner.TrySimplifyOr(constantOffset, coeffToMask);
                if (maybeOr == null)
                    break;
                processOr(maybeOr);
            }

            refiner.TryReduceMasks(coeffToMask);

            // If enabled, use just one level of recursion to search for a simpler combination
            // of conjunctions after subtracting the constant offset.
            if (tryDecomposeMultiBitBases)
            {
                var decomposed = TryDecomposeMultibitBases(constantOffset, varComb, newTerms, coeffToMask);
                if (decomposed != null)
                    return decomposed.Value;
            }

            return (constantOffset, newTerms);
        }

        private (ApInt newConstantOffset, List<AstNode> newTerms)? TryDecomposeMultibitBases(ApInt constantOffset, ulong varComb, List<AstNode> newTerms, Dictionary<ApInt, ApInt> coeffToMask)
        {
            // Convert the current set to a linear combination of bitwise functions.
            var firstSolution = GetLinearComb(0, varComb, newTerms, coeffToMask);

            ApInt vec0 = 0;
            ApInt vec1 = 0;
            foreach (var (coeff, mask) in coeffToMask)
            {
                vec0 += coeff * (mask & 0);
                vec1 += coeff * (mask & 1);
            }

            // Subtract off the constant factor.
            vec1 -= vec0;

            // We want the 1 bit result vector to yield an optional constant offset + a coefficient over the variable.
            // If the second entry is zero then there's no point to applying decomposition.
            vec0 &= moduloMask;
            vec1 &= moduloMask;
            if (vec1 == 0)
                return null;

            // For now assume there is no constant offset.
            // If there is, we need to append it to the lincomb below!
            Debug.Assert(vec0 == 0);

            // Build the term we are subtracting by.
            var subst = new VarNode("subst", bitSize);
            var subtractBy = new MulNode(new ConstNode(vec1, bitSize), subst);

            // Convert the dictionary to an ast representation of a linear combination.
            AstNode linComb = null;
            foreach (var (coeff, mask) in coeffToMask)
            {
                // Skip zero entries.
                if (coeff == 0 || mask == 0)
                    continue;

                // Add the term.
                var maskNode = new ConstNode(mask, bitSize);
                var term = new MulNode(new ConstNode(coeff, bitSize), new AndNode(maskNode, subst));
                if (linComb == null)
                    linComb = term;
                else
                    linComb = new AddNode(linComb, term);
            }

            // If there were no non-zero terms, there's nothing to simplify.
            if (linComb == null)
                return null;

            // Subtract our displacement term from the linear comb.
            // Note that multiplying by this.mask is equivalent to multiplication by negative one.
            linComb = new AddNode(linComb, new MulNode(new ConstNode(moduloMask, bitSize), subtractBy));

            // Try to simplify this linear combination.
            var simpl = Run(linComb, true, true, tryDecomposeMultiBitBases: false);
            simpl = new AddNode(subtractBy, simpl);

            // Compute the basis expression ast.
            var basis = ConjunctionFromVarMask(1, varComb, null);

            // Replace the substituted variable with the basis. TODO: Stop using string parsing.
            var formatted = simpl.ToString().Replace(subst.Name, basis.ToString());
            simpl = AstParser.Parse(formatted, bitSize);

            // Calculate the cost of both solutions.
            var cost0 = GetCost(firstSolution, false);
            var cost1 = GetCost(simpl, false);

            // If the original solution was the best, use it.
            if (cost0 <= cost1)
                return (constantOffset, newTerms);

            // Otherwise the new solution was the best.
            // Update the coefficient to mask mapping.
            coeffToMask.Clear();
            var terms = new List<AstNode>();
            SplitIntoTerms(simpl, terms);
            foreach (var term in terms)
            {
                var processBitop = (ApInt coeff, ApInt bitMask) =>
                {
                    coeffToMask.TryAdd(coeff, 0);
                    var existing = coeffToMask[coeff];
                    Debug.Assert((existing & bitMask) == 0);

                    // Must be disjoint.
                    coeffToMask[coeff] = existing | bitMask;
                };
                if (term is MulNode mulNode)
                {
                    // If m1 * (mask&bitop), update the dictionary.
                    var coeff = (ApInt)(mulNode.Children[0] as ConstNode).Value;
                    var bitop = mulNode.Children[1];
                    if (bitop is AndNode && bitop.Children[0] is ConstNode cn)
                    {
                        processBitop(coeff, (ApInt)cn.Value);
                    }

                    // If there's no bitmask, just add it as a new term.
                    else
                    {
                        newTerms.Add(term);
                    }
                }

                else if (term is AndNode)
                {
                    // If it has a bit mask, add it to the mapping.
                    if (term.Children[0] is ConstNode cn)
                    {
                        processBitop(1, (ApInt)cn.Value);
                    }

                    else
                    {
                        // Otherwise just add it to the term list.
                        newTerms.Add(term);
                    }
                }

                else if (term is VarNode)
                {
                    newTerms.Add(term);
                }

                else
                {
                    throw new InvalidOperationException($"Unrecognized term!");
                }
            }

            return (constantOffset, newTerms);
        }

        private AstNode GetLinearComb(ApInt constant, ulong varComb, List<AstNode> newTerms, Dictionary<ApInt, ApInt> coeffToMask)
        {
            // TODO: Constant support.
            Debug.Assert(constant == 0);

            AstNode result = null;
            foreach (var t in newTerms)
                result = Add(result, t);

            foreach (var (coeff, mask) in coeffToMask)
            {
                if (coeff == 0 || mask == 0)
                    continue;

                var t = ConjunctionFromVarMask(coeff, varComb, mask);
                result = Add(result, t);
            }

            return result;
        }

        private int GetCost(AstNode ast, bool inBitwise)
            => GetCost(ast, inBitwise, moduloMask);

        public static int GetCost(AstNode ast, bool inBitwise, ApInt mask)
        {
            var op0 = (bool inBitwise) => GetCost(ast.Children[0], inBitwise, mask);
            var op1 = (bool inBitwise) => GetCost(ast.Children[1], inBitwise, mask);
            return ast switch
            {
                AddNode => 1 + op0(inBitwise) + op1(inBitwise),
                MulNode => 1 + op0(inBitwise) + op1(inBitwise),
                AndNode => 1 + op0(true) + op1(true),
                OrNode => 1 + op0(true) + op1(true),
                XorNode => 1 + op0(true) + op1(true),
                NegNode => 1 + op0(inBitwise),
                // If a constant node is inside of a bitwise AND, check if it is equivalent to our modulo mask.
                // If it is, then it's zero cost. Otherwise cost 1.
                ConstNode constNode => inBitwise ? (ApInt)constNode.Value == mask ? 0 : 1 : 1,
                VarNode varNode => 1,
                _ => throw new InvalidOperationException($"Cannot calculate cost for type: {ast.Kind}")

            };
        }

        private AstNode Add(AstNode t0, AstNode t1)
        {
            if (t0 == null)
                return t1;

            return new AddNode(t0, t1);
        }

        private AstNode Or(AstNode t0, AstNode t1)
        {
            if (t0 == null)
                return t1;

            return new OrNode(t0, t1);
        }

        private AstNode SimplifyOneBit(bool alreadySplit)
        {
            // If there is more than 3 variables:
            HashSet<ApInt> resultSet = null;
            if (variables.Count > 3)
            {
                if (alreadySplit)
                {
                    resultSet = resultVector.ToHashSet();
                    if (refine && resultSet.Count == 1)
                    {
                        SimplifyOneValue(resultSet);
                    }

                    else
                    {
                        SimplifyGeneric();
                        if (refine)
                            TryRefine();
                    }
                }

                else
                {
                    // Run the simplification procedure.
                    SimplifyGeneric();

                    // If we cannot simplify the linear MBA further using knowledge of a newly reduced variable count,
                    // try to partition the linear MBA by it's variables, then simplify further.
                    if (!TrySimplifyFewerVariables())
                    {
                        TrySplit();
                    }
                }
            }

            else
            {
                resultSet = resultVector.ToHashSet();
                if (refine && resultSet.Count == 1)
                {
                    SimplifyOneValue(resultSet);
                }

                else
                {
                    SimplifyGeneric();
                    if (refine)
                        TryRefine();
                }
            }

            Debug.Assert(res != null);
            return res;
        }


        // Convert a 1-bit result vector into a linear combination.
        private (AstNode expr, int termCount) SimplifyOneBitGeneric(ulong[] variableCombinations, List<List<(ApInt coeff, ApInt bitMask)>> linearCombinations)
        {
            AstNode expr = null;
            int termCount = 0;
            for (int i = 0; i < linearCombinations.Count; i++)
            {
                // Skip if there are no terms with this basis expression.
                var entries = linearCombinations[i];
                if (entries.Count == 0)
                    continue;

                // For a 1-bit result vector there is only 1 entry for any given basis expression.
                var entry = entries[0];

                // Construct the term.
                var term = ConjunctionFromVarMask(entry.coeff, variableCombinations[i], null);
                if (expr == null)
                    expr = term;
                else
                    expr = new AddNode(expr, term);
                termCount++;
            }

            return (expr, termCount);
        }

        private void SimplifyOneValue(HashSet<ApInt> resultSet)
        {
            var coefficient = resultSet.First();
            resultSet.Remove(coefficient);
            CheckSolutionComplexity(new ConstNode(coefficient, bitSize), 1);
        }


        // For the optimal currently known solution, check how many variables it uses effectively.
        // If it is not more than 3, run the simplification procedure again for that variable count,
        // since we might be able to simplify the expression using truth tables.
        bool TrySimplifyFewerVariables()
        {
            Debug.Assert(res != null);

            // Clone the AST, while collecting a set of unique variables.
            var t = AstCloner.CloneAst(res, new(), res.BitSize);
            // Get the unique set of variables.
            // Note that this mutates the variable objects such that their 
            // index field is updated based off of their alphabetical name ordering.
            var newVariables = InputVariableUtility.GetVariables(t);

            // If there are more than three variables, we cannot simplify it any further.
            if (newVariables.Count > 3)
                return false;
            // If the variable count remains unchanged, we cannot simplify any further.
            if (newVariables.Count == variables.Count)
                return false;

            // Run the linear MBA simplifier on the
            var expr = Run(t);

            // Map name to variable.
            Dictionary<string, VarNode> nameToVar = new();
            foreach (var v in variables)
                nameToVar.Add(v.Name, v);

            // Clone the AST while substituting the variables back in.
            expr = BooleanSimplifier.RewriteUsingNewVariables(expr, (v) => nameToVar[v.Name]);
            CheckSolutionComplexity(expr);
            return true;
        }

        // Try to split the given expression, which is supposed to be a linear MBA,
        // into subexpressions with at most 3 variables each such that the list of 
        // occurring variables is partitioned thereby, simplify these subexpressions
        // and compose the results.
        void TrySplit()
        {
            var expr = res;
            Debug.Assert(expr != null);

            var l = new List<AstNode>();
            SplitIntoTerms(expr, l);
            var v = FindVariablesInTerms(l);

            var (constIdx, l1, l2, l3, lrem) = PartitionTermsByVariableCount(l, v);
            var partition = Partition(v, l1, l2, l3, lrem);
            var e = SimplifyPartsAndCompose(l, partition, constIdx, lrem.ToHashSet());
            CheckSolutionComplexity(e);
        }

        private int CountTerms(AstNode expr)
        {
            var terms = new List<AstNode>();
            SplitIntoTerms(expr, terms);
            return terms.Count;
        }

        public static void SplitIntoTerms(AstNode expr, List<AstNode> terms)
        {
            if (expr is AddNode)
            {
                var op1 = expr.Children[0];
                var op2 = expr.Children[1];
                if (op1 is AddNode)
                {
                    SplitIntoTerms(op1, terms);
                }

                else
                {
                    terms.Add(op1);
                }

                if (op2 is AddNode)
                {
                    SplitIntoTerms(op2, terms);
                }

                else
                {
                    terms.Add(op2);
                }
            }

            else
            {
                terms.Add(expr);
            }
        }

        private List<HashSet<int>> FindVariablesInTerms(List<AstNode> terms)
        {
            var sets = new List<HashSet<int>>();
            foreach (var e in terms)
            {
                var vars = InputVariableUtility.GetVarSet(e).Select(x => variables.IndexOf(x)).ToHashSet();
                sets.Add(vars);
            }

            return sets;
        }

        private (int, List<int>, List<int>, List<int>, List<int>) PartitionTermsByVariableCount(List<AstNode> l, List<HashSet<int>> v)
        {
            var constIdx = -1;

            // Terms with 1 variable.
            var l1 = new List<int>();
            // Terms with 2 variables.
            var l2 = new List<int>();
            // Terms with 3 variables.
            var l3 = new List<int>();
            // Terms with at least 4 variables.
            var lrem = new List<int>();

            foreach (var i in RangeUtil.Get(v.Count))
            {
                var lv = v[i].Count;
                if (lv == 0)
                {
                    Debug.Assert(constIdx == -1);
                    constIdx = i;
                    continue;
                }

                else if (lv == 1)
                {
                    l1.Add(i);
                }

                else if (lv == 2)
                {
                    l2.Add(i);
                }

                else if (lv == 3)
                {
                    l3.Add(i);
                }

                else
                {
                    lrem.Add(i);
                }
            }

            return (constIdx, l1, l2, l3, lrem);
        }

        // Partition the variables of the given list and the terms of the given list
        // correspondingly such that all terms with indices in an entry of the term
        // partition only use variables of the corresponding part of the variable
        // partition. If the terms cannot perfectly be split, the indices of term
        // that cannot be distributed to the partition are stored in the list of
        // remaining term indices.
        List<HashSet<int>> Partition(List<HashSet<int>> v, List<int> l1, List<int> l2, List<int> l3, List<int> lrem)
        {
            var partitionV = new List<HashSet<int>>();
            var partitionT = new List<HashSet<int>>();
            var remV = new HashSet<int>();

            foreach (var i in lrem)
                remV.AddRange(v[i]);

            var l23 = l2.Concat(l3).ToList();
            foreach (var i in l23)
            {
                // The variable set has a nonempty intersection with the variables
                // we cannot split, so merge them.
                if (v[i].Intersect(remV).Any())
                {
                    remV.AddRange(v[i]);
                    lrem.Add(i);
                    continue;
                }

                // Now check whether the term matches any partition's part, if existent.
                if (TryFindMatchingPartition(i, v[i], partitionV, partitionT))
                    continue;

                // If the term does not match any, check whether it violates any.
                // If so, reject it.
                var (intersections, valid) = DetermineIntersections(v[i], partitionV);

                // Create the new part.
                if (intersections.Count == 0)
                {
                    partitionV.Add(v[i]);
                    partitionT.Add(new() { i });
                }

                // We can merge the variable set into an existing one.
                else if (intersections.Count == 1)
                {
                    partitionV[intersections[0]].AddRange(v[i]);
                    partitionT[intersections[0]].Add(i);
                }

                // We have a conflict.
                else
                {
                    remV.AddRange(v[i]);
                    lrem.Add(i);

                    foreach (var j in intersections.Reverse<int>())
                    {
                        remV.AddRange(partitionV[j]);
                        lrem.AddRange(partitionT[j]);

                        partitionV.RemoveAt(j);
                        partitionT.RemoveAt(j);
                    }
                }
            }

            // Now check for each term with 1 variable whether it matches any
            // partitions' part, if existent. Note that it cannot violate any.
            bool done = false;
            foreach (var i in l1)
            {
                var _var = v[i].First();
                v[i].Remove(_var);

                if (remV.Contains(_var))
                {
                    lrem.Add(i);
                    continue;
                }

                done = false;

                foreach (var j in RangeUtil.Get(partitionV.Count))
                {
                    var part = partitionV[j];

                    // The variable is included in the part.
                    if (part.Contains(_var))
                    {
                        partitionT[j].Add(i);
                        done = true;
                        break;
                    }
                }

                if (!done)
                {
                    partitionV.Add(new() { _var });
                    partitionT.Add(new() { i });
                }
            }


            return partitionT;
        }

        bool TryFindMatchingPartition(int i, HashSet<int> variables, List<HashSet<int>> partitionV, List<HashSet<int>> partitionT)
        {
            Debug.Assert(partitionV.Count == partitionT.Count);

            foreach (var j in RangeUtil.Get(partitionV.Count))
            {
                // The variable set matches the part.
                var diff = variables.Except(partitionV[j]);
                if (diff.Count() == 0)
                {
                    partitionT[j].Add(i);
                    return true;
                }
            }

            return false;
        }

        // Returns all indices of entries in the given partition having a nonempty
        // intersection with the given set of variables. Additionally returns False
        // iff there is at least one intersection with more than 3 variables.
        private (List<int>, bool) DetermineIntersections(HashSet<int> variables, List<HashSet<int>> partitionV)
        {
            var intersections = new List<int>();
            bool valid = true;

            foreach (var j in RangeUtil.Get(partitionV.Count))
            {
                var part = partitionV[j];

                // The variable set has an empty intersection with the part.
                if (variables.Intersect(part).Count() == 0)
                    continue;

                // The variable set has a nonempty intersection with the part,
                // but does not match it. Check whether we can merge it into the
                // part. If not, reject it.
                intersections.Add(j);

                if (valid && (intersections.Count > 0 || variables.Union(part).Count() > 3))
                    valid = false;
            }

            return (intersections, valid);
        }

        // Simplify the sum of terms in the given list for each given partition and
        // compose the results.
        AstNode SimplifyPartsAndCompose(List<AstNode> l, List<HashSet<int>> partition, int constIdx, HashSet<int> lrem)
        {
            // Simplify the partition's parts separately and compose the results.
            AstNode simpl = null;
            foreach (var part in partition)
            {
                var e = ComposeTerms(l, part);

                // Run the simplification procedure on the ast.
                var s = Run(e, true);

                // Append the constant if existent and check whether the result has the same number of terms.
                // If so, use that instead and eliminate the constant.
                if (constIdx != -1)
                {
                    Debug.Assert(constIdx > -1);
                    var constNode = l[constIdx];
                    e = new AddNode(e, constNode);

                    // Run the simplification procedure on the newly created AST.
                    var s2 = Run(e, true);

                    if (CountTerms(s) == CountTerms(s2))
                    {
                        s = s2;
                        constIdx = -1;
                    }
                }

                // If the part is a constant zero, skip it.
                Debug.Assert(s != null);
                if (s is ConstNode cn && cn.Value == 0)
                    continue;

                if (simpl == null)
                    simpl = s;
                else
                    simpl = new AddNode(simpl, s);
            }

            // If there are no remaining terms but the constant, add the constant.
            if (lrem.Count == 0)
            {
                if (constIdx != -1)
                {
                    Debug.Assert(constIdx > -1);
                    var constNode = l[constIdx];
                    if (simpl == null)
                        simpl = constNode;
                    else
                        simpl = new AddNode(simpl, constNode);
                }

                return simpl == null ? new ConstNode((ulong)0, ast.BitSize) : simpl;
            }

            // Now consider the terms which did not fit into the partition, if existent.
            var remainingTerms = ComposeTerms(l, lrem);

            // Append the constant if existent and not used yet.
            if (constIdx != -1)
            {
                var constNode = l[constIdx];
                remainingTerms = new AddNode(constNode, remainingTerms);
            }

            // Run the simplifier on the remaining terms.
            remainingTerms = Run(remainingTerms, alreadySplit: true);

            if (simpl == null)
                simpl = remainingTerms;
            else
                simpl = new AddNode(simpl, remainingTerms);

            return simpl;
        }

        // Returns a sum of the given list's entries with indices contained in the given list.
        AstNode ComposeTerms(List<AstNode> l, HashSet<int> indices)
        {
            AstNode e = null;
            foreach (var i in indices)
            {
                var term = l[i];
                if (e == null)
                    e = term;
                else
                    e = new AddNode(e, term);
            }

            return e;
        }

        private void TryRefine()
        {
            Debug.Assert(lincombTerms != null);

            // Rebuild the result vector since it has been modified during simplification.
            resultVector = BuildResultVector(moduloMask, variables, ast, multiBit, numCombinations);

            // The number of terms in the linear combination of conjunctions only has one term.
            if (CheckTermCount(1))
                return;

            // Build a set of unique result vector values.
            var resultSet = resultVector.ToHashSet();
            var l = resultVector.Length;
            Debug.Assert(l > 0);

            // We apply a set of heuristics to try and find a solution that is simpler than
            // a linear combination of conjunctions. We perform several attempts, numbered according to
            // the paper.

            // (1-3) Try to find a single term that fits.
            TryRefineSingleTerm(resultSet);

            // We cannot simplify the expression better.
            if (CheckTermCount(2))
                return;

            // (4-8) Try to find a sum of two terms that fits.
            TryRefineTwoTerms(resultSet);

            // We cannot simplify the expression better.
            if (CheckTermCount(3))
                return;

            // (9) Try to reduce the number of unique values by expressing one as a
            // combination of the others.
            var constant = ReduceByConstant();
            var constNode = new ConstNode(constant, bitSize);
            var uniqueValues = resultVector.Where(x => x != 0).ToHashSet().ToList();
            TryEliminateUniqueValue(uniqueValues, constNode);

            var c = uniqueValues.Count + (constant == 0 ? 0 : 1);
            if (CheckTermCount(c))
                return;

            // (10) Find expressions corresponding to the unique values.
            var simpler = ExpressionForEachUniqueValue(uniqueValues.ToHashSet());
            simpler = AddConstant(simpler, constNode);
            CheckSolutionComplexity(simpler, c);
        }

        private void TryRefineSingleTerm(HashSet<ApInt> resultSet)
        {
            var l = resultSet.Count;
            Debug.Assert(l > 1);

            // We cannot come down to a single expression in this case.
            if (l > 2)
                return;

            // NOTE: The case (1) that we only have one unique value has already been
            // handled in simplify_one_value().

            // (2) If only one nonzero value occurs and the result for all variables being zero is zero,
            // we can find a single expression.
            if (resultVector[0] == 0)
            {
                var e = ExpressionForEachUniqueValue(resultSet);
                CheckSolutionComplexity(e, 1);
            }

            // (3) Check whether we can find one single negated term.
            TryFindNegatedSingleExpression(resultSet);
        }

        // For the given set of results of the input expression for combinations of
        // truth values, try to find a single negated expression representing the
        // result vector.
        private void TryFindNegatedSingleExpression(HashSet<ApInt> resultSet)
        {
            // We can only find a negated expression if we have 2 distinct values.
            Debug.Assert(resultSet.Count == 2);

            var s = resultSet.ToArray();
            var a = s[0];
            var b = s[1];

            // Exit early if we don't have two terms where one is equivalent to (2 * other).
            var aDouble = IsDoubleModulo(a, b);
            var bDouble = IsDoubleModulo(b, a);
            if (!aDouble && !bDouble)
                return;

            // Make sure that b is double a
            if (aDouble)
            {
                (a, b) = (b, a);
            }

            if (resultVector[0] == b)
                return;

            // Get a negated representation of the boolean truth table.
            var t = resultVector.Select(x => x == b ? 1 : 0).ToList();
            var e = BooleanSimplifier.GetBitwise(variables, t, negate: true);

            e = Term(e, NegateCoefficient(a));

            CheckSolutionComplexity(e, 1);
        }

        private ApInt NegateCoefficient(ApInt coeff)
        {
            // Note that the mask & maxValue * a should be equivalent to taking -a under the modular field.
            return moduloMask & (ApInt.MaxValue * coeff);
        }

        void TryRefineTwoTerms(HashSet<ApInt> resultSet)
        {
            if (resultVector[0] == 0)
                TryRefineTwoTermsFirstZero(resultSet);
            else
                TryRefineTwoTermsFirstNonZero(resultSet);
        }

        private void TryRefineTwoTermsFirstZero(HashSet<ApInt> resultSet)
        {
            Debug.Assert(resultVector[0] == 0);
            var l = resultSet.Count;

            // In this case 1==2 we know that the constant is nonzero since we would have
            // run into the case (2) otherwise.
            // TODO: We can still try to find a solution with 2 terms if we already have one with one term and then compare complexities.

            if (l == 3)
            {
                // (4) We can reduce the expression to two terms.
                FindTwoExpressionsByTwoValues();
            }

            else if (l == 4)
            {
                // (5) We can still come down to 2 expressions if we can express one
                // value as a sum of the others.
                var uniqueValues = resultVector.Where(x => x != 0).ToHashSet().ToList();
                TryEliminateUniqueValue(uniqueValues);
            }
        }

        // For the stored list of results of the input expression for combinations
        // of truth values, try to find two unnegated expressions representing the
        // result vector.
        private void FindTwoExpressionsByTwoValues()
        {
            Debug.Assert(resultVector[0] == 0);
            var resultSet = resultVector.ToHashSet();
            Debug.Assert(resultSet.Count == 3);

            // Remove the zero entry.
            resultSet.Remove(0);
            Debug.Assert(resultSet.Count == 2);

            var a = resultSet.First();
            resultSet.Remove(a);
            var b = resultSet.First();
            resultSet.Remove(b);

            // We have three possible solutions. Return the solution with the lowest complexity.
            DetermineCombOfTwo(a, b);
            DetermineCombOfTwo(moduloMask & (a - b), b);
            DetermineCombOfTwo(a, moduloMask & (b - a));
        }

        private void DetermineCombOfTwo(ApInt coeff1, ApInt coeff2, ApInt[] vec = null, bool secNegated = false)
        {
            var d = GetDecisionVector(coeff1, coeff2, vec);
            var cases = new Stack<List<List<Decision>>>();
            cases.Push(d);
            while (cases.Count > 0)
            {
                // Perform splitting if necessary.
                var c = cases.Pop();
                if (MustSplit(c))
                {
                    var split = Split(c);
                    foreach (var s in split)
                        cases.Push(s);
                    continue;
                }

                DetermineCombOfTwoForCase(coeff1, coeff2, c, secNegated);
            }
        }

        public enum Decision
        {
            None = 0,
            First = 1,
            Second = 2,
            Both = 3,
        }

        private List<List<Decision>> GetDecisionVector(ApInt coeff1, ApInt coeff2, ApInt[] vec)
        {
            if (vec == null)
                vec = resultVector;

            var d = new List<List<Decision>>();
            foreach (var r in vec)
            {
                var e = new List<Decision>();
                var f = (moduloMask & (r - coeff1)) == 0;
                var s = (moduloMask & (r - coeff2)) == 0;
                var b = (moduloMask & (r - coeff1 - coeff2)) == 0;
                // For more variables, this would take too long.
                if (r == 0 && variables.Count > 4)
                    b = false;
                // Same.
                if (f && s && variables.Count > 4)
                    s = false;

                if ((moduloMask & r) == 0)
                    e.Add(Decision.None);
                if (b)
                    e.Add(Decision.Both);
                if (f)
                    e.Add(Decision.First);
                if (s)
                    e.Add(Decision.Second);

                Debug.Assert(e.Count > 0);
                d.Add(e);
            }

            return d;
        }

        private bool MustSplit(List<List<Decision>> d)
        {
            foreach (var e in d)
            {
                Debug.Assert(e.Count > 0);
                if (e.Count > 1)
                    return true;
            }

            return false;
        }

        private List<List<List<Decision>>> Split(List<List<Decision>> d)
        {
            var sec = new List<List<Decision>>();
            bool split = false;

            foreach (var e in d)
            {
                Debug.Assert(e.Count > 0);
                // Splitting has already happened.
                if (split)
                {
                    sec.Add(e.ToList());
                    continue;
                }

                // Split at this entry.
                if (e.Count > 1)
                {
                    split = true;
                    var popped = e[e.Count - 1];
                    e.RemoveAt(e.Count - 1);

                    sec.Add(new() { popped });
                    continue;
                }

                sec.Add(e.ToList());
            }

            Debug.Assert(split);
            return new List<List<List<Decision>>>() { d, sec };
        }

        private void DetermineCombOfTwoForCase(ApInt coeff1, ApInt coeff2, List<List<Decision>> _case, bool secNegated)
        {
            // Flatten the list structure. 
            List<Decision> cases = _case.Select(x => x.Single()).ToList();

            // Compute a boolean function for the first term.
            var l = cases.Select(x => x == Decision.First || x == Decision.Both ? 1 : 0).ToList();
            var first = BooleanSimplifier.GetBitwise(variables, l);

            // Compute a boolean function for the second term.
            l = cases.Select(x => x == Decision.Second || x == Decision.Both ? 1 : 0).ToList();
            var second = BooleanSimplifier.GetBitwise(variables, l, secNegated);

            // Compose the terms together, optionally negating the second coefficient if requested.
            var secondCoeff = secNegated ? NegateCoefficient(coeff2) : coeff2;
            var e = Compose(new() { first, second }, new() { coeff1, secondCoeff });

            CheckSolutionComplexity(e, 2);
        }

        private AstNode Compose(List<AstNode> bitwises, List<ApInt> coeffs)
        {
            Debug.Assert(bitwises.Count > 0);
            Debug.Assert(bitwises.Count == coeffs.Count);

            AstNode comb = null;
            for (int i = 0; i < bitwises.Count; i++)
            {
                var coeff = new ConstNode(coeffs[i], bitSize);
                var term = new MulNode(coeff, bitwises[i]);
                if (comb == null)
                    comb = term;
                else
                    comb = new AddNode(comb, term);
            }

            return comb;
        }

        private void TryRefineTwoTermsFirstNonZero(HashSet<ApInt> resultSet)
        {
            Debug.Assert(resultVector[0] != 0);
            var l = resultSet.Count;

            // Clone the result vec since we mutate it.
            var resultVec = resultVector.ToArray();

            if (l == 2)
            {
                // (6) In this case we know that the constant is nonzero since we 
                // would have run into the case (2) otherwise.
                var constant = ReduceByConstant();
                var e = ExpressionForEachUniqueValue(resultVector.ToHashSet());
                e = AddConstant(e, new ConstNode(constant, bitSize));
                CheckSolutionComplexity(e, 2);

                // Restore the result vector since it has been modified above.
                // TODO: Since we only subtracted a single coefficient, we can shave off
                // via adding the constant back to all of the entries instead of cloning.
                resultVector = resultVec;
            }

            if (l <= 4)
            {
                // (7) Try to express the expression as a linear combination of a 
                // negated and an unnegated bitwise expression.
                TryFindNegatedAndUnnegatedExpression();

                // (8) Try to express the expression as the linear combination of two negated
                // bitwise expressions.
                TryFindTwoNegatedExpressions();
            }
        }

        // For the stored list of results of the input expression for combinations
        // of truth values, try to find a negated and an unnegated expression
        // representing the result vector.
        void TryFindNegatedAndUnnegatedExpression()
        {
            // TODO: We can still try to find a solution with 2 terms if we already
            // have one with one terms, and then compare complexities.
            if (!HasOnlyThreeOrFourEntries(resultVector))
                return;

            var negCoeff = resultVector[0];
            Debug.Assert(negCoeff != 0);
            var vec = resultVector.Select(x => moduloMask & (x - negCoeff)).ToArray();
            Debug.Assert(vec[0] == 0);

            var uniqueValues = vec.ToHashSet().Where(x => x != 0 && x != negCoeff).ToArray();
            Debug.Assert(uniqueValues.Length >= 1);

            // Not possible to find a combination if we still have more than two
            // unique values.
            if (uniqueValues.Length > 2)
                return;

            if (uniqueValues.Length == 2)
            {
                var a = uniqueValues[0];
                var b = uniqueValues[1];

                // Try to express one of these values as the sum of the negated bitwise
                // expression's coefficient and the other value.
                if ((moduloMask & (b - a - negCoeff)) != 0)
                {
                    (a, b) = (b, a);
                    if ((moduloMask & (b - a - negCoeff)) != 0)
                        return;
                }

                var unnegCoeff = a;
                DetermineCombOfTwo(unnegCoeff, negCoeff, vec, true);
                return;
            }

            var _a = uniqueValues[0];
            // If we have just one unique value(a) left, we have on degree of freedom
            // to choose the unnegated bitwise expression's coefficient:
            // It can be either be 'a' or 'a' reduced by the negated expression's coefficient.
            DetermineCombOfTwo(_a, negCoeff, vec, true);
            DetermineCombOfTwo(moduloMask & (_a - negCoeff), negCoeff, vec, true);
        }

        private void TryFindTwoNegatedExpressions()
        {
            if (!HasOnlyThreeOrFourEntries(resultVector))
                return;

            var coeffSum = resultVector[0];
            Debug.Assert(coeffSum != 0);

            var vec = resultVector.Select(x => moduloMask & (x - coeffSum)).ToList();
            Debug.Assert(vec[0] == 0);

            var uniqueValues = vec.ToHashSet().Where(x => x != 0 && x != coeffSum).ToList();
            Debug.Assert(uniqueValues.Count >= 1);

            // Not possible to find a combination if we still have more than two
            // unique values.
            if (uniqueValues.Count > 2)
                return;

            // This case has already been handled in
            // try_find_negated_and_unnegated_expression().
            // TODO: Handle here too and compare complexity?
            if (uniqueValues.Count == 1)
                return;

            var a = uniqueValues[0];
            var b = uniqueValues[1];

            // Try to express one of these values as the difference of coeffSum and
            // the other value.
            if ((moduloMask & (b + a - coeffSum)) != 0)
                return;

            var coeff1 = a;
            var l = vec.Select(x => x == coeff1 || x == coeffSum ? 1 : 0).ToList();
            var bitwise1 = BooleanSimplifier.GetBitwise(variables, l, negate: true);

            var coeff2 = b;
            vec = RangeUtil.Get(vec.Count).Select(x => moduloMask & (vec[x] - coeff1 * (ApInt)l[x])).ToList();
            l = vec.Select(x => x == coeff2 ? 1 : 0).ToList();
            var bitwise2 = BooleanSimplifier.GetBitwise(variables, l, negate: true);

            var e = Compose(new() { bitwise1, bitwise2 }, new() { NegateCoefficient(coeff1), NegateCoefficient(coeff2) });
            CheckSolutionComplexity(e, 2);
        }

        private static bool HasOnlyThreeOrFourEntries(ApInt[] resultVector)
        {
            // TODO: We can still try to find a solution with 2 terms if we already
            // have one with one terms, and then compare complexities.
            var uv = resultVector.ToHashSet();
            if (uv.Count != 3 && uv.Count != 4)
                return false;

            return true;
        }

        // Returns true iff a is double b in the modular field
        private bool IsSumModulo(ApInt s1, ApInt s2, ApInt a)
        {
            return (moduloMask & (s1 + s2)) == a;
        }

        // Returns true iff a is double b in the modular field
        private bool IsDoubleModulo(ApInt a, ApInt b)
        {
            var mul = moduloMask & (2 * b);
            return mul == a;
        }

        // Build an expression by summing up bitwise expressions, each corresponding to a value in the lookup table
        // and multiplied by this value thereafter.
        private AstNode ExpressionForEachUniqueValue(HashSet<ApInt> resultSet)
        {
            AstNode expr = null;
            foreach (var r in resultSet)
            {
                // Skip zero entries.
                if (r == 0)
                    continue;

                var term = TermRefinement(r);
                if (expr == null)
                    expr = term;
                else
                    expr = new AddNode(expr, term);
            }

            return expr;
        }

        // Returns a simple bitwise expression corresponding to the positions where
        // the vector of results for truth value combinations has a value of r1 or
        // rAlt to the given expression.
        private AstNode TermRefinement(ApInt r1, ApInt? rAlt = null)
        {
            var t = new List<int>();
            foreach (var r2 in resultVector)
            {
                var cond = r2 == r1 || rAlt != null && r2 == rAlt.Value ? 1 : 0;
                t.Add(cond);
            }

            // Return m1 * bitop.
            var bitwise = BooleanSimplifier.GetBitwise(variables, t);
            return Term(bitwise, r1);
        }

        private void TryEliminateUniqueValue(List<ApInt> uniqueValues, ConstNode constant = null)
        {
            // Would be possible also for higher l, implementation is generic.
            var l = uniqueValues.Count;
            if (l > 4)
                return;

            // Try to get rid of a value by representing it as a sum of the others.
            foreach (var i in RangeUtil.Get(l - 1))
            {
                foreach (var j in RangeUtil.Get(i + 1, l))
                {
                    foreach (var k in RangeUtil.Get(l))
                    {
                        if (k == i || k == j)
                            continue;

                        // Skip if these terms do not sum up to each other.
                        bool isSum = IsSumModulo(uniqueValues[i], uniqueValues[j], uniqueValues[k]);
                        if (!isSum)
                            continue;

                        // We have a match.
                        AstNode simpler = null;
                        foreach (var i1 in new int[] { i, j })
                        {
                            var term = TermRefinement(uniqueValues[i1], uniqueValues[k]);
                            if (simpler == null)
                                simpler = term;
                            else
                                simpler = new AddNode(simpler, term);
                        }

                        if (l > 3)
                        {
                            var resultSet = uniqueValues.ToHashSet();
                            resultSet.Remove(uniqueValues[i]);
                            resultSet.Remove(uniqueValues[j]);
                            resultSet.Remove(uniqueValues[k]);

                            while (resultSet.Count > 0)
                            {
                                var r1 = resultSet.First();
                                resultSet.Remove(r1);

                                var term = TermRefinement(r1);
                                if (simpler == null)
                                    simpler = term;
                                else
                                    simpler = new AddNode(simpler, term);
                            }
                        }

                        CheckSolutionComplexity(simpler, l - 1, constant);
                    }
                }
            }

            if (l < 4)
                return;

            // Compute the sum of all unique entries.
            ApInt sum = 0;
            foreach (var entry in uniqueValues)
                sum += entry;
            sum &= moduloMask;

            // Finally, if we have more than 3 values, try to express one of them as
            // a sum of all others.
            foreach (var i in RangeUtil.Get(l))
            {
                // Skip if this entry cannot be expressed as the sum of all others.
                bool isSum = 2 * uniqueValues[i] == sum;
                if (!isSum)
                    continue;

                AstNode simpler = null;
                foreach (var j in RangeUtil.Get(l))
                {
                    if (i == j)
                        continue;

                    var term = TermRefinement(uniqueValues[j], uniqueValues[i]);
                    if (simpler == null)
                        simpler = term;
                    else
                        simpler = new AddNode(simpler, term);
                }

                CheckSolutionComplexity(simpler, l - 1, constant);
            }
        }

        private ApInt ReduceByConstant()
        {
            var constant = resultVector[0];
            if (constant == 0)
                return constant;

            foreach (var i in RangeUtil.Get(resultVector.Length))
            {
                resultVector[i] -= constant;
                resultVector[i] = moduloMask & (resultVector[i]);
            }

            return constant;
        }

        private AstNode ConjunctionFromVarMask(ApInt coeff, ulong varMask, ApInt? bitMask)
            => ConjunctionFromVarMask(coeff, varMask, variables, bitMask);

        public static AstNode ConjunctionFromVarMask(ApInt coeff, ulong varMask, IReadOnlyList<VarNode> variables, ApInt? bitMask)
        {
            Debug.Assert(variables.Count > 0);
            if (coeff == 0)
                return null;

            AstNode conj = null;
            while (varMask != 0)
            {
                var lsb = BitOperations.TrailingZeroCount(varMask);
                var op = variables[lsb];
                if (conj == null)
                    conj = op;
                else
                    conj = new AndNode(conj, op);

                varMask ^= (1ul << (ushort)lsb);
            }

            if (bitMask != null)
                conj = new AndNode(new ConstNode(bitMask.Value, variables[0].BitSize), conj);
            return Term(conj, coeff, conj.BitSize);
        }

        private AstNode Conjunction(ApInt coeff, IReadOnlyList<int> variables, ApInt? mask = null)
        {
            Debug.Assert(variables.Count > 0);
            if (coeff == 0)
                return null;

            AstNode conj = null;
            foreach (var v in variables)
            {
                var op = this.variables[v];
                if (conj == null)
                    conj = op;
                else
                    conj = new AndNode(conj, op);
            }

            if (mask != null)
                conj = new AndNode(new ConstNode(mask.Value, bitSize), conj);
            return Term(conj, coeff);
        }

        private AstNode Conjunction(ApInt coeff, AstNode conj, ApInt? mask = null)
        {
            Debug.Assert(variables.Count > 0);
            if (coeff == 0)
                return null;

            if (mask != null)
                conj = new AndNode(new ConstNode(mask.Value, bitSize), conj);
            return Term(conj, coeff);
        }

        private AstNode Term(AstNode bitwise, ApInt coeff)
            => Term(bitwise, coeff, bitSize);

        private static AstNode Term(AstNode bitwise, ApInt coeff, uint bitSize)
        {
            if (coeff == 1)
                return bitwise;

            var coeffAst = new ConstNode(coeff, bitSize);
            return new MulNode(coeffAst, bitwise);
        }


        private bool CheckTermCount(int value)
        {
            if (lincombTerms.GetValueOrDefault() <= value)
                return false;
            if (metric != Metric.Terms)
                return false;

            var currTermCount = GetTermCountOfCurrentSolution();
            return currTermCount <= value;
        }

        private int? GetTermCountOfCurrentSolution()
        {
            var m = (int)Metric.Terms - (int)metric;
            Debug.Assert(m >= 0);

            Debug.Assert(__compl != null);
            Debug.Assert(__compl[m] != null);
            return __compl[m];
        }

        // Check whether the given solution is less complex than the currently
        // optimal known one. If so or if there is not yet any solution known, store
        // its computed complexity values. The term count is optionally given. If a
        // constant is given, this constant has to be added tot he expression and
        // the given term count has to be incremented by one accordingly.
        private void CheckSolutionComplexity(AstNode e, int? t = null, ConstNode constant = null)
        {
            Debug.Assert(res == null == (__compl == null));

            if (constant != null)
            {
                e = AddConstant(e, constant);
                if (t != null)
                    t = t + 1;
            }

            // No known solution yet
            var getCompl = () => Enumerable.Range((int)metric, (int)Metric.Count).Select(x => (int?)null).ToList();
            if (res == null)
            {
                res = e;
                __compl = getCompl();
                if (t != null && (int)metric <= (int)Metric.Terms)
                {
                    __compl[(int)Metric.Terms] = t;
                }

                return;
            }

            // TODO: Implement support for other metrics.
            var size = GetCost(e, false);
            var otherSize = GetCost(res, false);

            var min = (double)Math.Min(size, otherSize);
            var max = (double)Math.Max(size, otherSize);

            var tc1 = CountTerms(e);
            var tc2 = CountTerms(res);

            // Apply additional weight to the term count.
            // If two expressions have the same number of nodes,
            // we want the one with the least terms.
            size += tc1;
            otherSize += tc2;

            if (size < otherSize)
            {
                res = e;
                __compl[(int)Metric.Terms] = t;
            }
        }

        private void Log(string message)
        {
            if(dbg)
                Console.WriteLine(message);
        }

        private void Log(AstNode conj)
        {
            if (!dbg)
                return;

            if (newLine)
                Console.WriteLine(conj);
            else
                Console.Write(conj + " + ");
        }

        private static AstNode AddConstant(AstNode expr, ConstNode constant)
        {
            if (constant.Value == 0)
                return expr;

            return new AddNode(constant, expr);
        }

        // make a range over [start..end) , where end is not included (exclusive)
        public static IEnumerable<int> PyRange(int start, int end)
        {
            if (end <= start) return Enumerable.Empty<int>();
            // else
            return Enumerable.Range(start, end - start);
        }

    }
}
