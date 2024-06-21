using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class RefNode : UnaryNode
    {
        public RefNode(AstNode op1) : base(AstKind.Ref, op1)
        {

        }
    }
}
