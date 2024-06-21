using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class XorNode : BinaryNode
    {
        public XorNode(AstNode op1, AstNode op2) : base(AstKind.Xor, op1, op2) { }
    }
}
