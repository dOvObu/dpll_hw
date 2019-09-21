#include "vis_exec.h"


void exec::exec_tseitin(vec<expr*>& args)
{
   if (!args.empty()) {

      tseitin f;
      vis_expr_type expert;

      for (auto& it : args) {
         if (it != nullptr) {
            if (expert.get_type_of(it) == tok::therefore) {
               f.change_therefore_to_or(it);
            }
            it->enter(&f);
         }
      }
      CNF = f.CNF;

      tok type = expert.get_type_of(args.back());
      set<expr*> c;
      simplify_cnf();

      if (type == tok::_not) {
         c.emplace(args.back());

      } else if (type == tok::_and) {
         _and* node = (_and*)args.back();

         c.emplace(node->l);
         CNF.emplace(c);
         c.clear();
         c.emplace(node->r);

      } else if (type == tok::_or) {
         _or* node = (_or*)args.back();

         c.emplace(node->l);
         c.emplace(node->r);
      }
      CNF.emplace(c);

      simplify_cnf(false);
      show_cnf();
   }
}


void exec::simplify_cnf(bool rm_garbage)
{
   vis_equality expert;
   _not n1(nullptr), n2(nullptr), *not_not = &n1;
   n1.r = &n2;
   set<set<expr*>> cnf;
   vec<expr*> garbage;

   for (auto& it : CNF) {
      set<expr*> df;
      for (auto& jt : it) {
         expr* lit = jt;
         while (expert.is_in(not_not, lit)) {
            _not* nt = (_not*)lit;
            _not* nnt = (_not*)nt->r;
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
void exec::enter(therefore* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void exec::enter(_or* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void exec::enter(_and* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   if (p->r != nullptr) {
      p->r->enter(this);
   }
}


void exec::enter(_not* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);
      res = !res;
   }
}


void exec::enter(var* p)
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


void exec::enter(call* p)
{
   vis_walker::enter(p);

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
      vis_dpll expert;
      expert.exec(CNF);
   }
}


void exec::show_cnf()
{
   *out << "\n\n ==---- CNF ----== \n";
   bool first1 = true, first2 = true;
   printer writer;

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
