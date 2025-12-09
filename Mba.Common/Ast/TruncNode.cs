using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    // Sign extension node
    public class TruncNode : AstNode
    {
        public static TruncNode UnkWidth(AstNode a, AstNode b)
            => b is ConstNode cn ? new TruncNode(a, cn) : new TruncNode(a, b);

        public TruncNode(AstNode op1, ConstNode bitwidth) : base(AstKind.Trunc, (uint)bitwidth.UValue, op1)
        {

        }


        public TruncNode(AstNode op1, uint bitWidth) : base(AstKind.Trunc, bitWidth, op1, new ConstNode((ulong)bitWidth, 8))
        {

        }


        private TruncNode(AstNode op1, AstNode op2) : base(AstKind.Trunc, 64, op1, op2)
        {

        }

        protected override int OpCount => 2;
    }
}
