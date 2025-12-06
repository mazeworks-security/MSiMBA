using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    public class IntrinsicCallNode : AstNode
    {
        public string Name { get; }
        protected override int OpCount => Children.Count;

        public IntrinsicCallNode(string name, uint bitSize, List<AstNode> arguments) : base(AstKind.IntrinsicCall, bitSize, arguments.ToArray())
        {
            Name = name;
        }

    }
}
