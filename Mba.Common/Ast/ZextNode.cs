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
        public ZextNode(AstNode op1, uint bitwidth) : base(AstKind.Zext, bitwidth, op1)
        {
            
        }

        protected override int OpCount => 1;
    }
}
