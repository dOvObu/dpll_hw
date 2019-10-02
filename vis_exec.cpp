#include "vis_exec.h"


void VIS_EXEC::exec_tseitin(VEC<EXPR*>& args)
{
   if (!args.empty()) {

      VIS_TSEITIN f;
      VIS_EXPR_TYPE expert;

      for (auto& it : args) {
         if (it != nullptr) {
            if (expert.get_type_of(it) == TOK::therefore) {
               f.change_therefore_to_or(it);
            }
            it->enter(&f);
         }
      }
      CNF = f.CNF;

      TOK type = expert.get_type_of(args.back());
      set<EXPR*> c;
      simplify_cnf();

      if (type == TOK::_not) {
         c.emplace(args.back());

      } else if (type == TOK::_and) {
         AND* node = (AND*)args.back();

         c.emplace(node->l);
         CNF.emplace(c);
         c.clear();
         c.emplace(node->r);

      } else if (type == TOK::_or) {
         OR* node = (OR*)args.back();

         c.emplace(node->l);
         c.emplace(node->r);
      }
      CNF.emplace(c);

      simplify_cnf(false);
      show_cnf();
   }
}


void VIS_EXEC::simplify_cnf(bool rm_garbage)
{
   VIS_EQUALITY expert;
   NOT n1(nullptr), n2(nullptr), *not_not = &n1;
   n1.r = &n2;
   set<set<EXPR*>> cnf;
   VEC<EXPR*> garbage;

   for (auto& it : CNF) {
      set<EXPR*> df;
      for (auto& jt : it) {
         EXPR* lit = jt;
         while (expert.is_in(not_not, lit)) {
            NOT* nt = (NOT*)lit;
            NOT* nnt = (NOT*)nt->r;
            lit = nnt->r;
            garbage.push_back(nt);
         }
         df.emplace(lit);
      }
      cnf.emplace(df);
   }

   CNF.clear();
   if (rm_garbage) {
      while (!garbage.empty()) {
         rmExprFromPool(garbage.back());
         garbage.pop_back();
      }
   }
   CNF = cnf;
}
// ==----- OVERRIDES -----==
void VIS_EXEC::enter(THEREFORE* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void VIS_EXEC::enter(OR* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void VIS_EXEC::enter(AND* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void VIS_EXEC::enter(NOT* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);
      res = !res;
   }
}


void VIS_EXEC::enter(VAR* p)
{
   if (is_call) {
      _tseitin = (p->id == "Tseitin");
      _dpll = (p->id == "DPLL");
   }
   else {
      if (M.count(p->id)) {
         res = M[p->id];
      }
   }
}


void VIS_EXEC::enter(CALL* p)
{
   VIS_WALKER::enter(p);

   is_call = true;

   if (p->id != nullptr) {
      p->id->enter(this);
   }

   is_call = false;

   if (_tseitin) {
      _tseitin = false;
      exec_tseitin(p->args);
   }

   if (_dpll) {
      _dpll = false;
      VIS_DPLL expert;
      expert.exec(CNF);
   }
}


void VIS_EXEC::show_cnf()
{
   *out << "\n\n ==---- CNF ----== \n";
   bool first1 = true, first2 = true;
   VIS_PRINTER writer;

   for (auto& it : CNF) {

      if (!first1) {
         *out << " & ";
      } else {
         first1 = false;
      }

      *out << '(';

      if (!it.empty()) {
         for (auto& jt : it) {

            if (!first2) {
               *out << " | ";
            } else {
               first2 = false;
            }

            if (jt != nullptr) {
               jt->enter(&writer);
            } else {
               *out << "nil";
            }
         }
      }
      first2 = true;
      *out << ')';
   }
   *out << "\n ==------ ------== \n";
}
