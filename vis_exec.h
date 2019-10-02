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

struct VIS_EXEC : public VIS_WALKER {
   ostream* out = &cout;
   set<set<EXPR*>> CNF;
   map<string, int> M;
   bool res      { false };
   bool is_call  { false };
   bool _tseitin { false };
   bool _dpll    { false };
   bool _SAT_    { false };

   void exec_tseitin(VEC<EXPR*>& args);
   void simplify_cnf(bool rm_garbage = true);

   void enter(THEREFORE *p) override;
   void enter(OR       *p) override;
   void enter(AND      *p) override;
   void enter(NOT      *p) override;
   void enter(VAR       *p) override;
   void enter(CALL      *p) override;
private:
   void show_cnf();
};

#endif // #define EXEC
