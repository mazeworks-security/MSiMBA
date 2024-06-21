using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class WildCardConstantNode : AstNode
    {
        public string Name { get; }

        protected override int OpCount => 0;

        public WildCardConstantNode(string name, uint bitSize) : base(AstKind.WildCardConstant, bitSize, new AstNode[] { })
        {
            Name = name;
        }
    }
}
