#ifndef VIS_TSEITIN_H
#define VIS_TSEITIN_H
#include <iostream>
#include <vector>
#include <string>
#include "parser_nodes.h"
#include "vis_equality.h"
#include "vis_printer.h"
#include "vis_walker.h"
#include "vis_base.h"

using namespace std;

struct tseitin : public vis_walker {
   ostream* out        { &cout };
   bool thrfr_to_or    { false };
   bool and_to_lit     { false };
   bool or_to_lit      { false };
   bool not_not_invite { false };
   bool not_not_rm     { false };
   vis_expr_type expert;

   set<set<expr*>> CNF;

   void change_therefore_to_or(expr*& phf);
   void change_and_to_literal (expr*& phi);
   void change_or_to_literal  (expr*& phi);
   expr* get_contr(expr* p, bool need_rm = false);
// ==----- OVERRIDES -----==
   void enter(therefore* p) override;
   void enter(_or*       p) override;
   void enter(_and*      p) override;
   void enter(_not*      p) override;
};

#endif // #define TSEITIN
