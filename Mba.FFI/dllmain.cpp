#include <array>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <functional>
#include <cstdio>
#include <bit>
#include <chrono>

#define FFI_EXPORT extern "C" __declspec(dllexport)

typedef uint64_t u64_4 __attribute__((ext_vector_type(4)));
static uint64_t ReduceOr(u64_4 a) {
	return a[0] | a[1] | a[2] | a[3];
}

FFI_EXPORT bool CanChangeCoeff(uint64_t oldCoeff, uint64_t newCoeff, uint64_t inMask, uint64_t outMask) {
	u64_4 v_value = {
		1 << 0,
		1 << 1,
		1 << 2,
		1 << 3,
	};

	auto mask_iterator = inMask;
	while (mask_iterator) {
		auto pos = std::countr_zero(mask_iterator);
		mask_iterator &= ~(0b1111ull << pos);

		auto value = v_value << pos;
		auto op1 = oldCoeff * (value & inMask);
		auto op2 = newCoeff * (value & inMask);
		uint64_t nope = ReduceOr(op1 ^ op2);
		if (nope & outMask) {
			return false;
		}
	}
	return true;
}

// Check if we can remove the bit mask
FFI_EXPORT bool CanRemoveMask(uint64_t coeff, uint64_t and_mask, uint64_t out_mask) {
	size_t iter = out_mask;
	while (iter)
	{
		auto pos = std::countr_zero(iter);
		iter &= ~(1ull << pos);

		// Shift 1 into the current bit index.
		auto value = (uint64_t)1 << pos;

		uint64_t op1 = out_mask & (coeff * (value & and_mask));
		uint64_t op2 = out_mask & (coeff * (value & out_mask));
		if (op1 != op2)
			return false;
	}

	return true;
}

struct CoeffWithMask
{
	uint64_t coeff;
	uint64_t mask;

	CoeffWithMask(uint64_t coeff, uint64_t mask)
	{
		this->coeff = coeff;
		this->mask = mask;
	}
};

int Compare(CoeffWithMask int_a, CoeffWithMask int_b)
{
	if (int_a.coeff == int_b.coeff) return 0;
	else if (int_a.coeff > int_b.coeff) return -1;
	else return 1;
}

FFI_EXPORT void SimplifyDisjointSumMultiply(CoeffWithMask* coeffsWithMasks, int len, uint64_t outMask)
{
	std::sort(coeffsWithMasks, coeffsWithMasks + len, Compare);
	for (int a = 0; a < len; a++)
	{
		if (coeffsWithMasks[a].mask == 0)
			continue;

		for (int b = a + 1; b < len; b++)
		{
			// Break out if the first mask was set to zero.
			auto op1 = coeffsWithMasks[a];
			if (op1.mask == 0)
				break;
			// Skip if the current mask is zero.
			auto op2 = coeffsWithMasks[b];
			if (op2.mask == 0)
				continue;
			// The bit masks must be disjoint for this property to hold.
			if ((op1.mask & op2.mask) != 0)
				continue;

			// We know that the bit masks are disjoint.
			if (CanChangeCoeff(op2.coeff, op1.coeff, op2.mask, outMask))
			{
				auto ored = op1.mask | op2.mask;
				coeffsWithMasks[b] = CoeffWithMask(op2.coeff, 0);
				coeffsWithMasks[a] = CoeffWithMask(op1.coeff, ored);
			}

			// We know that the bit masks are disjoint.
			else if (CanChangeCoeff(op1.coeff, op2.coeff, op1.mask, outMask))
			{
				auto ored = op2.mask | op1.mask;
				coeffsWithMasks[a] = CoeffWithMask(op1.coeff, 0);
				coeffsWithMasks[b] = CoeffWithMask(op2.coeff, ored);

				// Break out of the entire loop since A's coefficient was changed.
				break;
			}
		}
	}

	return;
}