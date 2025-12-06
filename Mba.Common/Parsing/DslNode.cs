using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Parsing
{
    public enum DslType
    {
        Bool,
        U8,
        U64,
        Node,
    }

    public abstract class DslNode
    {
        public unsafe static implicit operator AstNode(DslNode ctx) => ((DslAstParsingWrapper)ctx).astNode;

        public unsafe static implicit operator DslNode(AstNode ctx) => new DslAstParsingWrapper(ctx);
    }

    public class DslFunction(bool isBuiltin, string name, IReadOnlyList<DslFunctionArgument> arguments, DslType returnType, AstNode body)
    {
        public bool IsBuiltin { get; } = isBuiltin;
        public string Name { get; } = name;
        public IReadOnlyList<DslFunctionArgument> Arguments { get; } = arguments;
        public DslType ReturnType { get; } = returnType;
        public AstNode Body { get; } = body;
    }

    public class DslFunctionArgument(string name, DslType type) : DslNode
    {
        public string Name { get; } = name;
        public DslType Type { get; } = type;
    }

    public class DslRuleGroup(string name, IReadOnlyList<DslRule> rules) : DslNode
    {
        public string Name { get; } = name;
        public IReadOnlyList<DslRule> Rules { get; } = rules;
    }

    public class DslRule(string name, AstNode before, AstNode after, AstNode precondition) : DslNode
    {
        public string Name { get; } = name;
        public AstNode Before { get; } = before;
        public AstNode After { get; } = after;
        public AstNode Precondition { get; } = precondition;
    }

    public class DslAstParsingWrapper : DslNode
    {
        public readonly AstNode astNode;

        public DslAstParsingWrapper(AstNode astNode) 
        {
            this.astNode = astNode;
        }
    }
}
