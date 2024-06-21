using Mba.Ast;
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
    }
}
