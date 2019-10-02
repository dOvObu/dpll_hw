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

struct VIS_TSEITIN : public VIS_WALKER {
   ostream* out        { &cout };
   bool thrfr_to_or    { false };
   bool and_to_lit     { false };
   bool or_to_lit      { false };
   bool not_not_invite { false };
   bool not_not_rm     { false };
   VIS_EXPR_TYPE expert;

   set<set<EXPR*>> CNF;

   void change_therefore_to_or(EXPR*& phf);
   void change_and_to_literal (EXPR*& phi);
   void change_or_to_literal  (EXPR*& phi);
   EXPR* get_contr(EXPR* p, bool need_rm = false);
// ==----- OVERRIDES -----==
   void enter(THEREFORE* p) override;
   void enter(OR*       p) override;
   void enter(AND*      p) override;
   void enter(NOT*      p) override;
};

#endif // #define TSEITIN
