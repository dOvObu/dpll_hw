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
   bool _SAT_    { false };

   // -- native functions --
   map<string, bool> nat_call_flags;
   map<string, void(*)(VIS_EXEC*, VEC<EXPR*>&)> call_backs;
   void add_native_func(string id);
   void add_native_func(string id, void(*func)(VIS_EXEC*, VEC<EXPR*>&));

   // -- type tables --
   enum class TYPES {
      Integer,
      String,
      Object,
      Arch,
      Func,
      Any,
   };
   set<EXPR*> integerPool;
   set<EXPR*> stringPool;
   set<EXPR*> objectPool;
   map<string, FUNC*> functions;
   map<EXPR*, pair<VEC<TYPES>, TYPES>> archPool;
   TYPES return_type = TYPES::Any;
   bool return_found = false;

   // -- var tables --
   map<string, TYPES> type_of_variable;
   map<string, int>    num_variables;
   map<string, LAM*>   arch_variables;
   map<string, string> str_variables;
   VEC<int>    num_stack;
   VEC<string> str_stack;
   VEC<LAM*>   arch_stack;
   VEC<FUNC*>  func_stack;
   VEC<TYPES>  types_stack;


   void exec_tseitin(VEC<EXPR*>& args);
   void simplify_cnf(bool rm_garbage = true);
   void check_calls_id(EXPR* id);

   void enter(ASSIGN    *p) override;
   void enter(BODY      *p) override;
   void enter(RET       *p) override;
   void enter(THEREFORE *p) override;
   void enter(MUL       *p) override;
   void enter(ADD       *p) override;
   void enter(OR        *p) override;
   void enter(AND       *p) override;
   void enter(NOT       *p) override;
   void enter(VAR       *p) override;
   void enter(CALL      *p) override;
   void enter(LAM       *p) override;
   void enter(STR       *p) override;
   void enter(NUM       *p) override;
private:
   void show_cnf();
   bool try_to_call_nat_func(CALL* p);
   void assign_expr_to_id(EXPR* p, string& id, map<string, int>& num_vars, map<string, string>& str_vars, map<string, LAM*>& arch_vars);
};

#endif // #define EXEC
