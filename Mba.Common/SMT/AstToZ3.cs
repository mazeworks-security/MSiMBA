using Mba.Ast;
using Microsoft.Z3;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Mba.SMT
{
    public class AstToZ3
    {
        private readonly uint bitWidth;

        public Context ctx;

        public AstToZ3(uint bitWidth, Context ctx = null)
        {
            this.ctx = ctx;
            if (this.ctx == null)
                this.ctx = new();
            this.bitWidth = bitWidth;
        }

        public Expr Translate(AstNode expression, AstNode? parent = null)
        {
            // Concise bit vector AST getter methods.
            var bv1 = () => (BitVecExpr)Translate(expression.Children[0], expression);
            var bv2 = () => (BitVecExpr)Translate(expression.Children[1], expression);

            Expr z3Ast = expression switch
            {
                VarNode varNode => ctx.MkBVConst(varNode.Name, bitWidth),
                ConstNode constNode => IsBitwise(parent) ? ctx.MkBVConst($"replaced_constant_{constNode.Value}", bitWidth) : ctx.MkBV(constNode.Value, bitWidth),
                AddNode => ctx.MkBVAdd(bv1(), bv2()),
                MulNode => ctx.MkBVMul(bv1(), bv2()),
                AndNode => ctx.MkBVAND(bv1(), bv2()),
                OrNode => ctx.MkBVOR(bv1(), bv2()),
                XorNode => ctx.MkBVXOR(bv1(), bv2()),
                NegNode => ctx.MkBVNot(bv1()),
                PowerNode => Power(expression),
                LshrNode => ctx.MkBVLSHR(bv1(), bv2()),
                ShlNode => ctx.MkBVSHL(bv1(), bv2()),
                _ => throw new InvalidOperationException()
            };

            return z3Ast;
        }

        // Optionally can be enabled for substitution of bitwise constants.
        private static bool IsBitwise(AstNode expression)
        {
            return false;

            if (expression == null)
                return false;

            var kind = expression.Kind;
            if (kind == AstKind.And || kind == AstKind.Or || kind == AstKind.Xor)
                return true;

            return false;
        }

        private Expr Power(AstNode expression)
        {
            var lhs = expression.Children[0];
            var rhs = expression.Children[1];
            if (rhs is not ConstNode constNode)
                throw new InvalidOperationException();

            return Power(Translate(lhs), constNode.Value, bitWidth);
        }

        public Expr Power(Expr x, long y, uint bitWidth)
        {
            if (y == 0)
                return ctx.MkBV(1, bitWidth);
            if (y == 1)
                return x;

            var originalBv = x;
            for (long i = 0; i < y - 1; i++)
            {
                // x = x * original;
                x = ctx.MkBVMul((BitVecExpr)x, (BitVecExpr)originalBv);
            }

            return x;
        }
    }
}
