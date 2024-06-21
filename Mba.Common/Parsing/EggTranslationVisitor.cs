using Antlr4.Runtime.Misc;
using Mba.Ast;
using Mba.Common.Parsing;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Parsing
{
    public class EggTranslationVisitor : EggBaseVisitor<AstNode>
    {
        private readonly uint bitSize;

        Dictionary<string, VarNode> varNodes = new();

        public EggTranslationVisitor(uint bitSize)
        {
            this.bitSize = bitSize;
        }

        public override AstNode VisitEgg([NotNull] EggParser.EggContext context)
        {
            return Visit(context.expr());
        }

        public override AstNode VisitEggExpression([NotNull] EggParser.EggExpressionContext context)
        {
            var @operator = context.OPERATOR().GetText();
            var op1 = () => Visit(context.expr(0));
            var op2 = () => Visit(context.expr(1));

            AstNode node = @operator switch
            {
                "**" => new PowerNode(op1(), op2()),
                "*" => Mul(op1(), op2()),
                // Write "a << b" as "a * 2**b".
                "<<" => new ShlNode(op1(), op2()),
                ">>" => new LshrNode(op1(), op2()),
                ">>>" => new AshrNode(op1(), op2()),
                "+" => new AddNode(op1(), op2()),
                "-" => new AddNode(op1(), new MulNode(op2(), new ConstNode(-1, bitSize))),
                "&" => new AndNode(op1(), op2()),
                "|" => new OrNode(op1(), op2()),
                "^" => new XorNode(op1(), op2()),
                "~" => new NegNode(op1()),
                _ => throw new InvalidOperationException($"Unrecognized binary operator: {@operator}")
            };

            return node;
        }

        private AstNode Mul(AstNode op1, AstNode op2)
        {
            return new MulNode(op1, op2);
        }
        
        public override AstNode VisitNumberExpression([NotNull] EggParser.NumberExpressionContext context)
        {
            var text = context.NUMBER().GetText();
            var negPrefix = context.NEGATIVE_NUMBER_PREFIX()?.GetText();
            if (negPrefix != null)
                text = negPrefix + text;

            bool longSuccess = Int128.TryParse(text.Replace("0x", ""), out Int128 result);
            bool ulongSuccess = UInt128.TryParse(text.Replace("0x", ""), out UInt128 uresult);
            if(!longSuccess && !ulongSuccess)
            {
                throw new InvalidOperationException();
            }
            return new ConstNode(longSuccess ? ((long)result) : ((long)uresult), bitSize);
        }

        public override AstNode VisitIdExpression([NotNull] EggParser.IdExpressionContext context)
        {
            var text = context.ID().GetText();
            if (varNodes.TryGetValue(text, out VarNode varNode))
                return varNode;

            varNode = new VarNode(text, bitSize);
            varNodes.Add(text, varNode);
            return varNode;
        }
    }
}
