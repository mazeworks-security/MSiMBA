using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    // Sign extension node
    public class SextNode : AstNode
    {
        public SextNode(AstNode src, uint bitwidth) : base(AstKind.Sext, bitwidth)
        {

        }

        protected override int OpCount => 1;
    }
}
