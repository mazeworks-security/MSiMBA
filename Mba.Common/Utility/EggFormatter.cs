using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Utility
{
    public static class EggFormatter
    {
        // Format the input AST into an s-expr representation.
        public static string FormatAst(AstNode node)
        {
            var sb = new StringBuilder();
            FormatAstInternal(node, ref sb);
            return sb.ToString();
        }

        private static void FormatAstInternal(AstNode node, ref StringBuilder sb)
        {
            if (node is ConstNode constNode)
            {
                sb.Append(constNode.Value);
                return;
            }

            if (node is VarNode varNode)
            {
                sb.Append(varNode.Name);
                return;
            }

            if (node is BinaryNode)
            {
                sb.Append("(");

                sb.Append(GetOperatorName(node.Kind));
                sb.Append(" ");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append(" ");
                FormatAstInternal(node.Children[1], ref sb);
                sb.Append(")");

                if (node.Children.Count != 2)
                    throw new InvalidOperationException("Flattened asts are not supported.");
                return;
            }

            if (node is UnaryNode)
            {
                sb.Append("(");
                sb.Append($"{GetOperatorName(node.Kind)}");
                sb.Append(" ");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append(")");
                return;
            }

            throw new InvalidOperationException($"Cannot print ast kind: {node.Kind}");
        }

        public static string GetOperatorName(AstKind kind)
        {
            return kind switch
            {
                AstKind.Power => "**",
                AstKind.Add => "+",
                AstKind.Mul => "*",
                AstKind.And => "&",
                AstKind.Or => "|",
                AstKind.Xor => "^",
                AstKind.Neg => "~",
                AstKind.Shl => "<<",
                AstKind.Lshr => ">>",
                _ => throw new InvalidOperationException($"Unrecognized operator: {kind.ToString()}")
            };
        }
    }
}
