#ifndef VIS_EQUALITY_H
#define VIS_EQUALITY_H
#include "parser_nodes.h"
#include "vis_walker.h"
#include "tokens.h"
#include <algorithm>

struct VIS_EXPR_TYPE : public VIS_WALKER {
   TOK get_type_of(EXPR* e)
   {
      e->enter(this);
      return type;
   }

   void enter(VAR       *p) override { type = TOK::id;        }
   void enter(NUM       *p) override { type = TOK::num_lit;   }
   void enter(STR       *p) override { type = TOK::str_lit;   }
   void enter(DIF       *p) override { type = TOK::minus;     }
   void enter(OR       *p) override { type = TOK::_or;       }
   void enter(THEREFORE *p) override { type = TOK::therefore; }
   void enter(AND      *p) override { type = TOK::_and;      }
   void enter(MORE      *p) override { type = TOK::more;      }
   void enter(LESS     *p) override { type = TOK::less;      }
   void enter(MORE_EQ   *p) override { type = TOK::more_eq;   }
   void enter(LESS_EQ   *p) override { type = TOK::less_eq;   }
   void enter(EQUAL    *p) override { type = TOK::equal;     }
   void enter(NOT      *p) override { type = TOK::_not;      }
   void enter(UN_DIF    *p) override { type = TOK::un_minus;  }
   void enter(UN_ADD    *p) override { type = TOK::un_pluss;  }
   void enter(CALL      *p) override { type = TOK::_func;     }
   void enter(IFEL      *p) override { type = TOK::_if;       }
   void enter(FOR_EACH *p) override { type = TOK::_for;      }
   void enter(RET       *p) override { type = TOK::_return;   }
   void enter(ASSERT    *p) override { type = TOK::_assert;   }
   void enter(LAM       *p) override { type = TOK::lam;       }

private:
   TOK type;
};


struct VIS_EQUALITY : public VIS_WALKER {
   
   bool is_in(EXPR* A, EXPR* B)
   {
      equal = true;
      _A_start = _A = A;
      _B = B;
      
      if (equal_types(_A, _B)) {
         _A->enter(this);
      } else {
         equal = false;
      }

      return equal;
   }


   bool equal_types(EXPR* A, EXPR* B)
   {
      return expert.get_type_of(A) == expert.get_type_of(B);
   }


   template<typename T> void check_equality_of_not_commut_bin_op(T* p)
   {
      T* q = (T*)_B;

      if (p->l == nullptr && p->r == nullptr) {
         equal = true;
         return;
      }

      equal = (p->l == nullptr || equal_types(p->l, q->l)) && (p->r == nullptr || equal_types(p->r, q->r));

      if (!equal) { return; }

      if (p->l != nullptr) {
         _B = q->l;
         p->l->enter(this);
      }

      if (!equal) return;

      if (p->r != nullptr) {
         _B = q->r;
         p->r->enter(this);
      }

      if (!equal) return;
   }


   template<typename T> void check_equality_of_commut_bin_op(T* p)
   {
      T* q = (T*)_B;

      if (p->l == nullptr && p->r == nullptr) {
         equal = true;
         return;
      }

      bool first  = (p->l == nullptr || equal_types(p->l, q->l)) && (p->r == nullptr || equal_types(p->r, q->r));
      bool second = (p->l == nullptr || equal_types(p->l, q->r)) && (p->r == nullptr || equal_types(p->r, q->l));

      equal = first || second;

      if (!equal) { return; }

      if (p->l != nullptr) {
         _B = first ? q->l : q->r;
         p->l->enter(this);
      }

      if (!equal) return;

      if (p->r != nullptr) {
         _B = first ? q->r : q->l;
         p->r->enter(this);
      }

      if (!equal) return;
   }


   template<typename T> void check_un_op_equality(T* p)
   {
      T* q = (T*)_B;

      if (p->r == nullptr) {
         equal = true;
         return;
      }

      equal = equal_types(p->r, q->r);

      if (!equal) return;

      if (p->r != nullptr) {
         _B = q->r;
         p->r->enter(this);
      }

      if (!equal) return;
   }


   void enter(DIF    *p) override { check_equality_of_not_commut_bin_op(p); }
   void enter(AND   *p) override { check_equality_of_commut_bin_op(p); }
   void enter(OR    *p) override { check_equality_of_commut_bin_op(p); }
   void enter(NOT   *p) override { check_un_op_equality(p); }
   void enter(UN_DIF *p) override { check_un_op_equality(p); }
   void enter(UN_ADD *p) override { check_un_op_equality(p); }
private:
   bool equal = false;

   EXPR* _A_start = nullptr;
   EXPR* _A       = nullptr;
   EXPR* _B       = nullptr;

   VIS_EXPR_TYPE expert;
};

#endif // VIS_EQUALITY_H