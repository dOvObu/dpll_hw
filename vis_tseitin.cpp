#include "vis_tseitin.h"


void tseitin::change_therefore_to_or(expr*& phi)
{
   _or* res = nullptr;
   expr* sb = nullptr;
   exprPool.push_back(expr_ptr(res = new _or(nullptr, nullptr)));
   res->l = get_contr(((therefore*)phi)->l, true);
   res->r = ((therefore*)phi)->r;
   rmExprFromPool(phi);
   phi = res;
}


void tseitin::change_and_to_literal(expr*& phi)
{
   expr*& l1 = ((_and*)phi)->l;
   expr*& l2 = ((_and*)phi)->r;

   var* new_var = nullptr;

   exprPool.push_back(expr_ptr(new_var = new var("p" + std::to_string(CNF.size() / 3 + 1))));
   expr* sb    = get_contr(new_var);
   expr* sb_l1 = get_contr(l1);
   expr* sb_l2 = get_contr(l2);

   set<expr*> C;
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


void tseitin::change_or_to_literal(expr*& phi)
{
   var* new_var = nullptr;
   exprPool.push_back(expr_ptr(new_var = new var("p" + std::to_string(CNF.size() / 3 + 1))));
   expr*& l1 = ((_and*)phi)->l;
   expr*& l2 = ((_and*)phi)->r;

   expr* sb    = get_contr(new_var);
   expr* sb_l1 = get_contr(l1);
   expr* sb_l2 = get_contr(l2);

   set<expr*> C;
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


expr* tseitin::get_contr(expr* p, bool need_rm_prew)
{
   expr* res;
   if (expert.get_type_of(p) == tok::_not) {
      res = ((_not*)p)->r;
      if (need_rm_prew) rmExprFromPool(p);
   } else {
      _not* q;
      exprPool.push_back(expr_ptr(q = new _not(nullptr)));
      q->r = p;
      res = q;
   }
   return res;
}
// ==----- OVERRIDES -----==
void tseitin::enter(therefore* p)
{
   thrfr_to_or = true;
}


void tseitin::enter(_or* p)
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


void tseitin::enter(_and* p)
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


void tseitin::enter(_not* p)
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
