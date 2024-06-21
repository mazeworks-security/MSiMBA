using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class VarNode : AstNode
    {
        protected override int OpCount => 0;

        public string Name { get; }

        public VarNode(string name, uint bitSize) : base(AstKind.Var, bitSize)
        {
            Name = name;
        }

        public override int GetHashCode()
        {
            return Name.GetHashCode();
        }

        public override bool Equals(object? obj)
        {
            if (obj is not VarNode varNode)
                return false;

            return varNode.Name == Name && varNode.BitSize == BitSize;
        }
    }
}
