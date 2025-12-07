using Mba.Ast;
using Mba.Common.Ast;
using Mba.Testing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.MSiMBA
{
    public static class AstCloner
    {
        public static AstNode CloneAst(AstNode ast, Dictionary<VarNode, AstNode> variables, uint size)
        {
            var op1 = () => CloneAst(ast.Children[0], variables, size);
            var op2 = () => CloneAst(ast.Children[1], variables, size);

            var processVar = (VarNode v) =>
            {
                if (variables.TryGetValue(v, out var existing))
                    return existing;

                var newVar = new VarNode(v.Name, v.BitSize);
                variables.Add(v, newVar);
                return newVar;
            };

            return ast switch
            {
                ConstNode constNode => new ConstNode(constNode.Value, size),
                VarNode varNode => processVar(varNode),
                PowerNode powerNode => new PowerNode(op1(), op2()),
                AddNode => new AddNode(op1(), op2()),
                MulNode mulNode => new MulNode(op1(), op2()),
                AndNode andNode => new AndNode(op1(), op2()),
                OrNode orNode => new OrNode(op1(), op2()),
                XorNode => new XorNode(op1(), op2()),
                NegNode => new NegNode(op1()),
            };
        }

        public static AstNode ReplaceVars(AstNode ast, Dictionary<string, AstNode> varToReplacement)
        {
            var func = (AstNode x) =>
            {
                if (x is not VarNode varNode)
                    return null;

                return varToReplacement[varNode.Name];
            };

            return Clone(ast, func);
        }

        public static AstNode ReplaceConstants(AstNode ast, Dictionary<ulong, AstNode> constToReplacement)
        {
            var func = (AstNode x) =>
            {
                if (x is not ConstNode constNode)
                    return null;

                return constToReplacement[constNode.UValue];
            };

            return Clone(ast, func);
        }

        public static AstNode Clone(AstNode ast, Func<AstNode, AstNode> process)
        {
            var children = ast.Children.ToList();
            for(int i = 0; i < children.Count; i++)
            {
                var child = children[i];
                children[i] = Clone(child, process);
            }

            var op1 = () => children[0];
            var op2 = () => children[1];
            var op3 = () => children[2];
            
            AstNode curr = ast switch
            {
                ConstNode constNode => new ConstNode(constNode.Value, constNode.BitSize),
                VarNode varNode => new VarNode(varNode.Name, varNode.BitSize),
                PowerNode powerNode => new PowerNode(op1(), op2()),
                AddNode => new AddNode(op1(), op2()),
                MulNode mulNode => new MulNode(op1(), op2()),
                AndNode andNode => new AndNode(op1(), op2()),
                OrNode orNode => new OrNode(op1(), op2()),
                XorNode => new XorNode(op1(), op2()),
                NegNode => new NegNode(op1()),
                LshrNode => new LshrNode(op1(), op2()),
                ZextNode zextNode => new ZextNode(op1(), zextNode.BitSize),
                TruncNode truncNode => new TruncNode(op1(), truncNode.BitSize),
                ICmpNode cmpNode => new ICmpNode(cmpNode.Pred, op1(), op2()),
                SelectNode => new SelectNode(op1(), op2(), op3()),
                WildCardConstantNode wc => new WildCardConstantNode(wc.Name, wc.BitSize), 
                IntrinsicCallNode call => new IntrinsicCallNode(call.Name, call.BitSize, children),
                ConditionalAndNode andNode => new ConditionalAndNode(op1(), op2()),
                ConditionalOrNode orNode => new ConditionalOrNode(op1(), op2()),
                _ => throw new InvalidOperationException(),
            };

            var processed = process(curr);
            if (processed != null)
                return processed;
            return curr;

        }
    }
}
