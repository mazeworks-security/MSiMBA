using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public class PowerNode : BinaryNode
    {
        public PowerNode(AstNode op1, int power) : base(AstKind.Power, op1, new ConstNode(power, op1.BitSize))
        {
        }

        public PowerNode(AstNode op1, AstNode op2) : base(AstKind.Power, op1, op2)
        {

        }
    }
}
