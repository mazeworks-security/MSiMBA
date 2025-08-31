using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public abstract class UnaryNode : AstNode
    {
        protected override int OpCount => 1;

        public UnaryNode(AstKind kind, AstNode op1) : base(kind, op1.BitSize, op1)
        {

        }
    }
}
