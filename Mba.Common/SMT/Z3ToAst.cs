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
    public class Z3ToAst
    {
        public AstNode FromZ3Expr(Expr expression)
        {
            if (expression.ASTKind == Z3_ast_kind.Z3_NUMERAL_AST)
            {
                var bvConst = (BitVecNum)expression;
                return new ConstNode(bvConst.UInt64, bvConst.SortSize);
            }

            if (expression.ASTKind != Z3_ast_kind.Z3_APP_AST)
                throw new InvalidOperationException();

            var declKind = expression.FuncDecl.DeclKind;
            AstNode ast = declKind switch
            {
                Z3_decl_kind.Z3_OP_BADD => FromAdd(expression),
                Z3_decl_kind.Z3_OP_BMUL => FromMul(expression),
                Z3_decl_kind.Z3_OP_BXOR => FromXor(expression),
                Z3_decl_kind.Z3_OP_BOR => FromOr(expression),
                Z3_decl_kind.Z3_OP_BNOT => FromNot(expression),
                Z3_decl_kind.Z3_OP_UNINTERPRETED => FromUninterpreted(expression),
                _ => throw new InvalidOperationException($"Cannot translate DeclKind {declKind} to triton ast node type.")
            }; ;

            return ast;
        }

        private AstNode FromAdd(Expr addExpr)
        {
            var children = addExpr.Args;
            var currentValue = FromZ3Expr(children[0]);
            foreach (var child in children.Skip(1))
            {
                currentValue = new AddNode(currentValue, FromZ3Expr(child));
            }

            return currentValue;
        }

        private AstNode FromMul(Expr addExpr)
        {
            var children = addExpr.Args;
            var currentValue = FromZ3Expr(children[0]);
            foreach (var child in children.Skip(1))
            {
                currentValue = new MulNode(currentValue, FromZ3Expr(child));
            }

            return currentValue;
        }

        private AstNode FromXor(Expr addExpr)
        {
            var children = addExpr.Args;
            var currentValue = FromZ3Expr(children[0]);
            foreach (var child in children.Skip(1))
            {
                currentValue = new XorNode(currentValue, FromZ3Expr(child));
            }

            return currentValue;
        }

        private AstNode FromOr(Expr addExpr)
        {
            var children = addExpr.Args;
            var currentValue = FromZ3Expr(children[0]);
            foreach (var child in children.Skip(1))
            {
                currentValue = new OrNode(currentValue, FromZ3Expr(child));
            }

            return currentValue;
        }

        private AstNode FromNot(Expr notExpr)
        {
            return new NegNode(FromZ3Expr(notExpr.Args.Single()));
        }

        private VarNode FromUninterpreted(Expr expression)
        {
            if (!expression.IsConst || expression.FuncDecl.Range is not BitVecSort bvSort)
                throw new InvalidOperationException($"Cannot translate z3 ast {expression}. (IsConst: {expression.IsConst}) (Range: {expression.FuncDecl.Range})");

            var name = expression.FuncDecl.Name.ToString();
            return new VarNode(name, bvSort.Size);
        }

    }
}
