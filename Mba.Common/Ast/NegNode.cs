using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class NegNode : UnaryNode
    {
        public NegNode(AstNode op1) : base(AstKind.Neg, op1)
        {

        }
    }
}
