using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    /// <summary>
    /// Class for logical right shift nodes.
    /// </summary>
    public class ShlNode : BinaryNode
    {
        public ShlNode(AstNode op1, int shiftCount) : base(AstKind.Shl, op1, new ConstNode(shiftCount, op1.BitSize))
        {
        }

        public ShlNode(AstNode op1, AstNode op2) : base(AstKind.Shl, op1, op2)
        {

        }
    }
}
