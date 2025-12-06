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

    public class AstTranslationVisitor : ExprBaseVisitor<AbstractDslNode>
    {
        private readonly uint bitSize;

        private readonly Dictionary<string, VarNode> varNodes;

        private readonly Dictionary<(ulong, uint), ConstNode> constNodes;

        private readonly Dictionary<string, WildCardConstantNode> wildCardConstantNodes;

        public AstTranslationVisitor(uint bitSize)
        {
            this.bitSize = bitSize;
            varNodes = new();
            constNodes = new();
            wildCardConstantNodes = new();

        }

        public AstTranslationVisitor(uint bitSize, Dictionary<string, VarNode> varNodes, Dictionary<(ulong, uint), ConstNode> constNodes, Dictionary<string, WildCardConstantNode> wildCardConstantNodes)
        {
            this.bitSize = bitSize;
            this.varNodes = varNodes;
            this.constNodes = constNodes;
            this.wildCardConstantNodes = wildCardConstantNodes;
        }

        public override AbstractDslNode VisitGamba([NotNull] ExprParser.GambaContext context)
        {
            var dsl = context.dsl();
            if (dsl != null)
                return VisitDsl(dsl);
            return Visit(context.expression());
        }

        public override AbstractDslNode VisitDsl([NotNull] ExprParser.DslContext context)
        {
            var fGroup = context.functionGroup().Select(x => (DslFunctionGroup)Visit(x)).ToList();
            var ruleGroup = context.ruleGroup().Select(x => (DslRuleGroup)Visit(x)).ToList();
            return new Dsl(fGroup, ruleGroup);
        }

        public override AbstractDslNode VisitFunctionGroup([NotNull] ExprParser.FunctionGroupContext context)
        {
            var name = context.ID().GetText();
            var functions = context.function().Select(x => (DslFunction)Visit(x)).ToList();
            return new DslFunctionGroup(name, functions);
        }

        public override AbstractDslNode VisitBuiltinFunction([NotNull] ExprParser.BuiltinFunctionContext context)
        {
            var name = context.ID().GetText();
            var args = context.functionArgument().Select(x => (DslFunctionArgument)Visit(x)).ToList();
            var typeName = context.type().GetText();
            var type = ParseType(typeName);
            return new DslFunction(true, name, args, type, null);
        }

        public override AbstractDslNode VisitImplementedFunction([NotNull] ExprParser.ImplementedFunctionContext context)
        {
            var name = context.ID().GetText();
            var args = context.functionArgument().Select(x => (DslFunctionArgument)Visit(x)).ToList();
            var typeName = context.type().GetText();
            var type = ParseType(typeName);
            var body = Visit(context.expression());
            return new DslFunction(false, name, args, type, body);
        }

        public override AbstractDslNode VisitFunctionArgument([NotNull] ExprParser.FunctionArgumentContext context)
        {
            var name = context.ID().GetText();
            var typeName = context.type().GetText();
            var type = ParseType(typeName);
            return new DslFunctionArgument(name, type);
        }

        private DslType ParseType(string name)
        {
            if (name == "Node")
                return new DslType(DslTypeKind.Node, 0);

            if (!name.StartsWith("i"))
                throw new InvalidOperationException();
            return new DslType(DslTypeKind.Integer, uint.Parse(name.Substring(1)));
        }

        public override AbstractDslNode VisitRuleGroup([NotNull] ExprParser.RuleGroupContext context)
        {
            var name = context.ID().GetText();
            var rules = context.rewriteRule().Select(x => (DslRule)Visit(x)).ToList();
            return new DslRuleGroup(name, rules);
        }

        public override AbstractDslNode VisitRewriteRule([NotNull] ExprParser.RewriteRuleContext context)
        {
            var name = context.ID().GetText();
            var before = Visit(context.expression()[0]);
            var after = Visit(context.expression()[1]);
            var precondition = context.expression().Length > 2 ? Visit(context.expression()[2]) : null;
            return new DslRule(name, before, after, precondition);
        }

        public override AbstractDslNode VisitExpression([NotNull] ExprParser.ExpressionContext context)
        {
            var result = base.VisitExpression(context);
            return result;
        }

        public override AbstractDslNode VisitPowExpression([NotNull] ExprParser.PowExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitMulExpression([NotNull] ExprParser.MulExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitAddOrSubExpression([NotNull] ExprParser.AddOrSubExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitShiftExpression([NotNull] ExprParser.ShiftExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitAndExpression([NotNull] ExprParser.AndExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitXorExpression([NotNull] ExprParser.XorExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitOrExpression([NotNull] ExprParser.OrExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        public override AbstractDslNode VisitLshrExpression([NotNull] ExprParser.LshrExpressionContext context)
            => Binary(context.expression()[0], context.expression()[1], context.children[1].GetText());

        private AbstractDslNode Binary(ExprParser.ExpressionContext exp1, ExprParser.ExpressionContext exp2, string text)
        {
            var op1 = (AstNode)Visit(exp1);
            var op2 = Visit(exp2);
            var binaryOperator = text;

            AbstractDslNode node = binaryOperator switch
            {
                "**" => new PowerNode(op1, op2),
                "*" => Mul(op1, op2),
                // Write "a << b" as "a * 2**b".
                "<<" => Shl(op1, op2),
                //"<<" => new ShlNode(op1, op2),
                "+" => new AddNode(op1, op2),
                "-" => new AddNode(op1, new MulNode(op2, Const(-1, op1.BitSize))),
                "&" => new AndNode(op1, op2),
                "|" => new OrNode(op1, op2),
                "^" => new XorNode(op1, op2),
                ">>" => new LshrNode(op1, op2),
                _ => throw new InvalidOperationException($"Unrecognized binary operator: {binaryOperator}")
            };

            return node;
        }

        private AbstractDslNode Mul(AbstractDslNode op1, AbstractDslNode op2)
        {
            var def = new MulNode(op1, op2);
            return def;
        }

        private AbstractDslNode Shl(AstNode op1, AstNode op2)
        {
            if (op2 is ConstNode constNode)
            {
                var degree = constNode.Value;
                // If we have shl x<<0, return x
                if (degree == 0)
                    return op1;

                ulong coeff = 2;
                for (int i = 1; i < degree; i++)
                {
                    coeff *= 2;
                }

                return new MulNode(Const(coeff, op1.BitSize), op1);
            }

            var def = new MulNode(op1, new PowerNode(Const((ulong)2, op1.BitSize), op2));
            return def;
        }

        public override AbstractDslNode VisitParenthesizedExpression([NotNull] ExprParser.ParenthesizedExpressionContext context)
        {
            return Visit(context.expression());
        }

        public override AbstractDslNode VisitNegativeOrNegationExpression([NotNull] ExprParser.NegativeOrNegationExpressionContext context)
        {
            var op1 = (AstNode)Visit(context.expression());
            var unaryOperator = context.children[0].GetText();

            AbstractDslNode node = unaryOperator switch
            {
                "~" => new NegNode(op1),
                // Write "-x" as "x * -1".
                // Note that if "x" is a constant(which happens because our parser interprets negative constants as subtraction), we propagate the entire expression to a negative constant/
                "-" => op1 is ConstNode constNode ? GetNegativeConstant((UInt128)constNode.Value, op1.BitSize) : new MulNode(op1, Const(-1, op1.BitSize)),
                _ => throw new InvalidOperationException($"Unrecognized unary operator: {unaryOperator}")
            };

            return node;
        }

        // Truncate the constant down to our bitsize, then multiply it by -1 and turn it into a ConstNode.
        private ConstNode GetNegativeConstant(UInt128 value, uint size) => Const(0 - (ulong)ModuloReducer.ReduceToModulo((ulong)value, size), size);

        public override AbstractDslNode VisitZextExpression([NotNull] ExprParser.ZextExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.type());
            return new ZextNode(op1, width);
        }

        public override AbstractDslNode VisitSextExpression([NotNull] ExprParser.SextExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.type());
            return new SextNode(op1, width);
        }

        public override AbstractDslNode VisitTruncExpression([NotNull] ExprParser.TruncExpressionContext context)
        {
            var op1 = Visit(context.expression());
            var width = GetWidth(context.type());
            return new TruncNode(op1, width);
        }

        public override AbstractDslNode VisitICmpExpression([NotNull] ExprParser.ICmpExpressionContext context)
        {
            var op1 = Visit(context.expression(0));
            var op2 = Visit(context.expression(1));
            var cmpOperator = context.ICMP_OPERATOR().GetText();
            var predicate = cmpOperator switch
            {
                "==" => Predicate.Eq,
                "!=" => Predicate.Ne,
                ">" => Predicate.Ugt,
                ">=" => Predicate.Uge,
                "<" => Predicate.Ult,
                "<=" => Predicate.Ule,
                ">s" => Predicate.Sgt,
                ">=s" => Predicate.Sge,
                "<s" => Predicate.Slt,
                "<=s" => Predicate.Sle,
                _ => throw new InvalidOperationException($"Unrecognized icmp operator {cmpOperator}"),
            };


            return new ICmpNode(predicate, op1, op2);
        }

        public override AbstractDslNode VisitSelectExpression([NotNull] ExprParser.SelectExpressionContext context)
        {
            var op1 = Visit(context.expression(0));
            var op2 = Visit(context.expression(1));
            var op3 = Visit(context.expression(2));
            return new SelectNode(op1, op2, op3);
        }

        private uint GetWidth(ExprParser.TypeContext widthSpecifier)
            => uint.Parse(widthSpecifier.ID().ToString().Substring(1));

        public override AbstractDslNode VisitNumberExpression([NotNull] ExprParser.NumberExpressionContext context)
        {
            var text = context.NUMBER().GetText();
            var value = (ulong)UInt128.Parse(text.Replace("0x", ""), text.Contains("0x") ? NumberStyles.HexNumber : NumberStyles.Number);
            var size = context.type() != null ? GetWidth(context.type()) : bitSize;
            var constNode = Const(value, size);
            return constNode;
        }

        private ConstNode Const(long value, uint bitSize)
            => Const((ulong)value, bitSize);

        private ConstNode Const(ulong value, uint bitSize)
        {
            // if (constNodes.TryGetValue((value, bitSize), out var existing))
            //    return existing;
            var node = new ConstNode(value, bitSize);
            // constNodes[(value, bitSize)] = node;
            return node;
        }

        public override AbstractDslNode VisitWildCardNumberExpression([NotNull] ExprParser.WildCardNumberExpressionContext context)
        {
            var text = context.ID().GetText();

            /*
            if (varNodes.TryGetValue(text, out VarNode varNode))
                throw new InvalidOperationException($"Variable name cannot be shared across wild card constant and variable nodes!");
            if (wildCardConstantNodes.TryGetValue(text, out var wcNode))
                return wcNode;
            */

            var wcNode = new WildCardConstantNode(text, bitSize);
            //wildCardConstantNodes.Add(text, wcNode);
   

            return wcNode;
        }

        public override AbstractDslNode VisitIntrinsicCallExpression([NotNull] ExprParser.IntrinsicCallExpressionContext context)
        {
            var name = context.ID().GetText();
            var t = context.GetText();
            var expr = context.expression();
            var args = expr.Select(x => (AstNode)Visit(x)).ToList();
            return new IntrinsicCallNode(name, 64, args);
        }

        public override AbstractDslNode VisitIdExpression([NotNull] ExprParser.IdExpressionContext context)
        {
            var text = context.ID().GetText();
            //if (varNodes.TryGetValue(text, out VarNode varNode))
            //    return varNode;

            var size = context.type() != null ? GetWidth(context.type()) : bitSize;

            var varNode = new VarNode(text, size);
            //varNodes.Add(text, varNode);
            return varNode;
        }
    }
}
