using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Parsing
{
    public enum DslTypeKind
    {
        Integer,
        Node,
    }

    public class Dsl(IReadOnlyList<DslFunctionGroup> functionGroups, IReadOnlyList<DslRuleGroup> ruleGroups) : AbstractDslNode
    {
        public IReadOnlyList<DslFunctionGroup> FunctionGroups { get; } = functionGroups;
        public IReadOnlyList<DslRuleGroup> RuleGroups { get; } = ruleGroups;
    }

    public class DslFunctionGroup(string name, IReadOnlyList<DslFunction> functions) : AbstractDslNode
    {
        public string Name { get; } = name;
        public IReadOnlyList<DslFunction> Functions { get; } = functions;
    }

    public class DslFunction(bool isBuiltin, string name, IReadOnlyList<DslFunctionArgument> arguments, DslType returnType, AstNode body) : AbstractDslNode
    {
        public bool IsBuiltin { get; } = isBuiltin;
        public string Name { get; } = name;
        public IReadOnlyList<DslFunctionArgument> Arguments { get; } = arguments;
        public DslType ReturnType { get; } = returnType;
        public AstNode Body { get; set; } = body;
    }

    public class DslFunctionArgument(string name, DslType type) : AbstractDslNode
    {
        public string Name { get; } = name;
        public DslType Type { get; } = type;
    }

    public class DslType(DslTypeKind kind, uint Width) : AbstractDslNode
    {
        public DslTypeKind Kind { get; } = kind;
        public uint Width { get; } = Width;
    }

    public class DslRuleGroup(string name, IReadOnlyList<DslRule> rules) : AbstractDslNode
    {
        public string Name { get; } = name;
        public IReadOnlyList<DslRule> Rules { get; } = rules;
    }

    public class DslRule(string name, AstNode before, AstNode after, AstNode precondition) : AbstractDslNode
    {
        public string Name { get; } = name;
        public AstNode Before { get; set; } = before;
        public AstNode After { get; set; } = after;
        public AstNode Precondition { get; set; } = precondition;

        public override string ToString()
        {
            var str = $"{Name}: {Before} => {After}";
            if (Precondition != null)
                str += $" :: {Precondition}";
            return str;
        }
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
        public unsafe static implicit operator AstNode(AbstractDslNode ctx) => ctx == null ? null : ((DslAstParsingWrapper)ctx).astNode;

        public unsafe static implicit operator AbstractDslNode(AstNode ctx) => new DslAstParsingWrapper(ctx);
    }

}
