using Mba.Ast;
using Mba.Common.MSiMBA;
using Mba.Common.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Minimization
{
    public static class SimbaMinimizer
    {
        // Simplify the boolean expression as a 1-bit polynomial.
        // When the ground truth contains many XORs, this yields exponentially more compact results than DNF.
        // TODO: The result can be refined through factoring and other means.
        public static unsafe AstNode SimplifyBoolean(IReadOnlyList<VarNode> variables, List<int> resultVector)
        {
            var resultVec = resultVector.Select(x => (ulong)x).ToArray();
            var variableCombinations = MultibitSiMBA.GetVariableCombinations(variables.Count);

            // Keep track of which variables are demanded by which combination,
            // as well as which result vector idx corresponds to which combination.
            var groupSizes = MultibitSiMBA.GetGroupSizes(variables.Count);
            List<(ulong trueMask, int resultVecIdx)> combToMaskAndIdx = new();
            for (int i = 0; i < variableCombinations.Length; i++)
            {
                var myMask = variableCombinations[i];
                var myIndex = MultibitSiMBA.GetGroupSizeIndex(groupSizes, myMask);
                combToMaskAndIdx.Add((myMask, (int)myIndex));
            }

            var varCount = variables.Count;
            bool onlyOneVar = varCount == 1;
            int width = (int)(varCount == 1 ? 1 : 2u << (ushort)(varCount - 1));
            List<int> terms = new();
            fixed (ulong* ptr = &resultVec[0])
            {
                for (int i = 0; i < variableCombinations.Length; i++)
                {
                    // Fetch the result vector index for this conjunction.
                    // If the coefficient is zero, we can skip it.
                    var comb = variableCombinations[i];
                    var (trueMask, index) = combToMaskAndIdx[i];
                    var coeff = ptr[index];
                    if (coeff == 0)
                        continue;

                    // Subtract the coefficient from the result vector.
                    MultibitSiMBA.SubtractCoeff(1, ptr, 0, coeff, index, width, varCount, onlyOneVar, trueMask);
                    terms.Add(i);
                }
            }

            AstNode result = null;
            foreach(var term in terms)
            {
                var conj = MultibitSiMBA.ConjunctionFromVarMask(1, variableCombinations[term], variables, null);
                if (result == null)
                    result = conj;
                else
                    result = new XorNode(result, conj);
            }

            return result;
        }
    }
}
