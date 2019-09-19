#ifndef VIS_TSEITIN_H
#define VIS_TSEITIN_H
#include <iostream>
#include <vector>
#include <string>
#include "parser_nodes.h"
#include "vis_printer.h"
#include "vis_walker.h"
#include "vis_base.h"

using namespace std;

struct tseitin : public vis_walker {
   ostream* out = &cout;
   bool thrfr_to_or    = false;
   bool and_to_lit     = false;
   bool or_to_lit      = false;
   bool not_not_invite = false;
   bool not_not_rm     = false;

   set<set<expr*>> CNF; // множество множеств литералов

   void change_therefore_to_or(expr*& tf)
   {
      _or* res = nullptr;
      _not* sb = nullptr;
      exprPool.push_back(expr_ptr(res = new _or(nullptr, nullptr)));
      exprPool.push_back(expr_ptr(sb = new _not(nullptr)));
      sb->r = ((therefore*)tf)->l;
      res->l = sb;
      res->r = ((therefore*)tf)->r;
      rmExprFromPool(tf);
      tf = res;
   }

   void change_and_to_literal(expr*& fi)
   {
      expr*& l1 = ((_and*)fi)->l;
      expr*& l2 = ((_and*)fi)->r;
      var* new_var = nullptr;
      _not* sb = nullptr, *sb_l1 = nullptr, *sb_l2 = nullptr;
      exprPool.push_back(expr_ptr(new_var = new var("p" + std::to_string(CNF.size() / 3 + 1))));

      exprPool.push_back(expr_ptr(sb = new _not(nullptr)));
      sb->r = new_var;
      exprPool.push_back(expr_ptr(sb_l1 = new _not(nullptr)));
      sb_l1->r = l1;
      exprPool.push_back(expr_ptr(sb_l2 = new _not(nullptr)));
      sb_l2->r = l2;
      {
         set<expr*> df;
         df.insert(sb);
         df.insert(l1);
         CNF.emplace(df);
      }
      {
         set<expr*> df;
         df.insert(sb);
         df.insert(l2);
         CNF.emplace(df);
      }
      {
         set<expr*> df;
         df.insert(sb_l1);
         df.insert(sb_l2);
         df.insert(new_var);
         CNF.emplace(df);
      }
      rmExprFromPool(fi);
      fi = new_var;
   }

   void change_or_to_literal(expr*& fi)
   {
      var* new_var = nullptr;
      _not* sb = nullptr, *sb_l1 = nullptr, *sb_l2 = nullptr;
      exprPool.push_back(expr_ptr(new_var = new var("p" + std::to_string(CNF.size() / 3 + 1))));
      expr*& l1 = ((_and*)fi)->l;
      expr*& l2 = ((_and*)fi)->r;

      exprPool.push_back(expr_ptr(sb = new _not(nullptr)));
      sb->r = new_var;
      exprPool.push_back(expr_ptr(sb_l1 = new _not(nullptr)));
      sb_l1->r = l1;
      exprPool.push_back(expr_ptr(sb_l2 = new _not(nullptr)));
      sb_l2->r = l2;
      {
         set<expr*> df;
         df.insert(sb);
         df.insert(l1);
         df.insert(l2);
         CNF.emplace(df);
      }
      {
         set<expr*> df;
         df.insert(sb_l1);
         df.insert(new_var);
         CNF.emplace(df);
      }
      {
         set<expr*> df;
         df.insert(sb_l2);
         df.insert(new_var);
         CNF.emplace(df);
      }
      rmExprFromPool(fi);
      fi = new_var;
   }

// ==----- OVERRIDES -----==

   void enter(therefore  *p) override
   {
      thrfr_to_or = true;
   }

   void enter(_or        *p) override
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
         }
         else if (or_to_lit) {
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
         }
         else if (or_to_lit) {
            or_to_lit = false;
            change_or_to_literal(p->r);
         }
      }
      or_to_lit = true;
   }

   void enter(_and       *p) override
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
         }
         else if (or_to_lit) {
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
         }
         else if (or_to_lit) {
            or_to_lit = false;
            change_or_to_literal(p->r);
         }
      }
      and_to_lit = true;
   }

   void enter(_not       *p) override
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
         }
         else if (or_to_lit) {
            or_to_lit = false;
            change_or_to_literal(p->r);
         }
      }
   }
};

#endif // #define TSEITIN
