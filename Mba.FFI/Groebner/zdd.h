#pragma once

#include <iostream>
#include <stack>
#include <utility>

#include "zddcache.h"

namespace symbsat {

template <typename MonomT> class ZDD {
  struct Node {
    int mVar;
    const Node *mMul; // and
    const Node *mAdd; // xor

    Node() = delete;
    Node(const Node &n) = delete;
    Node(const Node &&n) = delete;
    Node &operator=(const Node &) = delete;
    Node &operator=(const Node &&) = delete;

    Node(bool one) {
      if (one) {
        mVar = -1;
      } else {
        mVar = -2;
      }
      mMul = nullptr;
      mAdd = nullptr;
    }
    Node(int var, const Node *mul, const Node *add)
        : mVar(var), mMul(mul), mAdd(add) {}

    inline bool isZero() const { return mVar == -2; }
    inline bool isOne() const { return mVar == -1; }

    static bool isEqual(const Node *a, const Node *b) {
      if (a->mVar != b->mVar) {
        return false;
      } else if (a->isZero() && b->isZero()) {
        return true;
      } else if (a->isOne() && b->isOne()) {
        return true;
      } else {
        return isEqual(a->mAdd, b->mAdd) && isEqual(a->mMul, b->mMul);
      }
    }
  };

  template <typename ZDDNode>
  friend class ZDDCache;

  ZDDCache<Node> cache;

  const Node *mRoot;

  //const Node *const mOne = create_node(-1, nullptr, nullptr);
  //const Node *const mZero = create_node(-2, nullptr, nullptr);
  static const Node *const mOne;
  static const Node *const mZero;

  inline const Node *create_node(int var, const Node *mul, const Node *add) {
    return cache.createNode(var, mul, add);
  }

  const Node *copy(const Node *n) {
    if (n->mVar == -2) {
      return mZero;
    } else if (n->mVar == -1) {
      return mOne;
    } else {
      return create_node(n->mVar, copy(n->mMul), copy(n->mAdd));
    }
  }

  friend std::ostream &operator<<(std::ostream &out, const Node *a) {
      if (a->isZero()) {
          out << "_zero";
      } else if (a->isOne()) {
          out << "_one";
      } else {
          out << a->mVar << " -> {" << a->mMul << "} {" << a->mAdd << "} ";
      }

      return out;
  }

  const Node *add(const Node *i, const Node *j) {
    if (i->isZero()) {
      return copy(j);
    } else if (j->isZero()) {
      return copy(i);
    } else if (ZDD::Node::isEqual(i, j)) {
      return mZero;
    } else if (i->isOne()) {
      return create_node(j->mVar, copy(j->mMul), add(j->mAdd, mOne));
    } else if (j->isOne()) {
      return create_node(i->mVar, copy(i->mMul), add(i->mAdd, mOne));
    } else {
      if (i->mVar < j->mVar) {
        return create_node(i->mVar, copy(i->mMul), add(i->mAdd, j));
      } else if (i->mVar > j->mVar) {
        return create_node(
             j->mVar, copy(j->mMul), add(i, j->mAdd));
      } else {
        auto m = add(i->mMul, j->mMul);
        auto a = add(i->mAdd, j->mAdd);

        if (m->isZero()) {
          return a;
        }

        return create_node(i->mVar, m, a);
      }
    }
  }
  const Node *mul(const Node *i, const Node *j) {
    if (i->isOne()) {
      return copy(j);
    } else if (i->isZero() || j->isZero()) {
      return mZero;
    } else if (j->isOne() || ZDD::Node::isEqual(i, j)) {
      return copy(i);
    } else {
      if (i->mVar < j->mVar) {
        auto m = mul(i->mMul, j);
        auto a = mul(i->mAdd, j);

        if (m->isZero()) {
          return a;
        }

        return create_node(i->mVar, m, a);
      } else if (i->mVar > j->mVar) {
        auto m = mul(j->mMul, i);
        auto a = mul(j->mAdd, i);

        if (m->isZero()) {
          return a;
        }

        return create_node(j->mVar, m, a);
      } else {
        auto m1 = mul(i->mAdd, j->mMul);
        auto m2 = mul(i->mMul, j->mMul);
        auto m3 = mul(i->mMul, j->mAdd);
        auto m = add(m1, add(m2, m3));

        if (m->isZero()) {
          return mul(i->mAdd, j->mAdd);
        }

        return create_node(i->mVar, m, mul(i->mAdd, j->mAdd));
      }
    }
  }

public:
  typedef MonomT MonomType;

  ZDD() {
    mRoot = mZero;
  }
  ZDD(const ZDD &z) {
    mRoot = copy(z.mRoot);
  }
  ZDD(ZDD &&z) {
    mRoot = std::exchange(z.mRoot, nullptr);
    cache = std::move(z.cache);
  }

  ZDD &operator=(const ZDD &other) {
    if (this != &other) {
      mRoot = copy(other.mRoot);
    }
    return *this;
  }
  ZDD &operator=(ZDD &&other) {
    if (this != &other) {
      mRoot = std::exchange(other.mRoot, nullptr);
      cache = std::move(other.cache);
    }
    return *this;
  }
  ~ZDD() =default;

  explicit ZDD(int var) { mRoot = create_node(var, mOne, mZero); }
  explicit ZDD(const MonomT &m) {
    if (m.isOne()) {
      setOne();
    } else if (m.isZero()) {
      setZero();
    } else {
      std::vector<int> vars = m.getVars();
      mRoot = create_node(vars[0], mOne, mZero);

      size_t vsize = vars.size();
      for (size_t i = 1; i < vsize; ++i) {
        mRoot = mul(mRoot, create_node(vars[i], mOne, mZero));
      }
    }
  }

  inline bool isZero() const { return mRoot->isZero(); }
  inline bool isOne() const { return mRoot->isOne(); }
  inline void setZero() { mRoot = mZero; }
  inline void setOne() { mRoot = mOne; }
  MonomT lm() const {
    MonomT tmp;
    if (isZero()) {
      return tmp;
    } else if (isOne()) {
      tmp.setOne();
      return tmp;
    } else {
      for (const Node *i = this->mRoot; i->mVar >= 0; i = i->mMul) {
        tmp.setVar(i->mVar);
      }
      return tmp;
    }
  }

  ZDD &operator+=(const ZDD &rhs) {
    mRoot = add(mRoot, rhs.mRoot);
    return *this;
  }
  friend ZDD operator+(ZDD lhs, const ZDD &rhs) {
    lhs += rhs;
    return lhs;
  }

  ZDD &operator+=(const MonomT &rhs) {
    ZDD rhs_zdd(rhs);
    mRoot = add(mRoot, rhs_zdd.mRoot);
    return *this;
  }
  friend ZDD operator+(ZDD lhs, const MonomT &rhs) {
    lhs += rhs;
    return lhs;
  }

  ZDD &operator*=(const ZDD &rhs) {
    mRoot = mul(mRoot, rhs.mRoot);
    return *this;
  }
  friend ZDD operator*(ZDD lhs, const ZDD &rhs) {
    lhs *= rhs;
    return lhs;
  }

  ZDD &operator*=(const MonomT &rhs) {
    ZDD rhs_zdd(rhs);
    mRoot = mul(mRoot, rhs_zdd.mRoot);
    return *this;
  }
  friend ZDD operator*(ZDD lhs, const MonomT &rhs) {
    lhs *= rhs;
    return lhs;
  }

  // TODO need to review for correctness
  bool operator==(const ZDD &rhs) const {

    if (this == &rhs) {
      return true;
    }

    MonomConstIterator it1(*this), it2(rhs);

    //while (!it1 || !it2) {
    while (!it1 && !it2) {
      if (!(it1.monom() == it2.monom())) {
        return false;
      }
      ++it1;
      ++it2;
    }

    // TODO should be used || or ^ ?
    // FIXME bitwise operator on bools
    if (!it1 ^ !it2) {
      return false;
    }

    return true;
  }

  class MonomConstIterator {
    std::vector<int> mMonom;
    std::stack<const Node *> mPath;

  public:
    explicit MonomConstIterator(const ZDD &z) {
      for (const Node *i = z.mRoot; i->mVar >= 0; i = i->mMul) {
        mPath.push(i);
        mMonom.push_back(i->mVar);
      }
    };
    ~MonomConstIterator() = default;

    const MonomT monom() const {
      MonomT tmp;
      if (mMonom.back() == -1) {
        tmp.setOne();
      } else {
        for (auto &i : mMonom) {
          tmp.setVar(i);
        }
      }
      return tmp;
    }

    operator bool() const { return mPath.empty(); }
    void operator++() {
      if (mPath.top()->isOne()) {
        mPath.pop();
        mMonom.clear();
      }
      while (!mPath.empty() && (mPath.top()->mAdd->isZero())) {
        mPath.pop();
        mMonom.pop_back();
      }

      if (!mPath.empty()) {
        const Node *i = mPath.top()->mAdd;
        mPath.pop();
        mMonom.pop_back();

        while (!i->isOne()) {
          mPath.push(i);
          mMonom.push_back(i->mVar);
          i = i->mMul;
        }
        if (mMonom.empty()) {
          mPath.push(mOne);
          mMonom.push_back(-1);
        }
      }
    }

  };

  friend std::ostream &operator<<(std::ostream &out, const ZDD &a) {
    if (a.isZero()) {
      out << "0";
    } else if (a.isOne()) {
      out << "1";
    } else {
      ZDD::MonomConstIterator it(a);
      while (!it) {
        out << it.monom() << " ";
        ++it;
      }
    }
    return out;
  }
  std::string toStr() const {
    std::ostringstream s;
    s << *this;
    return s.str();
  }
  int count_nodes() const {
    return cache.size();
  }
}; // ZDD Template

template <typename MonomT>
const typename ZDD<MonomT>::Node *const
   ZDD<MonomT>::mOne = new ZDD<MonomT>::Node(true);

template <typename MonomT>
const typename ZDD<MonomT>::Node *const
   ZDD<MonomT>::mZero = new ZDD<MonomT>::Node(false);

}; // namespace symbsat
