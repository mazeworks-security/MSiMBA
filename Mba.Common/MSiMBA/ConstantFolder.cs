using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.MSiMBA
{
    public class ConstantFolder
    {
        private readonly uint bitSize;

        private readonly long mask;

        public ConstantFolder(uint bitSize, long mask)
        {
            this.bitSize = bitSize;
            this.mask = mask;
        }

        // Try to find subtrees that reduce to a constant.
        public AstNode TryFold(AstNode ast)
        {
            // Skip if there are no trees to fold.
            if (ast.Children.Count == 0)
                return ast;
            // If one child is a constant, move it to the left.
            if (ast.Children.Count == 2 && ast.Children[1].Kind == AstKind.Const && ast.Children[0].Kind != AstKind.Const)
                return TryFold(Swap(ast));
            // If both children are constant, fold it.
            if (ast.Children.Count == 2 && ast.Children[0] is ConstNode c0 && ast.Children[1] is ConstNode c1)
                return FoldBinop(ast.Kind, c0.Value, c1.Value);
            // If this is a negation of a constant, fold it.
            if (ast.Kind == AstKind.Neg && ast.Children[0] is ConstNode negatedConst)
                return new ConstNode(mask & ~negatedConst.Value, bitSize);
            // (c1 + (c2 + x)) => (c1+c2) + x
            if (ast.Children.Count == 2 && ast.Children[0] is ConstNode firstConst)
            {
                var otherChild = ast.Children[1];
                if (otherChild.Kind == ast.Kind && otherChild.Children[0] is ConstNode secondConst)
                {
                    var cprop = FoldBinop(ast.Kind, firstConst.Value, secondConst.Value);
                    var result = AstNode.Binop(ast.Kind, cprop, otherChild.Children[1]);
                    return TryFold(result);
                }
            }

            // Otherwise recursively simplify all children.
            for (int i = 0; i < ast.Children.Count; i++)
            {
                var child = ast.Children[i];
                var folded = TryFold(child);
                ast.Children[i] = folded;
            }

            return ast;
        }

        private static AstNode Swap(AstNode ast)
        {
            var old = ast.Children[0];
            ast.Children[0] = ast.Children[1];
            ast.Children[1] = old;
            return ast;
        }

        private ConstNode FoldBinop(AstKind kind, long op0, long op1)
        {
            var value = kind switch
            {
                AstKind.Add => mask & op0 + op1,
                AstKind.Mul => mask & op0 * op1,
                AstKind.And => mask & op0 & op1,
                AstKind.Or => mask & (op0 | op1),
                AstKind.Xor => mask & (op0 ^ op1),
                _ => throw new InvalidOperationException($"Unrecognized binop: {kind}")
            };

            return new ConstNode(value, bitSize);
        }
    }
}
