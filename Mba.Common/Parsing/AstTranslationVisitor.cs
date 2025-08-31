using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using Mba.Ast;
using Mba.Common.Ast;
using Mba.Common.Parsing;
using Mba.Utility;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Parsing
{
    public class AstTranslationVisitor : ExprBaseVisitor<AstNode>
    {
        private readonly uint bitSize;

        private readonly Dictionary<string, VarNode> varNodes = new();

        private readonly Dictionary<string, WildCardConstantNode> wildCardConstantNodes = new();

        public AstTranslationVisitor(uint bitSize)
        {
            this.bitSize = bitSize;
        }

        public override AstNode VisitGamba([NotNull] ExprParser.GambaContext context)
        {
            return Visit(context.expression());
        }

        public override AstNode VisitExpression([NotNull] ExprParser.ExpressionContext context)
        {
            var result = base.VisitExpression(context);
            return result;
        }

        public override AstNode VisitPowExpression([NotNull] ExprParser.PowExpressionContext context) 
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitMulExpression([NotNull] ExprParser.MulExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitAddOrSubExpression([NotNull] ExprParser.AddOrSubExpressionContext context) 
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitShiftExpression([NotNull] ExprParser.ShiftExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitAndExpression([NotNull] ExprParser.AndExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitXorExpression([NotNull] ExprParser.XorExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitOrExpression([NotNull] ExprParser.OrExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AstNode VisitLshrExpression([NotNull] ExprParser.LshrExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        private AstNode Binary(ExprParser.ExpressionContext exp1, ExprParser.ExpressionContext exp2, string text)
        {
            var op1 = Visit(exp1);
            var op2 = Visit(exp2);
            var binaryOperator = text;

            AstNode node = binaryOperator switch
            {
                "**" => new PowerNode(op1, op2),
                "*" => Mul(op1, op2),
                // Write "a << b" as "a * 2**b".
                "<<" => Shl(op1, op2),
                //"<<" => new ShlNode(op1, op2),
                "+" => new AddNode(op1, op2),
                "-" => new AddNode(op1, new MulNode(op2, new ConstNode(-1, bitSize))),
                "&" => new AndNode(op1, op2),
                "|" => new OrNode(op1, op2),
                "^" => new XorNode(op1, op2),
                ">>" => new LshrNode(op1, op2),
                _ => throw new InvalidOperationException($"Unrecognized binary operator: {binaryOperator}")
            } ;

            return node;
        }

        private AstNode Mul(AstNode op1, AstNode op2)
        {
            var def = new MulNode(op1, op2);
            return def;
        }

        private AstNode Shl(AstNode op1, AstNode op2)
        {
            if(op2 is ConstNode constNode)
            {
                var degree = constNode.Value;
                // If we have shl x<<0, return x
                if (degree == 0)
                    return op1;

                ulong coeff = 2;
                for(int i = 1; i < degree; i++)
                {
                    coeff *= 2;
                }

                return new MulNode(new ConstNode(coeff, op1.BitSize), op1);
            }

            var def = new MulNode(op1, new PowerNode(new ConstNode((ulong)2, op1.BitSize), op2));
            return def;
        }

        public override AstNode VisitParenthesizedExpression([NotNull] ExprParser.ParenthesizedExpressionContext context)
        {
            return Visit(context.expression());
        }

        public override AstNode VisitNegativeOrNegationExpression([NotNull] ExprParser.NegativeOrNegationExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var unaryOperator = context.children[0].GetText();

            AstNode node = unaryOperator switch
            {
                "~" => new NegNode(op1),
                // Write "-x" as "x * -1".
                // Note that if "x" is a constant(which happens because our parser interprets negative constants as subtraction), we propagate the entire expression to a negative constant/
                "-" => op1 is ConstNode constNode ? GetNegativeConstant((UInt128)constNode.Value) : new MulNode(op1, new ConstNode(-1, bitSize)),
                _ => throw new InvalidOperationException($"Unrecognized unary operator: {unaryOperator}")
            };

            return node;
        }

        // Truncate the constant down to our bitsize, then multiply it by -1 and turn it into a ConstNode.
        private ConstNode GetNegativeConstant(UInt128 value) => new ConstNode(0 - (ulong)ModuloReducer.ReduceToModulo(value, bitSize), bitSize);

        public override AstNode VisitZextExpression([NotNull] ExprParser.ZextExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.WIDTH_SPECIFIER());
            return new ZextNode(op1, width);
        }

        public override AstNode VisitSextExpression([NotNull] ExprParser.SextExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.WIDTH_SPECIFIER());
            return new SextNode(op1, width);
        }

        public override AstNode VisitTruncExpression([NotNull] ExprParser.TruncExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.WIDTH_SPECIFIER());
            return new TruncNode(op1, width);
        }

        private uint GetWidth(ITerminalNode widthSpecifier)
            => uint.Parse(widthSpecifier.ToString().Substring(1));

        public override AstNode VisitNumberExpression([NotNull] ExprParser.NumberExpressionContext context)
        {
            var text = context.NUMBER().GetText();
            var value = (ulong)UInt128.Parse(text.Replace("0x", ""), text.Contains("0x") ? NumberStyles.HexNumber : NumberStyles.Number);
            var size = context.WIDTH_SPECIFIER() != null ? GetWidth(context.WIDTH_SPECIFIER()) : bitSize;
            return new ConstNode(value, size);
        }

        public override AstNode VisitWildCardNumberExpression([NotNull] ExprParser.WildCardNumberExpressionContext context)
        {
            var text = context.ID().GetText();
            if (varNodes.TryGetValue(text, out VarNode varNode))
                throw new InvalidOperationException($"Variable name cannot be shared across wild card constant and variable nodes!");
            if (wildCardConstantNodes.TryGetValue(text, out var wcNode))
                return wcNode;

            wcNode = new WildCardConstantNode(text, bitSize);
            wildCardConstantNodes.Add(text, wcNode);
            return wcNode;
        }

        public override AstNode VisitIdExpression([NotNull] ExprParser.IdExpressionContext context)
        {
            var text = context.ID().GetText();
            if (varNodes.TryGetValue(text, out VarNode varNode))
                return varNode;

            var size = context.WIDTH_SPECIFIER() != null ? GetWidth(context.WIDTH_SPECIFIER()) : bitSize;

            varNode = new VarNode(text, size);
            varNodes.Add(text, varNode);
            return varNode;
        }
    }
}
