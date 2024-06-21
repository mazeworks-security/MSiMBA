using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Utility
{
    public enum AstClassification
    {
        // Bitwise expressions with pure variables(e.g. a&b)
        Bitwise = 0,
        // Bitwise expressions with constants and optional pure variables
        BitwiseWithConstants = 1,
        // Linear combinations of bitwise expressions(including whole variables)
        Linear = 2,
        // Linear combinations of bitwise expressions with constants 
        SemiLinear = 3,
        // Anything that does not fall under the above categories
        Nonlinear = 4,
    }

    public class AstClassifier
    {
        public static IReadOnlyDictionary<AstNode, AstClassification> Classify(AstNode src)
        {
            var mapping = new Dictionary<AstNode, AstClassification>();
            Classify(src, mapping, new HashSet<AstNode>());
            return mapping.AsReadOnly();
        }

        private static void Classify(AstNode node, Dictionary<AstNode, AstClassification> mapping, HashSet<AstNode> constMapping)
        {
            var op1 = () => node.Children[0];
            var op2 = () => node.Children[1];
            var any = (Func<AstNode, bool> predicate) => node.Children.Any(predicate);

            foreach (var operand in node.Children)
            {
                Classify(operand, mapping, constMapping);
            }

            // Constant operations are always linear.
            if (node.Children.Any() && node.Children.All(x => constMapping.Contains(x)))
            {
                constMapping.Add(node);
                mapping[node] = AstClassification.Linear;
                return;
            }

            switch (node)
            {
                // Treat constant and varnodes as linear expressions.
                case ConstNode:
                    constMapping.Add(node);
                    mapping[node] = AstClassification.Linear;
                    break;
                case WildCardConstantNode:
                    mapping[node] = AstClassification.Linear;
                    break;
                case VarNode:
                    mapping[node] = AstClassification.Linear;
                    break;
                case PowerNode:
                    mapping[node] = AstClassification.Nonlinear;
                    break;
                case MulNode:
                    ConstNode constant = null;
                    AstNode other = null;
                    // Identify the constant and non-constant operand.
                    if (node.Children[1] is ConstNode cn)
                    {
                        constant = cn;
                        other = node.Children[0];
                    }

                    else
                    {
                        other = node.Children[1];
                        constant = node.Children[0] as ConstNode;

                        // If neither operand is constant then the multiplication is nonlinear.
                        if (constant == null)
                        {
                            mapping[node] = AstClassification.Nonlinear;
                            return;
                        }
                    }

                    var otherKind = mapping[other];
                    // m1 * bitop becomes linear
                    if (otherKind == AstClassification.Bitwise)
                        mapping[node] = AstClassification.Linear;
                    else if (otherKind == AstClassification.BitwiseWithConstants)
                        mapping[node] = AstClassification.SemiLinear;
                    // Every other kind inherits the classification of the non constant operand
                    else
                        mapping[node] = mapping[other];
                    break;
                case AddNode:
                    if(any(x => mapping[x] == AstClassification.Nonlinear))
                        mapping[node] = AstClassification.Nonlinear;
                    else if(any(x => mapping[x] == AstClassification.SemiLinear))
                        mapping[node] = AstClassification.SemiLinear;
                    else if(any(x => mapping[x] == AstClassification.BitwiseWithConstants))
                        mapping[node] = AstClassification.SemiLinear;
                    else
                        mapping[node] = AstClassification.Linear;
                    break;
                case AndNode:
                case OrNode:
                case XorNode:
                case NegNode:
                    if (any(x => mapping[x] == AstClassification.Nonlinear))
                        mapping[node] = AstClassification.Nonlinear;
                    else if (any(x => mapping[x] == AstClassification.SemiLinear))
                        mapping[node] = AstClassification.Nonlinear;
                    else if (any(x => mapping[x] == AstClassification.Linear && !constMapping.Contains(x) && x is not VarNode varNode))
                        mapping[node] = AstClassification.Nonlinear;
                    else if (any(x => mapping[x] == AstClassification.Linear && constMapping.Contains(x)) || any(x => mapping[x] == AstClassification.BitwiseWithConstants))
                        mapping[node] = AstClassification.BitwiseWithConstants;
                    else
                        mapping[node] = AstClassification.Bitwise;
                    break;
                default:
                    throw new InvalidOperationException($"Unrecognized opcode: {node.Kind}");
            }
        }

        public static bool IsLinear(AstClassification classification)
        {
            return classification switch
            {
                AstClassification.Bitwise => true,
                AstClassification.Linear => true,
                AstClassification.SemiLinear => true,
                AstClassification.Nonlinear => false,
                _ => false,
            };
        }
    }
}
