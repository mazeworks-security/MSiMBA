using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Ast
{
    public enum Predicate : byte
    {
        Eq = 0,
        Ne = 1,
        Ugt = 2,
        Uge = 3,
        Ult = 4,
        Ule = 5,
        Sgt = 6,
        Sge = 7,
        Slt = 8,
        Sle = 9,
    }

    public class ICmpNode : AstNode
    {
        public Predicate Pred { get; set; }

        protected override int OpCount => 3;

        public ICmpNode(Predicate pred, AstNode op1, AstNode op2) : base(AstKind.ICmp, 1, op1, op2)
        {
            Pred = pred;
        }
    }
}
