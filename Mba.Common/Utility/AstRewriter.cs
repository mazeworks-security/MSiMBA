using Mba.Ast;
using Mba.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Utility
{
    public static class AstRewriter
    {
        public static AstNode ChangeBitwidth(AstNode node, uint newWidth)
        {
            var opcode = node.Kind;
            var binop = () => AstNode.Binop(opcode, ChangeBitwidth(node.Children[0], newWidth), ChangeBitwidth(node.Children[1], newWidth));

            return opcode switch
            {
                AstKind.Const => new ConstNode(ModuloReducer.ReduceToModulo((ulong)(node as ConstNode).Value, newWidth), newWidth),
                AstKind.Var => new VarNode((node as VarNode).Name, newWidth),
                AstKind.Add => binop(),
                AstKind.Mul => binop(),
                AstKind.And => binop(),
                AstKind.Or => binop(),
                AstKind.Xor => binop(),
                AstKind.Neg => new NegNode(ChangeBitwidth(node.Children[0], newWidth)),
                _ => throw new InvalidOperationException($"Cannot change width of opcode {opcode}"),
            };
        }
    }
}
