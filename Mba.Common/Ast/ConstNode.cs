using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class ConstNode : AstNode
    {
        protected override int OpCount => 0;

        public long Value { get; }

        public ConstNode(ulong value, uint bitSize) : base(AstKind.Const, bitSize)
        {
            Value = (long)value;
        }

        public ConstNode(long value, uint bitSize) : base(AstKind.Const, bitSize)
        {
            Value = (long)value;
        }

        public ConstNode(UInt128 value, uint bitSize) : this((ulong)value, bitSize)
        {

        }
    }
}
