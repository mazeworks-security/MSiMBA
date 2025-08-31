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
        public TruncNode(AstNode op1, uint bitwidth) : base(AstKind.Trunc, bitwidth, op1)
        {

        }

        protected override int OpCount => 1;
    }
}
