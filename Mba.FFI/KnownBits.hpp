#include <llvm/ADT/APInt.h>
#include <llvm/Support/KnownBits.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ErrorHandling.h>
#include "common.hpp"

using namespace llvm;

struct FfiKnownBits
{
	uint32_t width;

	uint64_t zero;

	uint64_t one;

	FfiKnownBits()
	{

	}

	FfiKnownBits(KnownBits kb)
	{
		width = kb.getBitWidth();
		zero = kb.Zero.getZExtValue();
		one = kb.One.getZExtValue();
	}

	operator KnownBits()
	{
		auto zeroes = APInt(width, zero);
		auto ones = APInt(width, one);
		auto kb = KnownBits(width);
		kb.Zero = APInt(width, zero);
		kb.One = APInt(width, one);
		return kb;
	}

};

enum Opcode : unsigned char
{
	Add = 0,
	Mul = 1,
	// Skip pow
	Shl = 2,
	And = 3,
	Or = 4,
	Xor = 5,
	Neg = 6,

};

// Transfer functions
llvm::KnownBits and_(const llvm::KnownBits& LHS, const llvm::KnownBits& RHS) {
	
	auto result = LHS;
	result.One &= RHS.One;
	result.Zero |= RHS.Zero;
	return result;
}

llvm::KnownBits or_(const llvm::KnownBits& LHS, const llvm::KnownBits& RHS) {
	auto result = LHS;
	result.One |= RHS.One;
	result.Zero &= RHS.Zero;
	return result;
}

llvm::KnownBits xor_(const llvm::KnownBits& LHS, const llvm::KnownBits& RHS) {
	auto result = LHS;
	llvm::APInt KnownZeroOut =
		(LHS.Zero & RHS.Zero) | (LHS.One & RHS.One);
	result.One = (LHS.Zero & RHS.One) | (LHS.One & RHS.Zero);
	result.Zero = std::move(KnownZeroOut);
	// ^ logic copied from LLVM ValueTracking.cpp
	return result;
}

llvm::KnownBits shl(const llvm::KnownBits& LHS, const llvm::KnownBits& RHS) {
	llvm::KnownBits Result(LHS.getBitWidth());
	const auto width = Result.getBitWidth();

	auto Op0KB = LHS;
	if (RHS.isConstant()) {
		auto Val = RHS.getConstant().getLimitedValue();
		if (Val < 0 || Val >= width) {
			return Result;
		}

		Op0KB.One <<= Val;
		Op0KB.Zero <<= Val;
		Op0KB.Zero.setLowBits(Val);
		// setLowBits takes an unsigned int, so getLimitedValue is harmless
		return Op0KB;
	}

	unsigned minValue = RHS.One.getLimitedValue();
	Result.Zero.setLowBits(std::min(LHS.countMinTrailingZeros() + minValue, width));

	return Result;
}

llvm::KnownBits lshr(const llvm::KnownBits& LHS, const llvm::KnownBits& RHS) {
	llvm::KnownBits Result(LHS.getBitWidth());
	const auto width = Result.getBitWidth();

	auto Op0KB = LHS;
	if (RHS.isConstant()) {
		auto Val = RHS.getConstant().getLimitedValue();
		if (Val < 0 || Val >= width) {
			return Result;
		}
		Op0KB.One.lshrInPlace(Val);
		Op0KB.Zero.lshrInPlace(Val);
		Op0KB.Zero.setHighBits(Val);
		// setHighBits takes an unsigned int, so getLimitedValue is harmless
		return Op0KB;
	}

	unsigned minValue = RHS.One.getLimitedValue();
	Result.Zero.setHighBits(std::min(minValue + LHS.countMinLeadingZeros(), width));

	return Result;
}

FFI_EXPORT void GetAddKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(KnownBits::computeForAddSub(true, false, false, lhs, rhs));;
}

FFI_EXPORT void GetSubKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(KnownBits::computeForAddSub(false, false, false, lhs, rhs));;
}

FFI_EXPORT void GetMulKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(KnownBits::mul(lhs, rhs));
}

FFI_EXPORT void GetAndKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(and_(lhs, rhs));
}

FFI_EXPORT void GetOrKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(or_(lhs, rhs));
}

FFI_EXPORT void GetXorKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(xor_(lhs, rhs));
}

FFI_EXPORT void GetNegKnownBits(FfiKnownBits& lhs, FfiKnownBits* out)
{
	// Negation can be rewritten as (x ^ -1).
	auto apInt = llvm::APInt(lhs.width, -1);
	auto kbNegativeOne = llvm::KnownBits::makeConstant(apInt);
	*out = FfiKnownBits(xor_(lhs, kbNegativeOne));
}

FFI_EXPORT void GetShlKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(shl(lhs, rhs));
}

FFI_EXPORT void GetLshrKnownBits(FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = FfiKnownBits(lshr(lhs, rhs));
}

FFI_EXPORT void GetZextKnownBits(FfiKnownBits& lhs, uint32_t width, FfiKnownBits* out)
{
	KnownBits kb = lhs;
	*out = FfiKnownBits(kb.zext(width));
}

FFI_EXPORT void GetTruncKnownBits(FfiKnownBits& lhs, uint32_t width, FfiKnownBits* out)
{
	KnownBits kb = lhs;
	*out = FfiKnownBits(kb.trunc(width));
}

enum CmpPredicate : unsigned char
{
	Eq = 0,
	Ne = 1,
	Ugt = 2,
	Uge = 3,
	Ult = 4,
	Ule = 5,
	Sgt = 6,
	Sge = 7,
	Slt = 8,
	Sle = 9,
};


FfiKnownBits Icmp(CmpPredicate pred, const llvm::KnownBits& lhs, const llvm::KnownBits& rhs)
{
	std::optional<bool> result = std::nullopt;
	switch (pred)
	{
	case CmpPredicate::Eq:
		result = llvm::KnownBits::eq(lhs, rhs);
		break;
	case CmpPredicate::Ne:
		result = llvm::KnownBits::ne(lhs, rhs);
		break;
	case CmpPredicate::Ugt:
		result = llvm::KnownBits::ugt(lhs, rhs);
		break;
	case CmpPredicate::Uge:
		result = llvm::KnownBits::uge(lhs, rhs);
		break;
	case CmpPredicate::Ult:
		result = llvm::KnownBits::ult(lhs, rhs);
		break;
	case CmpPredicate::Ule:
		result = llvm::KnownBits::ule(lhs, rhs);
		break;
	case CmpPredicate::Sgt:
		result = llvm::KnownBits::sgt(lhs, rhs);
		break;
	case CmpPredicate::Sge:
		result = llvm::KnownBits::sge(lhs, rhs);
		break;
	case CmpPredicate::Slt:
		result = llvm::KnownBits::slt(lhs, rhs);
		break;
	case CmpPredicate::Sle:
		result = llvm::KnownBits::sle(lhs, rhs);
		break;
	}

	FfiKnownBits kb;
	kb.width = 1;
	kb.one = 0;
	kb.zero = 0;

	if (!result.has_value())
	{
		return kb;
	}

	kb.one |= (uint64_t)result.value();
	kb.zero |= (uint64_t)!result.value();
	return kb;
}

FFI_EXPORT void GetIcmpKnownBits(CmpPredicate pred, FfiKnownBits& lhs, FfiKnownBits& rhs, FfiKnownBits* out)
{
	*out = Icmp(pred, lhs, rhs);
}

FFI_EXPORT void GetSelectKnownBits(FfiKnownBits& a, FfiKnownBits& b, FfiKnownBits& c, FfiKnownBits* out)
{
	llvm::KnownBits kb0 = a;
	llvm::KnownBits kb1 = b;
	llvm::KnownBits kb2 = c;
	if (kb0.One.getBoolValue())
	{
		*out = FfiKnownBits(kb1);
		return;
	}

	if (kb0.Zero.getBoolValue())
	{
		*out = FfiKnownBits(kb2);
		return;
	}

	*out = FfiKnownBits(kb1.intersectWith(kb2));
}