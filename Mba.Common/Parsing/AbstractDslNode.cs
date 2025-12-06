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

    public class Dsl(IReadOnlyList<DslFunctionGroup> functionGroups, IReadOnlyList<DslRuleGroup> ruleGroups)
    {
        public IReadOnlyList<DslFunctionGroup> FunctionGroups { get; } = functionGroups;
        public IReadOnlyList<DslRuleGroup> RuleGroups { get; } = ruleGroups;
    }

    public class DslFunctionGroup(string name, IReadOnlyList<DslFunction> functions) : AbstractDslNode
    {
        public string Name { get; } = name;
        public IReadOnlyList<DslFunction> Functions { get; } = functions;
    }

    public class DslFunction(bool isBuiltin, string name, IReadOnlyList<DslFunctionArgument> arguments, DslType returnType, AstNode body)
    {
        public bool IsBuiltin { get; } = isBuiltin;
        public string Name { get; } = name;
        public IReadOnlyList<DslFunctionArgument> Arguments { get; } = arguments;
        public DslType ReturnType { get; } = returnType;
        public AstNode Body { get; } = body;
    }

    public class DslFunctionArgument(string name, DslType type) : AbstractDslNode
    {
        public string Name { get; } = name;
        public DslType Type { get; } = type;
    }

    public class DslRuleGroup(string name, IReadOnlyList<DslRule> rules) : AbstractDslNode
    {
        public string Name { get; } = name;
        public IReadOnlyList<DslRule> Rules { get; } = rules;
    }

    public class DslRule(string name, AstNode before, AstNode after, AstNode precondition) : AbstractDslNode
    {
        public string Name { get; } = name;
        public AstNode Before { get; } = before;
        public AstNode After { get; } = after;
        public AstNode Precondition { get; } = precondition;
    }

    public class DslAstParsingWrapper : AbstractDslNode
    {
        public readonly AstNode astNode;

        public DslAstParsingWrapper(AstNode astNode) 
        {
            this.astNode = astNode;
        }
    }

    public abstract class AbstractDslNode
    {
        public unsafe static implicit operator AstNode(AbstractDslNode ctx) => ((DslAstParsingWrapper)ctx).astNode;

        public unsafe static implicit operator AbstractDslNode(AstNode ctx) => new DslAstParsingWrapper(ctx);
    }

}
