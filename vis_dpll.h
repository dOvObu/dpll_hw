#ifndef VIS_DPLL_H
#define VIS_DPLL_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include "parser_nodes.h"
#include "vis_base.h"
#include "vis_printer.h"
#include "vis_walker.h"
#include "vis_equality.h"

using namespace std;


struct vis_dpll : public vis_walker {

   ostream* out = &cout;
   set<set<expr*>> CNF;
   map<string, int> _M;
   bool res      = false;
   bool _SAT_    = false;
   bool _UNSAT_  = false;

   void exec(set<set<expr*>> _CNF);

// ==----- OVERRIDES -----==

   void enter(_not* p) override;
   void enter(var*  p) override;
   void enter(num*  p) override;

// ==----- PRIVATE ------==
private:
   bool _dpll(set<set<expr*>> S, map<string, int> M);

   set<set<expr*>> unit_propagate(set<set<expr*>> S, map<string, int>& M);
   set<set<expr*>> eliminate_pure_literal(set<set<expr*>> s, map<string, int>& M);
   set<expr*>      rm_contrar_lit(set<expr*> C);
   string          select_lit(set<set<expr*>>& S, map<string, int>& M);

   string get_id_of_lit(expr* p);

   int  get_res(expr* p, map<string, int>& M);
   bool is_positive(expr *p);
   void clear_interpretation(map<string, int>& M);
   bool is_sat(set<set<expr*>>& S, map<string, int>& M);
   bool is_unsat(set<set<expr*>>& S);
   void show_interpretation(map<string, int>& M);
   void show_cnf(set<set<expr*>>& S);
   void show_cnf();
};

#endif // VIS_DPLL_H
