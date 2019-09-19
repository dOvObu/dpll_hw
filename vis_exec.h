#ifndef VIS_EXEC_H
#define VIS_EXEC_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include "parser_nodes.h"
#include "vis_base.h"
#include "vis_tseitin.h"
#include "vis_dpll.h"
#include "vis_printer.h"
#include "vis_walker.h"
#include "vis_equality.h"

using namespace std;

struct exec : public vis_walker {
   ostream* out = &cout;
   set<set<expr*>> CNF;
   map<string, int> M;
   bool res = false;
   bool is_call  = false;
   bool _tseitin = false;
   bool _dpll    = false;
   bool _SAT_    = false;

   void exec_tseitin(vec<expr*>& args)
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
         set<expr*> df;

         simplify_cnf();
         if (type == tok::_not) {
            df.emplace(args.back());
         } else if (type == tok::_and) {
            _and* node = (_and*)args.back();
            df.emplace(node->l);
            CNF.emplace(df);
            df.clear();
            df.emplace(node->r);
         } else if (type == tok::_or) {
            _or* node = (_or*)args.back();
            df.emplace(node->l);
            df.emplace(node->r);
         }
         CNF.emplace(df);

         simplify_cnf(false);
         show_cnf();
      }
   }


   void simplify_cnf(bool rm_garbage = true)
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
               _not* nt  = (_not*)lit;
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

   void enter(therefore  *p) override
   {
      if (p->l != nullptr) {
         p->l->enter(this);
      }
      if (p->r != nullptr) {
         p->r->enter(this);
      }
   }

   void enter(_or        *p) override
   {
      if (p->l != nullptr) {
         p->l->enter(this);
      }
      if (p->r != nullptr) {
         p->r->enter(this);
      }
   }

   void enter(_and       *p) override
   {
      if (p->l != nullptr) {
         p->l->enter(this);
      }
      if (p->r != nullptr) {
         p->r->enter(this);
      }
   }

   void enter(_not       *p) override
   {
      if (p->r != nullptr) {
         p->r->enter(this);
         res = !res;
      }
   }

   void enter(var        *p) override
   {
      if (is_call) {
         _tseitin = (p->id == "Tseitin");
         _dpll    = (p->id == "DPLL"   );
      } else {
         if (M.count(p->id)) {
            res = M[p->id];
         }
      }
   }

   void enter(call       *p) override
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
         /*
         CNF = set<set<expr*>>{
            {new _not(new var("a")),          new var("b") ,          new var("c") },
            {         new var("a") ,          new var("c") ,          new var("d") },
            {         new var("a") ,          new var("c") , new _not(new var("d"))},
            {         new var("a") , new _not(new var("c")),          new var("d") },
            {         new var("a") , new _not(new var("c")), new _not(new var("d"))},
            {new _not(new var("b")), new _not(new var("c")),          new var("d") },
            {new _not(new var("a")),          new var("b") , new _not(new var("c"))},
            {new _not(new var("a")), new _not(new var("b")),          new var("c") }
         };
         //*/
         vis_dpll expert;
         expert.exec(CNF);
      }
   }

private:

   void show_cnf()
   {
      *out << "\n\n ==---- CNF ----== \n";
      bool first1 = true, first2 = true;
      printer writer;

      for (auto& it : CNF) {

         if (!first1) {
            *out << " & ";
         }
         else {
            first1 = false;
         }

         *out << '(';

         if (!it.empty()) {
            for (auto& jt : it) {

               if (!first2) {
                  *out << " | ";
               }
               else {
                  first2 = false;
               }

               if (jt != nullptr) {
                  jt->enter(&writer);
               }
               else {
                  *out << "nil";
               }
            }
         }
         first2 = true;
         *out << ')';
      }
      *out << "\n ==------ ------== \n";
   }
};

#endif // #define EXEC
