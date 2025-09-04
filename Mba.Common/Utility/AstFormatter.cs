﻿using System;
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

            if (node is BinaryNode)
            {
                sb.Append("(");

                for(int i = 0; i < node.Children.Count; i++)
                {
                    
                    FormatAstInternal(node.Children[i], ref sb);
                    if (i != node.Children.Count - 1)
                        sb.Append(GetOperatorName(node.Kind));
                }

                sb.Append(")");
                return;
            }

            if (node is UnaryNode)
            {
                sb.Append("(");
                sb.Append($"{GetOperatorName(node.Kind)}");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append(")");
                return;
            }

            if (node is ZextNode || node is SextNode || node is TruncNode)
            {
                sb.Append("(");
                FormatAstInternal(node.Children[0], ref sb);
                sb.Append($" {GetOperatorName(node.Kind)} i{node.BitSize})");
                return;
            }

            throw new InvalidOperationException($"Cannot print ast kind: {node.Kind}");
        }

        public static string GetOperatorName(AstKind kind)
        {
            return kind switch
            {
                AstKind.Const => "",
                AstKind.Power => "**",
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
                _ => throw new InvalidOperationException($"Unrecognized operator: {kind.ToString()}")
            };
        }
    }
}
