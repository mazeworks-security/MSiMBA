using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    public class SelectNode : AstNode
    {
        public SelectNode(AstNode a, AstNode b, AstNode c) : base(AstKind.Select, b.BitSize, a, b, c)
        {
        }

        protected override int OpCount => 3;
    }
}
