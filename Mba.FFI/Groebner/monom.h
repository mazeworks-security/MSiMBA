#pragma once

#include <bitset>
#include <vector>
#include <sstream>

#include "order.h"

namespace symbsat {

template <size_t N, template <typename> class Order = Lex>
class Monom {

  friend Lex<Monom>;
  friend DegLex<Monom>;
  friend DegRevLex<Monom>;

  bool is_one = false;

public:
  std::bitset<N> mVars;
  Monom() =default;
  ~Monom() = default;
  Monom(size_t var) { mVars.set(var); }
  Monom(const Monom &m): is_one(m.is_one), mVars(m.mVars) {}
  Monom(const Monom &&m) noexcept : is_one(std::move(m.is_one)), mVars(std::move(m.mVars)) {}
  Monom &operator=(const Monom &other) {
    if (this != &other) {
      mVars = other.mVars;
      is_one = other.is_one;
    }
    return *this;
  }
  Monom &operator=(Monom &&other) noexcept {
    if (this != &other) {
      mVars = std::move(other.mVars);
      is_one = std::move(other.is_one);
    }
    return *this;
  }

  inline bool isZero() const { return !is_one && mVars.none(); }
  inline bool isOne() const { return is_one; }
  inline void setZero() {
    mVars.reset();
    is_one = false;
  }
  inline void setOne() {
    mVars.reset();
    is_one = true;
  }

  void setVar(size_t var) {
      mVars.set(var);
  }
  inline size_t degree() const {
    return mVars.count();
  }
  std::vector<int> getVars() const {
    std::vector<int> vars;

    size_t msize = mVars.size();
    for (size_t i = 0; i < msize; ++i) {
      if (mVars[i]) {
        vars.push_back(i);
      }
    }
    return vars;
  }

  bool isdivisible(const Monom &other) const {
    if (other.isOne()) {
      return true;
    } else if (this->isOne()) {
      return false;
    } else {
      return mVars == (mVars | other.mVars);
    }
  }
  bool isrelativelyprime(const Monom &other) const {
    if (mVars == other.mVars) {
      return true;
    } else if (this->isOne()) {
      return true;
    } else {
      auto lcm = mVars | other.mVars;
      return (lcm ^ mVars) == other.mVars;
    }
  }

  // bool operator<(const Monom &a) const {
  //   size_t msize = mVars.size();
  //   for (size_t i = 0; i < msize; ++i) {
  //     if (mVars[i] ^ a.mVars[i]) {
  //       return a.mVars[i];
  //       // return mVars[i];
  //     }
  //   }
  //   return false;
  // }

  bool operator<(const Monom &a) const {
    return Order<Monom>::lt(*this, a);
  }
  bool operator==(const Monom &other) const {
    if (isZero() && other.isZero())
      return true;
    if (isOne() && other.isOne())
      return true;
    if (isOne() && other.isZero())
      return false;
    if (isZero() && other.isOne())
      return false;
    return mVars == other.mVars;
  }

  Monom &operator*=(const Monom &b) {
    if (*this == b) {
      return *this;
    }
    if (this->isOne()) {
      is_one = false;
      mVars = b.mVars;
      return *this;
    }
    if (b.isOne()) {
      return *this;
    }
    if (this->isZero() || b.isZero()) {
      setZero();
      return *this;
    }
    mVars |= b.mVars;
    return *this;
  }
  friend Monom operator*(Monom lhs, const Monom &rhs) {
    lhs *= rhs;
    return lhs;
  }

  Monom &operator/=(const Monom &b) {
    if (b.isOne()) {
      return *this;
    } else if (*this == b) {
      // return 1
      setOne();
      return *this;
    } else if (!isdivisible(b)) {
      // return 0
      setZero();
      return *this;
    } else {
      mVars ^= b.mVars;
      return *this;
    }
  }
  friend Monom operator/(Monom lhs, const Monom &rhs) {
    lhs /= rhs;
    return lhs;
  }

  struct hash {
    size_t operator()(const Monom &x) const {
      return std::hash<std::bitset<N>>()(x.mVars);
    }
  };

  std::string toStr() const {
    std::ostringstream s;
    s << *this;
    return s.str();
  }

  friend std::ostream &operator<<(std::ostream &out, const Monom &a) {
    if (a.isZero()) {
      out << "0";
    } else if (a.isOne()) {
      out << "1";
    } else {
      out << "[ ";
      for (size_t i = 0; i < a.mVars.size(); ++i) {
        if (a.mVars[i]) {
          out << i << " ";
        }
      }
      out << "]";
    }
    return out;
  }

};

}; // namespace
