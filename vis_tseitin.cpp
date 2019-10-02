#include "vis_tseitin.h"


void VIS_TSEITIN::change_therefore_to_or(EXPR*& phi)
{
   OR* res = nullptr;
   EXPR* sb = nullptr;
   exprPool.push_back(EXPR_PTR(res = new OR(nullptr, nullptr)));
   res->l = get_contr(((THEREFORE*)phi)->l, true);
   res->r = ((THEREFORE*)phi)->r;
   rmExprFromPool(phi);
   phi = res;
}


void VIS_TSEITIN::change_and_to_literal(EXPR*& phi)
{
   EXPR*& l1 = ((AND*)phi)->l;
   EXPR*& l2 = ((AND*)phi)->r;

   VAR* new_var = nullptr;

   exprPool.push_back(EXPR_PTR(new_var = new VAR("p" + std::to_string(CNF.size() / 3 + 1))));
   EXPR* sb    = get_contr(new_var);
   EXPR* sb_l1 = get_contr(l1);
   EXPR* sb_l2 = get_contr(l2);

   set<EXPR*> C;
   C.insert(sb);
   C.insert(l1);
   CNF.emplace(C);

   C.clear();
   C.insert(sb);
   C.insert(l2);
   CNF.emplace(C);

   C.clear();
   C.insert(sb_l1);
   C.insert(sb_l2);
   C.insert(new_var);
   CNF.emplace(C);

   rmExprFromPool(phi);
   phi = new_var;
}


void VIS_TSEITIN::change_or_to_literal(EXPR*& phi)
{
   VAR* new_var = nullptr;
   exprPool.push_back(EXPR_PTR(new_var = new VAR("p" + std::to_string(CNF.size() / 3 + 1))));
   EXPR*& l1 = ((AND*)phi)->l;
   EXPR*& l2 = ((AND*)phi)->r;

   EXPR* sb    = get_contr(new_var);
   EXPR* sb_l1 = get_contr(l1);
   EXPR* sb_l2 = get_contr(l2);

   set<EXPR*> C;
   C.insert(sb);
   C.insert(l1);
   C.insert(l2);
   CNF.emplace(C);

   C.clear();
   C.insert(sb_l1);
   C.insert(new_var);
   CNF.emplace(C);

   C.clear();
   C.insert(sb_l2);
   C.insert(new_var);
   CNF.emplace(C);

   rmExprFromPool(phi);
   phi = new_var;
}


EXPR* VIS_TSEITIN::get_contr(EXPR* p, bool need_rm_prew)
{
   EXPR* res;
   if (expert.get_type_of(p) == TOK::_not) {
      res = ((NOT*)p)->r;
      if (need_rm_prew) rmExprFromPool(p);
   } else {
      NOT* q;
      exprPool.push_back(EXPR_PTR(q = new NOT(nullptr)));
      q->r = p;
      res = q;
   }
   return res;
}
// ==----- OVERRIDES -----==
void VIS_TSEITIN::enter(THEREFORE* p)
{
   thrfr_to_or = true;
}


void VIS_TSEITIN::enter(OR* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);

      if (thrfr_to_or) {
         thrfr_to_or = false;
         change_therefore_to_or(p->l);
         p->l->enter(this);
      }

      if (and_to_lit) {
         and_to_lit = false;
         change_and_to_literal(p->l);
      } else if (or_to_lit) {
         or_to_lit = false;
         change_or_to_literal(p->l);
      }
   }

   if (p->r != nullptr) {
      p->r->enter(this);

      if (thrfr_to_or) {
         thrfr_to_or = false;
         change_therefore_to_or(p->r);
         p->r->enter(this);
      }

      if (and_to_lit) {
         and_to_lit = false;
         change_and_to_literal(p->r);
      } else if (or_to_lit) {
         or_to_lit = false;
         change_or_to_literal(p->r);
      }
   }
   or_to_lit = true;
}


void VIS_TSEITIN::enter(AND* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);

      if (thrfr_to_or) {
         thrfr_to_or = false;
         change_therefore_to_or(p->l);
         p->l->enter(this);
      }

      if (and_to_lit) {
         and_to_lit = false;
         change_and_to_literal(p->l);
      } else if (or_to_lit) {
         or_to_lit = false;
         change_or_to_literal(p->l);
      }
   }

   if (p->r != nullptr) {
      p->r->enter(this);

      if (thrfr_to_or) {
         thrfr_to_or = false;
         change_therefore_to_or(p->r);
         p->r->enter(this);
      }

      if (and_to_lit) {
         and_to_lit = false;
         change_and_to_literal(p->r);
      } else if (or_to_lit) {
         or_to_lit = false;
         change_or_to_literal(p->r);
      }
   }
   and_to_lit = true;
}


void VIS_TSEITIN::enter(NOT* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);

      if (thrfr_to_or) {
         thrfr_to_or = false;
         change_therefore_to_or(p->r);
         p->r->enter(this);
      }

      if (and_to_lit) {
         and_to_lit = false;
         change_and_to_literal(p->r);
      } else if (or_to_lit) {
         or_to_lit = false;
         change_or_to_literal(p->r);
      }
   }
}
