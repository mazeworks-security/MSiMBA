using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Mba.Ast;
using Mba.Common.Parsing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace Mba.Parsing
{
    public static class AstParser
    {
        public static AstNode Parse(string exprText, uint bitSize)
        {
            // Parse the expression AST.
            var charStream = new AntlrInputStream(exprText);
            var lexer = new ExprLexer(charStream);
            var tokenStream = new CommonTokenStream(lexer);
            var parser = new ExprParser(tokenStream);
            parser.BuildParseTree = true;
            var expr = parser.gamba();

            // Throw if ANTLR has any errors.
            var errCount = parser.NumberOfSyntaxErrors;
            if (errCount > 0)
                throw new InvalidOperationException($"Parsing ast failed. Encountered {errCount} errors.");

            // Process the parse tree into a usable AST node.
            var visitor = new AstTranslationVisitor(bitSize);
            var result = visitor.Visit(expr);
            return result;
        }
    }
}
