using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public abstract class BinaryNode : AstNode
    {
        protected override int OpCount => 2;

        public BinaryNode(AstKind kind, IEnumerable<AstNode> nodes) : base(kind, nodes.First().BitSize, nodes.ToArray())
        {
        }

        public BinaryNode(AstKind kind, AstNode op1, AstNode op2) : base(kind, op1.BitSize, op1, op2)
        {
            if (op1.BitSize != op2.BitSize)
                throw new InvalidOperationException($"Operand size mismatch between {op1} and {op2}");
        }
    }
}
