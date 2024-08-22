using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Utility
{
    public static class AstExtensions
    {
        public static AstNode Or(this List<AstNode> nodes)
        {
            var initial = nodes[0];
            for (int i = 1; i < nodes.Count; i++)
            {
                initial = new OrNode(initial, nodes[i]);
            }

            return initial;
        }

        public static AstNode And(this List<AstNode> nodes)
        {
            var initial = nodes[0];
            for (int i = 1; i < nodes.Count; i++)
            {
                initial = new AndNode(initial, nodes[i]);
            }

            return initial;
        }

        public static AstNode Add(this List<AstNode> nodes)
        {
            var initial = nodes[0];
            for (int i = 1; i < nodes.Count; i++)
            {
                initial = new AddNode(initial, nodes[i]);
            }

            return initial;
        }
    }
}
