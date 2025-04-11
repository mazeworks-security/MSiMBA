#pragma once

namespace symbsat {

  template <typename Monom> class Lex {
  protected:
      static bool lex(const Monom &a, const Monom &b) {
        auto n = (a.mVars ^ b.mVars).to_ulong();

        if (!n) {
          return false;
        }

        //n &= -n;
        n &= (~n + 1);

        int pos = 0;
        while (n >>= 1) {
          pos++;
        }

        return b.mVars[pos];
      }
      static bool revlex(const Monom &a, const Monom &b) {
        auto n = (a.mVars ^ b.mVars).to_ulong();

        if (!n) {
          return false;
        }

        int pos = 0;
        while (n >>= 1) {
          pos++;
        }

        return b.mVars[pos];
      }
  public:
      static bool lt(const Monom &a, const Monom &b) {
          return Lex::lex(a, b);
      }
  };

  template <typename Monom>
  class DegLex: public Lex<Monom> {
  public:
      static bool lt(const Monom &a, const Monom &b) {
          if (a.degree() < b.degree()) {
            return true;
          }
          if (a.degree() > b.degree()) {
            return false;
          }
          return Lex<Monom>::lex(a, b);
      }
  };

  template <typename Monom>
  class DegRevLex: public Lex<Monom> {
  public:
      static bool lt(const Monom &a, const Monom &b) {
          return a.mVars.to_ulong() < b.mVars.to_ulong();
      }
  };

}
