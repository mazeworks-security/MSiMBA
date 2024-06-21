using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UserMapping = System.Collections.Generic.Dictionary<long, System.Collections.Generic.List<(Mba.Ast.AstNode user, int childIndex)>>;

namespace Mba.Common.MSiMBA
{
    public class ConstantSubstituter
    {
        private readonly AstNode ast;

        private UserMapping usersMapping = new();

        public static (AstNode astWithSubstitutions, Dictionary<long, VarNode> substitutionMapping)? Apply(AstNode ast, HashSet<string> existingVars)
            => new ConstantSubstituter(ast).Apply(existingVars);

        public static AstNode ApplyBackSubstitution(AstNode ast, Dictionary<VarNode, long> substitutionMapping)
        {
            var inverseMapping = new Dictionary<VarNode, AstNode>();
            foreach (var (varNode, constant) in substitutionMapping)
                inverseMapping.Add(varNode, new ConstNode(constant, ast.BitSize));

            return AstCloner.CloneAst(ast, inverseMapping, ast.BitSize);
        }

        private ConstantSubstituter(AstNode ast)
        {
            // Clone the AST since we're mutating it.
            this.ast = AstCloner.CloneAst(ast, new(), ast.BitSize);
        }

        private (AstNode astWithSubstitutions, Dictionary<long, VarNode> substitutionMapping)? Apply(HashSet<string> existingVars)
        {
            // Collect all bitwise constants.
            bool atLimit = false;
            Collect(ast, usersMapping, false, ref atLimit);
            if (atLimit)
                return null;

            // Try to minimize the number of constants and expression relationships between them.
            var unmerger = new FastConstantUnmerger(ast.BitSize, usersMapping.Keys.ToList());
            unmerger.Unmerge();

            // Update the ast to used the unmerged results.
            foreach (var (constant, users) in usersMapping)
            {
                var unmerged = unmerger.BuildAstForConstant(constant);
                foreach (var (user, childIndex) in users)
                {
                    user.Children[childIndex] = unmerged;
                }
            }

            // Update the use mapping.
            usersMapping.Clear();
            Collect(ast, usersMapping, false, ref atLimit);

            // Substitute all unique constants with temporary variables.
            var substMapping = new Dictionary<long, VarNode>();
            foreach (var (constant, users) in usersMapping)
            {
                // Throw if the input expression uses a variable with this name already.
                var name = $"um{constant.ToString()}";
                if (existingVars.Contains(name))
                    throw new InvalidOperationException($"Name conflict!");

                var subst = new VarNode(name, ast.BitSize);
                substMapping.Add(constant, subst);
                foreach (var (user, childIndex) in users)
                {
                    user.Children[childIndex] = subst;
                }
            }

            return (ast, substMapping);
        }

        public static void Collect(AstNode ast, UserMapping userMapping, bool inBitwise, ref bool atLimit)
        {
            if (atLimit)
                return;
            if(userMapping.Count >= 10)
            {
                atLimit = true;
                return;
            }

            var kind = ast.Kind;
            if (kind == AstKind.Or || kind == AstKind.And || kind == AstKind.Xor)
            {
                for (int i = 0; i < ast.Children.Count; i++)
                {
                    // We've found a constant inside of a bitwise operand.
                    var child = ast.Children[i];
                    if (child is ConstNode cn)
                        Add(userMapping, cn.Value, (ast, i));
                    // Otherwise we need to recurse into checking children.
                    else
                        Collect(child, userMapping, true, ref atLimit);
                }
            }

            // Negation can be treated as either an arithmetic or a bitwise function.
            else if (kind == AstKind.Neg && inBitwise)
            {
                var single = ast.Children[0];
                if (single is ConstNode constNode)
                    Add(userMapping, constNode.Value, (ast, 0));
                else
                    Collect(single, userMapping, inBitwise, ref atLimit);
            }

            else if (kind == AstKind.Var || kind == AstKind.Const)
            {
                return;
            }

            else
            {
                foreach (var child in ast.Children)
                    Collect(child, userMapping, inBitwise, ref atLimit);
            }
        }

        private static void Add(UserMapping userMapping, long constant, (AstNode ast, int childIndex) item)
        {
            if (userMapping.TryGetValue(constant, out var existing))
            {
                existing.Add(item);
                return;
            }

            userMapping.Add(constant, new() { item });
        }
    }
}
