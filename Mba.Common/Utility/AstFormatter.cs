using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mba.Ast;
using Mba.Common.Ast;
using Microsoft.Z3;

namespace Mba.Utility
{
    public static class AstFormatter
    {
        // Temporary hack to generate datasets that are suitable for ProMBA(https://github.com/astean1001/ProMBA/)'s benchmarking script.
        public static bool InCilForm = false;

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
                var cStr = !InCilForm ? constNode.Value.ToString() : $"0x{constNode.Value.ToString("X")}UL";
                sb.Append($"{cStr}:i{node.BitSize}");
                return;
            }

            if (node is VarNode varNode)
            {
                sb.Append($"{varNode.Name}:i{varNode.BitSize}");
                return;
            }

            if (node is WildCardConstantNode wcNode)
            {
                sb.Append($"Const({wcNode.Name})");
                return;
            }

            if (node is BinaryNode || node is ICmpNode cmpNode || node is ConditionalAndNode || node is ConditionalOrNode)
            {
                sb.Append("(");

                for(int i = 0; i < node.Children.Count; i++)
                {
                    
                    FormatAstInternal(node.Children[i], ref sb);
                    if (i != node.Children.Count - 1)
                        sb.Append(GetOperatorName(node));
                }

                sb.Append(")");
                return;
            }

            if (node is UnaryNode)
            {
                sb.Append("(");
                sb.Append($"{GetOperatorName(node)}");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append(")");
                return;
            }

            if (node is ZextNode || node is SextNode || node is TruncNode)
            {
                sb.Append("(");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append($" {GetOperatorName(node)} i{node.BitSize})");
                return;
            }

            if (node is SelectNode)
            {
                sb.Append("(");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append(" ? ");
                FormatAstInternal(node.Children[1], ref sb);
                sb.Append(" : ");
                FormatAstInternal(node.Children[2], ref sb);
                sb.Append(")");
                return;
            }

            if (node is IntrinsicCallNode)
            {
                sb.Append("(");
                sb.Append($"{GetOperatorName(node)}(");
                for(int i = 0; i < node.Children.Count; i++)
                {
                    FormatAstInternal(node.Children[i], ref sb);
                    if (i != node.Children.Count - 1)
                        sb.Append(", ");
                }
                sb.Append("))");
                return;
            }


            throw new InvalidOperationException($"Cannot print ast kind: {node.Kind}");
        }

        public static string GetOperatorName(AstNode node)
        {
            return node.Kind switch
            {
                AstKind.Const => "",
                AstKind.Pow => "**",
                AstKind.Add => "+",
                AstKind.Mul => "*",
                AstKind.And => "&",
                AstKind.Or => "|",
                AstKind.Xor => "^",
                AstKind.Neg => "~",
                AstKind.Shl => "<<",
                AstKind.Lshr => ">>",
                AstKind.Ashr => ">>>",
                AstKind.Zext => "zx",
                AstKind.Sext => "sx",
                AstKind.Trunc => "tr",
                AstKind.ICmp => GetPredicateOperator((node as ICmpNode).Pred),
                AstKind.IntrinsicCall => (node as IntrinsicCallNode).Name,
                AstKind.ConditionalAnd => "&&",
                AstKind.ConditionalOr => "||",
                _ => throw new InvalidOperationException($"Unrecognized operator: {node.Kind.ToString()}")
            };
        }

        public static string GetPredicateOperator(Predicate pred)
        {
            return pred switch
            {
                Predicate.Eq => "==",
                Predicate.Ne => "!=",
                Predicate.Ugt => ">",
                Predicate.Uge => ">=",
                Predicate.Ult => "<",
                Predicate.Ule => "<=",
                Predicate.Sgt => ">s",
                Predicate.Sge => ">=s",
                Predicate.Slt => "<s",
                Predicate.Sle => "<=s",
                _ => throw new InvalidOperationException($"Unrecognized predicate: {pred}")
            };
        }
    }
}
