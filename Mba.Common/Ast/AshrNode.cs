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
    public class AshrNode : BinaryNode
    {
        public AshrNode(AstNode op1, int shiftCount) : base(AstKind.Ashr, op1, new ConstNode(shiftCount, op1.BitSize))
        {
        }

        public AshrNode(AstNode op1, AstNode op2) : base(AstKind.Ashr, op1, op2)
        {

        }
    }
}
