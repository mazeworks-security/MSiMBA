using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    public class ConditionalOrNode : AstNode
    {
        protected override int OpCount => 2;

        public ConditionalOrNode(AstNode op1, AstNode op2) : base(AstKind.ConditionalOr, 1, op1, op2)
        {
        }
    }
}
