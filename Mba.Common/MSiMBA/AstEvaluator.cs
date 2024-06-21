using Mba.Ast;
using Mba.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using static Antlr4.Runtime.Atn.SemanticContext;
using ApInt = System.UInt128;
namespace Mba.Common.MSiMBA
{
    public class AstEvaluator
    {
        private readonly ApInt clearMask;

        private readonly ApInt[] varNodeValues;

        public AstEvaluator(ApInt[] varNodeValues, int bitWidth)
        {
            this.varNodeValues = varNodeValues;
            clearMask = ModuloReducer.GetMask((uint)bitWidth);
        }

        public ApInt Eval(AstNode node)
        {
            ApInt value = 0;
            var ops = node.Children;
            switch(node.Kind)
            {
                case AstKind.Const:
                    value = (UInt128)(ulong)(node as ConstNode).Value;
                    break;
                case AstKind.Var:
                    value = varNodeValues[node.index];
                    break;
                case AstKind.Add:
                    value = Eval(ops[0]) + Eval(ops[1]);
                    break;
                case AstKind.Mul:
                    value = Eval(ops[0]) * Eval(ops[1]);
                    break;
                case AstKind.And:
                    value = Eval(ops[0]) & Eval(ops[1]);
                    break;
                case AstKind.Or:
                    value = Eval(ops[0]) | Eval(ops[1]);
                    break;
                case AstKind.Xor:
                    value = Eval(ops[0]) ^ Eval(ops[1]);
                    break;
                case AstKind.Neg:
                    value = ~Eval(ops[0]);
                    break;
                case AstKind.Lshr:
                    value = ~Eval(ops[0]) >> (ushort)Eval(ops[1]);
                    break;
            }

            return value & clearMask;
        }
    }
}
