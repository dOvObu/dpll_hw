#ifndef VIS_PRINTER_H
#define VIS_PRINTER_H
#include <iostream>
#include <vector>
#include <string>
#include "parser_nodes.h"
#include "vis_base.h"

using namespace std;

struct VIS_PRINTER : public VIS_BASE {
   ostream* out = &cout;
   string tabs = "";
   string tab = "   ";
   size_t tabsize = tab.length();

   void addtab();
   void poptab();

   void enter(VAR        *p);
   void enter(NUM        *p);
   void enter(STR        *p);
   void enter(SEQ        *p);
   void enter(ADD        *p);
   void enter(DIF        *p);
   void enter(MUL        *p);
   void enter(SEP        *p);
   void enter(DOT        *p);
   void enter(COL        *p);
   void enter(THEREFORE  *p);
   void enter(OR        *p);
   void enter(AND       *p);
   void enter(MORE       *p);
   void enter(LESS      *p);
   void enter(MORE_EQ    *p);
   void enter(LESS_EQ    *p);
   void enter(EQUAL     *p);
   void enter(NOT       *p);
   void enter(UN_DIF     *p);
   void enter(UN_ADD     *p);
   void enter(CALL       *p);
   void enter(IFEL       *p);
   void enter(FOR_CH     *p);
   void enter(FOR_EACH  *p);
   void enter(IF_ELSE    *p);
   void enter(WHILE_LOOP *p);
   void enter(MAPA       *p);
   void enter(ASSIGN     *p);
   void enter(BODY       *p);
   void enter(RET        *p);
   void enter(BREAK     *p);
   void enter(CONTINUE  *p);
   void enter(ASSERT     *p);
   void enter(FUNC       *p);
   void enter(LAM        *p);
   void enter(STRUCT     *p);
};


void showExprPool();
void showStmtPool();
void show_contents_of_pool();

#endif // VIS_PRINTER
