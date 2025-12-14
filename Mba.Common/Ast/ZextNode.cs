using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    public class ZextNode : AstNode
    {
        public static ZextNode UnkWidth(AstNode a, AstNode b)
            => b is ConstNode cn ? new ZextNode(a, cn) : new ZextNode(a, b);

        public ZextNode(AstNode op1, ConstNode bitwidth) : base(AstKind.Zext, (uint)bitwidth.UValue, op1, bitwidth)
        {
            
        }

        public ZextNode(AstNode op1, uint bitWidth) : base(AstKind.Zext, bitWidth, op1, new ConstNode((ulong)bitWidth, 64))
        { 
            
        }

        private ZextNode(AstNode op1, AstNode op2) : base(AstKind.Zext, 64, op1, op2)
        {
            
        }

        protected override int OpCount => 2;
    }
}
