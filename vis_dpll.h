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


struct VIS_DPLL : public VIS_WALKER {

   ostream* out = &cout;
   set<set<EXPR*>> CNF;
   map<string, int> _M;
   bool res      = false;
   bool _SAT_    = false;
   bool _UNSAT_  = false;

   void exec(set<set<EXPR*>> _CNF);

// ==----- OVERRIDES -----==

   void enter(NOT* p) override;
   void enter(VAR*  p) override;
   void enter(NUM*  p) override;

// ==----- PRIVATE ------==
private:
   bool _dpll(set<set<EXPR*>> S, map<string, int> M);

   set<set<EXPR*>> unit_propagate(set<set<EXPR*>> S, map<string, int>& M);
   set<set<EXPR*>> eliminate_pure_literal(set<set<EXPR*>> s, map<string, int>& M);
   set<EXPR*>      rm_contrar_lit(set<EXPR*> C);
   string          select_lit(set<set<EXPR*>>& S, map<string, int>& M);

   string get_id_of_lit(EXPR* p);

   int  get_res(EXPR* p, map<string, int>& M);
   bool is_positive(EXPR *p);
   void clear_interpretation(map<string, int>& M);
   bool is_sat(set<set<EXPR*>>& S, map<string, int>& M);
   bool is_unsat(set<set<EXPR*>>& S);
   void show_interpretation(map<string, int>& M);
   void show_cnf(set<set<EXPR*>>& S);
   void show_cnf();
};

#endif // VIS_DPLL_H
