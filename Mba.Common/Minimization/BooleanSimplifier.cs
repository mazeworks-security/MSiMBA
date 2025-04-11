using Mba.Ast;
using Mba.Common.MSiMBA;
using Mba.Testing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Minimization
{
    public static class BooleanSimplifier
    {
        public static AstNode GetBitwise(IReadOnlyList<VarNode> variables, List<int> resultVector, bool negate = false)
        {
            // If requested, negate the result vector to find a negated expression.
            if (negate)
                resultVector = resultVector.Select(x => ~x & 1).ToList();

            // Exit early if the boolean function is a constant.
            var asConstant = AsConstant(resultVector, variables[0].BitSize);
            if (asConstant != null)
                return asConstant;

            // If there are four or less variables, we can pull the optimal representation from the truth table.
            // TODO: One could possibly construct a 5 variable truth table for all 5 variable NPN classes.
            if (variables.Count <= 4 && variables.Count > 1)
            {
                return FromTruthTable(variables, resultVector);
            }

            // Otherwise use Espresso to compute a semi optimal version of the boolean function.
            //var xnf = SimbaMinimizer.SimplifyBoolean(variables, resultVector);
            var dnf = EspressoMinimizer.SimplifyBoolean(resultVector, variables).ast;

            //var c1 = MultibitSiMBA.GetCost(xnf, false, 1);
            var c2 = MultibitSiMBA.GetCost(dnf, false, 1);
            //if (c1 < c2)
            //    return xnf;
            return dnf;
        }

        private static AstNode AsConstant(List<int> resultVector, uint bitWidth)
        {
            var first = resultVector[0];
            for(int i = 1; i < resultVector.Count; i++)
            {
                if (resultVector[i] != first)
                    return null;
            }

            return new ConstNode(first, bitWidth);
        }

        private static AstNode FromTruthTable(IReadOnlyList<VarNode> variables, List<int> resultVector)
        {
            // Convert the result vector to an index into the N variable truth table.
            var tableIdx = ResultVecToTableIdx(resultVector);

            // Fetch the truth table entry corresponding to this node.
            var ast = TruthTableDatabase.Instance.GetTableEntry(variables, (int)tableIdx);

            // Rewrite the AST to use our variables.
            var rewritten = RewriteUsingNewVariables(ast, (v) => variables[v.index]);
            return rewritten;
        }

        public static AstNode RewriteUsingNewVariables(AstNode ast, Func<VarNode, VarNode> getVar)
        {
            var op1 = () => RewriteUsingNewVariables(ast.Children[0], getVar);
            var op2 = () => RewriteUsingNewVariables(ast.Children[1], getVar);
            return ast switch
            {
                ConstNode constNode => new ConstNode(constNode.Value, ast.BitSize),
                VarNode varNode => getVar(varNode),
                PowerNode powerNode => new PowerNode(op1(), op2()),
                AddNode => new AddNode(op1(), op2()),
                MulNode mulNode => new MulNode(op1(), op2()),
                AndNode andNode => new AndNode(op1(), op2()),
                OrNode orNode => new OrNode(op1(), op2()),
                XorNode => new XorNode(op1(), op2()),
                NegNode => new NegNode(op1()),
            };
        }

        public static ulong ResultVecToTableIdx(List<int> resultVector)
        {
            // Represent the result vector as a 64 bit integer,
            // where each bit represents whether the result vector entry at that idx is non-zero.
            ulong table = 0;
            for (int i = 0; i < resultVector.Count; i++)
            {
                ulong cond = resultVector[i] != 0 ? 1u : 0;
                ulong value = cond << (ushort)i;
                table |= value;
            }

            return table;
        }
    }
}
