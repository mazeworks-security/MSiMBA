#pragma once

#include <iostream>

#include <algorithm>
#include <initializer_list>
#include <unordered_set>
#include <vector>
#include <sstream>

namespace symbsat {

template <typename MonomT> class Poly {

public:
  std::vector<MonomT> mMonoms;
  typedef MonomT MonomType;

  Poly() = default;
  ~Poly() = default;
  explicit Poly(MonomT &m) : mMonoms{m} {}
  explicit Poly(std::initializer_list<MonomT> monoms) : mMonoms(monoms) {
    std::sort(mMonoms.begin(), mMonoms.end());
  }
  explicit Poly(std::vector<MonomT> &monoms) : mMonoms(monoms) {
    std::sort(mMonoms.begin(), mMonoms.end());
  }
  Poly(const Poly &other) : mMonoms(other.mMonoms) {}
  Poly &operator=(const Poly &other) {
    if (this != &other) {
      mMonoms = other.mMonoms;
    }
    return *this;
  }
  Poly(const Poly &&other) noexcept : mMonoms(std::move(other.mMonoms)) {}
  Poly &operator=(Poly &&other) noexcept {
    if (this != &other) {
      mMonoms = std::move(other.mMonoms);
    }
    return *this;
  }

  explicit Poly(int var) {
    MonomT m(var);
    mMonoms.push_back(m);
  }

  bool isZero() const { return mMonoms.empty(); }
  bool isOne() const { return mMonoms.size() == 1 && mMonoms[0].isOne(); }
  void setZero() { mMonoms.clear(); };
  void setOne() {
    MonomT m_one;
    m_one.setOne();
    mMonoms.clear();
    mMonoms.push_back(m_one);
  }

  MonomT lm() const {
    if (isZero()) {
      return MonomT();
    }
    return mMonoms.back();
  };

  bool operator==(const Poly &other) const {
    return std::equal(mMonoms.begin(), mMonoms.end(), other.mMonoms.begin());
  }

  Poly &operator+=(const Poly &b) {
    std::vector<MonomT> monoms;
    monoms.reserve(mMonoms.size() + b.mMonoms.size());

    std::set_symmetric_difference(mMonoms.cbegin(), mMonoms.cend(),
                                  b.mMonoms.cbegin(), b.mMonoms.cend(),
                                  std::back_inserter(monoms));

    mMonoms = std::move(monoms);
    return *this;
  }
  friend Poly operator+(Poly lhs, const Poly &rhs) {
    lhs += rhs;
    return lhs;
  }

  // TODO Add more tests
  Poly &operator+=(const MonomT &b) {
    if (isZero()) {
      mMonoms.push_back(b);
      return *this;
    }
    for (auto it = mMonoms.begin(); it != mMonoms.end(); it++ ) {
      if (*it == b) {
        mMonoms.erase(it);
        break;
      }
      if (*it < b) {
        continue;
      }
      if (!(*it < b)) {
        mMonoms.insert(it, b);
        break;
      }
    }
    return *this;
  }
  friend Poly operator+(Poly lhs, const MonomT &rhs) {
    lhs += rhs;
    return lhs;
  }

  Poly &operator*=(const Poly &b) {
    if (isZero() || b.isZero()) {
      mMonoms.clear();
      return *this;
    }
    std::unordered_set<MonomT, typename MonomT::hash> monoms_set;

    for (auto &m1 : mMonoms) {
      for (auto &m2 : b.mMonoms) {
        MonomT tmp(m1 * m2);
        if (monoms_set.find(tmp) == monoms_set.end()) {
          monoms_set.insert(tmp);
        } else {
          monoms_set.erase(tmp);
        }
      }
    }

    std::vector<MonomT> monoms_vec(std::begin(monoms_set),
                                   std::end(monoms_set));

    mMonoms = std::move(monoms_vec);
    std::sort(mMonoms.begin(), mMonoms.end());

    return *this;
  }
  friend Poly operator*(Poly lhs, const Poly &rhs) {
    lhs *= rhs;
    return lhs;
  }

  Poly &operator*=(const MonomT &b) {
    if (isZero() || b.isZero()) {
      mMonoms.clear();
      return *this;
    }

    std::vector<MonomT> monoms;
    monoms.reserve(mMonoms.size());

    for (auto &m: mMonoms) {
      monoms.push_back(m * b);
    }

    // remove duplicates
    std::sort(monoms.begin(), monoms.end());

    for (auto it = monoms.begin(); std::next(it) != monoms.end(); ) {
      if (*it == *std::next(it)) {
        monoms.erase(it);
        if (monoms.erase(it) == monoms.end()) {
          break;
        }
      } else {
        ++it;
      }
    }

    mMonoms = std::move(monoms);

    return *this;
  }
  friend Poly operator*(Poly lhs, const MonomT &rhs) {
    lhs *= rhs;
    return lhs;
  }

  friend std::ostream &operator<<(std::ostream &out, const Poly &a) {
    if (a.isZero()) {
      out << "0";
    } else if (a.isOne()) {
      out << "1";
    } else {
      for (auto it = a.mMonoms.rbegin(); it != a.mMonoms.rend(); ){
        if (std::next(it) == a.mMonoms.rend()) {
          out << *it;
        } else {
          out << *it << " ";
        }
        it++;
      }
    }
    return out;
  }
  std::string toStr() const {
    std::ostringstream s;
    s << *this;
    return s.str();
  }
};

}; // namespace symbsat
