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
   bool res      { false };
   bool is_call  { false };
   bool _tseitin { false };
   bool _dpll    { false };
   bool _SAT_    { false };

   void exec_tseitin(vec<expr*>& args);
   void simplify_cnf(bool rm_garbage = true);

   void enter(therefore *p) override;
   void enter(_or       *p) override;
   void enter(_and      *p) override;
   void enter(_not      *p) override;
   void enter(var       *p) override;
   void enter(call      *p) override;
private:
   void show_cnf();
};

#endif // #define EXEC
