using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Utility
{
    public class InputVariableUtility
    {
        public static void CollectInputVariables(AstNode x, HashSet<VarNode> variables)
        {
            if (x is VarNode varNode)
            {
                variables.Add(varNode);
                return;
            }

            foreach(var child in x.Children)
                CollectInputVariables(child, variables);
        }

        public static HashSet<VarNode> GetVarSet(AstNode ast)
        {
            var variables = AstClassifier.Classify(ast)
             .Select(x => x.Key)
             .Where(x => x is VarNode varNode).Cast<VarNode>().ToHashSet();

            return variables;
        }

        public static IReadOnlyList<VarNode> GetVars(AstNode ast, bool mutate = true)
        {
            var output = new HashSet<VarNode>();
            Collect(ast, output);

            var variables = output.OrderBy(x => x.Name).ToList();
            if (mutate)
            {
                for (int i = 0; i < variables.Count; i++)
                {
                    variables[i].index = i;
                }
            }

            return variables;
        }

        private static void Collect(AstNode ast, HashSet<VarNode> variables)
        {
            var kind = ast.Kind;
            if (kind == AstKind.Var)
            {
                variables.Add((VarNode)ast);
            }

            else if (kind != AstKind.Const)
            {
                foreach (var child in ast.Children)
                    Collect(child, variables);
            }
        }

        public static IReadOnlyList<VarNode> GetVariables(AstNode ast, bool mutate = true)
        {
            var variables = AstClassifier.Classify(ast)
                .Select(x => x.Key)
                .Where(x => x is VarNode varNode).ToHashSet().OrderBy(x => (x as VarNode).Name).Cast<VarNode>().ToList();


            //  Console.WriteLine("Variables: ");
            // If requested, assign a numeric index to each variable.
            if (mutate)
            {
                for (int i = 0; i < variables.Count; i++)
                {
                    variables[i].index = i;
                    //  Console.WriteLine($"    {variables[i].Name}");
                }
            }

            return variables;
        }
    }
}
