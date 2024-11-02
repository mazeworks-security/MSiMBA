using Mba.Common.Interop;
using Microsoft.Z3;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using ApInt = System.UInt64;

namespace Mba.Common.MSiMBA
{
    // Implements refinements and analyses over semi-linear expressions
    public class MultibitRefiner
    {
        private uint bitSize;

        private readonly ulong moduloMask;

        public MultibitRefiner(uint bitSize, ApInt moduloMask)
        {
            this.bitSize = bitSize;
            this.moduloMask = moduloMask;
        }

        // Try to simplify the linear combination of conjunctions.
        public Dictionary<ApInt, ApInt> SimplifyMultibitEntry(IReadOnlyList<(ApInt coeff, ApInt bitMask)> linearCombinations)
        {
            // (1) Merge expressions with the same coefficient and same boolean function.
            // This could be much simpler, but it comes at the cost of many additional dictionary lookups / insertions.
            var coeffToMask = new Dictionary<ApInt, ApInt>();
            ApInt lastCoeff = 0;
            ApInt lastMask = 0;
            foreach (var (coeff, mask) in linearCombinations)
            {
                // Skip zero coefficients.
                if (coeff == 0)
                    continue;

                // If this is the same coefficient, OR the mask.
                if (lastCoeff == coeff)
                {
                    Debug.Assert((lastMask & mask) == 0);
                    lastMask |= mask;
                }

                // If the coefficient changed:
                else
                {
                    // If the last coefficient was non-zero, we need to update the dictionary for the old entry.
                    if (lastCoeff != 0)
                    {
                        ApInt existingMask = 0;
                        coeffToMask.TryGetValue(lastCoeff, out existingMask);
                        var newMask = existingMask | lastMask;
                        coeffToMask[lastCoeff] = newMask;
                    }

                    // Update the last coefficient/mask.
                    lastCoeff = coeff;
                    lastMask = mask;
                }
            }

            if (lastCoeff != 0)
            {
                coeffToMask.TryGetValue(lastCoeff, out ApInt finalMask);
                coeffToMask[lastCoeff] = finalMask | lastMask;
            }

            // Try to merge terms.
            TryReduceTermCount(coeffToMask);

            // Simplify expressions that are now known to be zero.
            SimplifyKnownZeroes(coeffToMask);

            // Try to reduce as many coefficients as possible down to -1.
            /*
            var changed = true;
            while (changed)
                changed = TryReduceCoefficients(coeffToMask);
            */

            // Try to reduce the number of terms again.
            TryReduceTermCount(coeffToMask);

            // (7) Try to minimize the number of terms under this basis expression,
            // by expressing say three elements as the sum of two other elements.
            TryEliminateUniqueMultibitValues(coeffToMask);

            return coeffToMask;
        }

        public unsafe void TryReduceTermCount(Dictionary<ApInt, ApInt> coeffToMask)
        {
            var len = coeffToMask.Count;
            var arr = stackalloc CoeffWithMask[len];
            int idx = 0;
            foreach (var entry in coeffToMask)
            {
                arr[idx] = new(entry.Key, entry.Value);
                idx++;
            }

            UnmanagedAnalyses.SimplifyDisjointSumMultiply(arr, len, moduloMask);

            coeffToMask.Clear();

            for (int i = 0; i < len; i++)
            {
                var entry = arr[i];
                if (entry.mask == 0)
                    continue;

                coeffToMask[entry.coeff] = entry.mask;
            }
        }

        public void SimplifyKnownZeroes(Dictionary<ApInt, ApInt> coeffToMask)
        {
            var clone = coeffToMask.ToDictionary(x => x.Key, x => x.Value);
            foreach (var (coeff, mask) in clone)
            {
                // Skip if we cannot change the coefficient to zero
                if (!CanChangeCoefficientTo(coeff, 0, mask))
                    continue;
                coeffToMask.Remove(coeff);
            }
        }

        // Try to reduce each coefficient to -1.
        public bool TryReduceCoefficients(Dictionary<ApInt, ApInt> coeffToMask)
        {
            // Clone and return as soon as we modify, to avoid collections getting out of sync.
            var clone = coeffToMask.ToDictionary(x => x.Key, x => x.Value);
            foreach (var (coeff, mask) in clone)
            {
                // Skip if more than one bit in set.
                var popcount = BitOperations.PopCount(mask);
                if (popcount != 1)
                    continue;

                // Skip if we cannot rewrite this as multiplication by negative one.
                bool isLowerable = (moduloMask & (coeff * mask)) == (moduloMask & (moduloMask * mask));
                if (!isLowerable)
                    continue;

                // Skip if the entry at -1 shares any set bits with the current mask.
                if (coeffToMask.TryGetValue(moduloMask, out var existing) && (existing & mask) != 0)
                    continue;

                // Otherwise we found a match.
                // Update the mask for the entry at -1, such that our mask is ORed with the existing mask.
                coeffToMask.Remove(coeff);
                coeffToMask.TryAdd(moduloMask, 0);
                coeffToMask[moduloMask] = coeffToMask[moduloMask] | mask;
                return true;
            }

            return false;
        }

        // Try to recover XORs.
        public (ApInt adjustedConstantOffset, ApInt coeff, ApInt xorMask)? TrySimplifyXor(ApInt constantOffset, Dictionary<ApInt, ApInt> coeffToMask)
        {
            var coeffs = coeffToMask.Keys.OrderByDescending(x => x).ToList();
            for (int a = 0; a < coeffs.Count; a++)
            {
                // Skip entries with a coefficient of zero.
                var coeffA = coeffs[a];
                if (coeffA == 0)
                    continue;

                var mask = coeffToMask[coeffA];

                // Skip if a term does not exist with an inverse(multiplied by UNSIGNED_MAX) coefficient.
                var inverse = moduloMask & (moduloMask * coeffA);
                if (!coeffToMask.TryGetValue(inverse, out var otherMask))
                    continue;

                // If we cannot change the other mask to a negation of the first mask,
                // then we do not have a XOR relationship.
                var negatedMask = moduloMask & ~mask;
                if (!CanChangeMaskTo(inverse, otherMask, negatedMask))
                    continue;

                var multiplied = moduloMask & (inverse * mask);

                coeffToMask.Remove(coeffA);
                coeffToMask.Remove(inverse);
                var adjustedConstantOffset = moduloMask & (constantOffset - multiplied);
                return (adjustedConstantOffset, inverse, mask);
            }

            return null;
        }

        // Try to recover ORs by constant.
        public (ApInt adjustedConstantOffset, ApInt coeff, ApInt orMask)? TrySimplifyOr(ApInt constantOffset, Dictionary<ApInt, ApInt> coeffToMask)
        {
            // If there is no constant offset, we cannot recover a bitwise OR by constant.
            if (constantOffset == 0)
                return null;

            var coeffs = coeffToMask.Keys.OrderByDescending(x => x).ToList();
            for (int a = 0; a < coeffs.Count; a++)
            {
                // Skip entries with a coefficient of zero.
                var coeffA = coeffs[a];
                if (coeffA == 0)
                    continue;

                var mask = coeffToMask[coeffA];

                // Skip if there is no room for a bitwise OR by constant.
                var negatedMask = moduloMask & ~mask;
                if (negatedMask == 0)
                    continue;

                // We only want to decrease the constant offset.
                // If creating a bitwise OR would increase the constant offset,
                // it's likely not profitable.
                var multiplied = moduloMask & (coeffA * negatedMask);
                if (multiplied > constantOffset)
                    continue;

                // Otherwise we've found an OR by constant.
                coeffToMask.Remove(coeffA);
                var adjustedConstantOffset = moduloMask & (constantOffset - multiplied);
                return (adjustedConstantOffset, coeffA, negatedMask);
            }

            return null;
        }

        // Try to express say 3 elements as the sum of two other elements.
        public void TryEliminateUniqueMultibitValues(Dictionary<ApInt, ApInt> coeffToMask)
        {
            (ApInt coeff, ApInt mask)[] uniqueValues = coeffToMask.Select(x => (x.Key, x.Value)).ToArray();
            var l = uniqueValues.Length;
            if (l == 0)
                return;

            // Try to get rid of a value by representing it as a sum of the others.
            foreach (var i in RangeUtil.Get(l - 1))
            {
                foreach (var j in RangeUtil.Get(i + 1, l))
                {
                    foreach (var k in RangeUtil.Get(l))
                    {
                        if (k == i || k == j)
                            continue;

                        // Skip any entries with zero coefficients.
                        var op0 = uniqueValues[i];
                        var op1 = uniqueValues[j];
                        var op2 = uniqueValues[k];
                        if (op0.coeff == 0 || op1.coeff == 0 || op2.coeff == 0)
                            continue;

                        // Try to express three elements as the sum of two others.
                        var maybeSum = TryAsMultibitSumModulo(op0.coeff, op0.mask, op1.coeff, op1.mask, op2.coeff, op2.mask);
                        if (maybeSum == null)
                            continue;

                        // If we succeeded, set the third element's coefficient to zero.
                        uniqueValues[k] = (0, op2.mask);

                        // Update the coefficients of the remaining elements.
                        var asSum = maybeSum.Value;
                        uniqueValues[i] = (asSum.coeff0, asSum.mask0);
                        uniqueValues[j] = (asSum.coeff1, asSum.mask1);
                    }
                }
            }

            coeffToMask.Clear();
            foreach (var entry in uniqueValues)
            {
                if (entry.coeff == 0)
                    continue;

                coeffToMask.Add(entry.coeff, entry.mask);
            }
        }
        // Check if we can express three elements as the sum of two others.
        public (ApInt coeff0, ApInt mask0, ApInt coeff1, ApInt mask1)? TryAsMultibitSumModulo(ApInt coeffA, ApInt maskA, ApInt coeffB, ApInt maskB, ApInt coeffC, ApInt maskC)
        {
            // The first two coefficients must sum up to the third coefficient.
            if ((moduloMask & (coeffA + coeffB)) != coeffC)
                return null;
            // The two bitmasks must be disjoint for the merger to happen.
            if ((maskB & maskC) != 0)
                return null;
            // All combinations of bit masks must be disjoint for this to work.
            // In the future there may be cases where clever rearranging of the coefficients/bitmasks
            // can yield fewer terms in the presence of non-disjoint bit masks.
            if ((maskA & maskB) != 0)
                return null;
            if ((maskA & maskC) != 0)
                return null;

            // Otherwise we have a match.
            var coeff0 = coeffA;
            var mask0 = maskA | maskC;

            var coeff1 = coeffB;
            var mask1 = maskB | maskC;
            return (coeff0, mask0, coeff1, mask1);
        }

        public void TryReduceMasks(Dictionary<ApInt, ApInt> coeffToMask)
        {
            // Try to reduce the number of set bits within the bit mask.
            foreach (var (coeff, mask) in coeffToMask)
            {
                var minimalMask = FindMinimalMask(coeff, mask);
                coeffToMask[coeff] = minimalMask;
            }
        }

        public ApInt? TryIsolateSingleVariableConjunction(Dictionary<ApInt, ApInt> coeffToMask)
        {
            // If there is only one entry, and it's mask is negative one, we can isolate the variable immediately.
            if (coeffToMask.Count == 1)
            {
                var (coeff, mask) = coeffToMask.First();
                if (mask == moduloMask || CanRemoveMask(coeff, mask))
                {
                    coeffToMask.Clear();
                    return coeff;
                }

                return null;
            }

            ApInt? variableCoefficient = null;
            var keys = coeffToMask.Keys.ToList();
            foreach (var coeff in keys)
            {
                // Skip if this mask is zero.
                var mask = coeffToMask[coeff];
                if (mask == 0)
                    continue;

                // Try to remove a simple sum.
                var sumCoeff = TryRemoveNegatedDoubleSum(coeff, coeffToMask);
                if (sumCoeff != null)
                {
                    if (variableCoefficient == null)
                        variableCoefficient = sumCoeff;
                    else
                        variableCoefficient = variableCoefficient.Value + sumCoeff;
                    continue;
                }

                // Check if we can just remove the bit mask.
                // Example: `(2*(9223372036854775807&y))` is just `2*y`.
                bool canRemoveBitmask = CanRemoveMask(coeff, mask);
                if (canRemoveBitmask)
                {
                    coeffToMask[coeff] = 0;
                    if (variableCoefficient == null)
                        variableCoefficient = coeff;
                    else
                        variableCoefficient = variableCoefficient.Value + coeff;
                }
            }

            // Try to express a linear combination of say m1*(a&c1)+m2*(a&c2) as m1*(a&c1) 
            var result = TryExpressAsSingleBitwiseSum(keys, coeffToMask);
            if (variableCoefficient == null)
                variableCoefficient = result == 0 ? null : result;
            else
                variableCoefficient = variableCoefficient.Value + result;
            return variableCoefficient;
        }

        // Try to express a linear combination of say m1*(a&c1)+m2*(a&c2) as m1*(a)+m2*(a&c1)
        // E.g. (7*(1111&x)) + (2*(9223372036854774696&x)) => 5*(1111&x) + 2*x
        public ApInt TryExpressAsSingleBitwiseSum(List<ApInt> keys, Dictionary<ApInt, ApInt> coeffToMask)
        {
            ApInt result = 0;
            for (int a = 0; a < keys.Count; a++)
            {
                for (int b = 0; b < keys.Count; b++)
                {
                    if (a == b)
                        continue;

                    var coeffA = keys[a];
                    var maskA = coeffToMask[coeffA];
                    if (coeffA == 0 || maskA == 0)
                        continue;
                    var coeffB = keys[b];
                    var maskB = coeffToMask[coeffB];
                    if (coeffB == 0 || maskB == 0)
                        continue;

                    // Subtract the second coefficient from the first coefficient.
                    var sum1 = moduloMask & (coeffA - coeffB);

                    // Check if we can change the first coefficient and remove the second bit mask.
                    bool canRewrite = CanChangeSumMaskAndCoefficients(
                        // Old coefficients and masks
                        coeffA, coeffB, maskA, maskB,
                        // New coefficients and masks.
                        sum1, coeffB, maskA, ulong.MaxValue
                    );

                    // Skip if we can't.
                    if (!canRewrite)
                        continue;

                    // Remove both of the old terms.
                    coeffToMask[coeffA] = 0;
                    coeffToMask[coeffB] = 0;

                    // Add an entry for the new term. 5*(1111&x) in our example.
                    coeffToMask[sum1] = maskA;

                    // Update the coefficient for the whole term.
                    result = moduloMask & (result + coeffB);
                }
            }
            return result;
        }

        // Check if we can rewrite the two terms addition using a new set of coefficients and masks.
        public bool CanChangeSumMaskAndCoefficients(
            // Old coefficients and masks
            ulong oldCoeffA, ulong oldCoeffB, ulong oldMaskA, ulong oldMaskB,
            // New coefficients and masks
            ulong newCoeffA, ulong newCoeffB, ulong newMaskA, ulong newMaskB
            )
        {
            for (ushort i = 0; i < 64; i++)
            {
                // Shift 1 into the current bit index.
                var value = (ulong)1 << i;

                var op1 = moduloMask & (oldCoeffA * (value & oldMaskA) + oldCoeffB * (value & oldMaskB));
                var op2 = moduloMask & (newCoeffA * (value & newMaskA) + newCoeffB * (value & newMaskB));
                if (op1 != op2)
                    return false;
            }

            return true;
        }

        // Take for example `(2*(23432324&a) + (-23432325&a)` and rewrite it as a+(23432324&a)
        public ApInt? TryRemoveNegatedDoubleSum(ApInt coeff, Dictionary<ApInt, ApInt> coeffToMask)
        {
            // Skip if no entry exists with a coefficient that is double ours.
            var doubleCoeff = moduloMask & (2 * coeff);
            if (!coeffToMask.TryGetValue(doubleCoeff, out var otherMask))
                return null;

            // If this bitmask is not a negation of the other mask, we cannot extract out a whole variable.
            // In the future we may need other heuristics to isolate out whole variables.
            var thisMask = coeffToMask[coeff];
            if ((moduloMask & ~thisMask) != otherMask)
                return null;

            // Otherwise we've found a match.
            // First zero out the entry for the double coefficient.
            coeffToMask[doubleCoeff] = 0;
            // Swap out the bit mask. 
            coeffToMask[coeff] = otherMask;

            return coeff;
        }

        // TODO: When there is flexibility in the bitmask, consider the signed/unsigned representation and pick the 
        // most readable version.

        public ApInt FindMinimalMask(ApInt coeff, ApInt oldMask)
        {
            return oldMask;
        }

        public ApInt FindUndemandedCoeffBits(ApInt coeff, ApInt mask, ApInt offset)
        {
            ApInt undemanded = 0;
            for (ushort i = 0; i < bitSize; i++)
            {
                // Skip this bit if it's not set in the coefficient.
                var bitMask = (ApInt)1 << i;

                var negated = coeff & ~bitMask;
                if ((coeff & bitMask) == 0) // If this bit was not set, instead set it. Effectively we are inverting the bit.
                    negated = coeff | bitMask;

                if (!CanChangeCoeffToSlow(coeff, negated, mask, offset))
                    continue;

                // If we can change this bit, mark it as undemanded.
                undemanded |= bitMask;
            }

            return coeff;
        }


        public ApInt FindMinimalCoeff(ApInt coeff, ApInt mask, ApInt offset)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Skip this bit if it's not set in the coefficient.
                var bitMask = (ApInt)1 << i;

                if ((coeff & bitMask) == 0)
                    continue;

                var negated = coeff & ~bitMask;
                if (!CanChangeCoefficientTo(coeff, negated, mask))
                    continue;
                coeff = negated;
            }

            return coeff;
        }

        public ApInt FindMinimalCoeffSlow(ApInt coeff, ApInt mask, ApInt offset)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Skip this bit if it's not set in the coefficient.
                var bitMask = (ApInt)1 << i;

                if ((coeff & bitMask) == 0)
                    continue;

                var negated = coeff & ~bitMask;
                if (!CanChangeCoeffToSlow(coeff, negated, mask, offset))
                    continue;
                coeff = negated;
            }

            return coeff;
        }

        public ApInt FindMaximalCoeff(ApInt coeff, ApInt mask)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Skip this bit if it's not set in the coefficient.
                var bitMask = (ApInt)1 << i;

                if ((coeff & bitMask) != 0)
                    continue;

                var negated = coeff | bitMask;
                if (!CanChangeCoefficientTo(coeff, negated, mask))
                    continue;
                coeff = negated;
            }

            return coeff;
        }

        public ApInt FindMinimalMask2(ApInt coeff, ApInt mask, ApInt offset = 0)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Skip this bit if it's not set in the coefficient.
                var bitMask = (ApInt)1 << i;
                if ((mask & bitMask) == 0)
                    continue;

                var negated = mask & ~bitMask;
                if (!CanChangeMaskToSlow(coeff, mask, negated, offset))
                    continue;
                mask = negated;
            }

            return mask;
        }

        public ulong MinimizeCoeffOptimal(ulong coeff, ushort bitIdx)
        {
            return coeff & (moduloMask >> bitIdx);
            // If the bit idx is 0, then the all bits are dead.
            // If the bit idx is 1, then the upper 63 are dead
            // (64 - 1)
            // 

            // Say our bit idx is 16... the upper 48 bits(140737488355328) are bit
        }

        public ulong MinimizeCoeff(ulong coeff, ulong inMask)
        {
           return UnmanagedAnalyses.MinimizeCoeff(coeff, inMask, moduloMask);
        }

        public bool CanChangeCoefficientTo(ulong oldCoeff, ulong newCoeff, ulong andMask)
        {
            return UnmanagedAnalyses.CanChangeCoeff(oldCoeff, newCoeff, andMask, moduloMask);
        }

        public bool CanRemoveMask(ApInt coeff, ApInt andMask)
        {
            return UnmanagedAnalyses.CanRemoveMask(coeff, andMask, moduloMask);
        }

        // Returns true if the expression is equivalent with the bitmask removed.
        public bool CanChangeMaskTo(ApInt coeff, ApInt oldMask, ApInt newMask)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Shift 1 into the current bit index.
                var value = (ApInt)1 << i;

                var op1 = moduloMask & (coeff * (value & oldMask));
                var op2 = moduloMask & (coeff * (value & newMask));
                if (op1 != op2)
                    return false;
            }

            return true;
        }

        // Returns true if the expression is equivalent with the bitmask removed.
        public bool CanChangeMaskToSlow(ApInt coeff, ApInt oldMask, ApInt newMask, ApInt offset = 0)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Shift 1 into the current bit index.
                var value = (ApInt)1 << i;

                var op1 = moduloMask & ((coeff * (value & oldMask) + offset));
                var op2 = moduloMask & ((coeff * (value & newMask)) + offset);
                if (op1 != op2)
                    return false;
            }

            return true;
        }

        // Returns true if the expression is equivalent with the bitmask removed.
        public bool CanChangeCoeffToSlow(ApInt oldCoeff, ApInt newCoeff, ApInt mask, ApInt offset = 0)
        {
            for (ushort i = 0; i < bitSize; i++)
            {
                // Shift 1 into the current bit index.
                var value = (ApInt)1 << i;

                var op1 = moduloMask & ((oldCoeff * (value & mask)) + offset);
                var op2 = moduloMask & ((newCoeff * (value & mask)) + offset);
                if (op1 != op2)
                    return false;
            }

            return true;
        }
    }
}
