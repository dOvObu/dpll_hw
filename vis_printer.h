#ifndef VIS_PRINTER_H
#define VIS_PRINTER_H
#include <iostream>
#include <vector>
#include <string>
#include "parser_nodes.h"
#include "vis_base.h"

using namespace std;

struct printer : public vis_base {
   ostream* out = &cout;
   string tabs = "";
   string tab = "   ";
   size_t tabsize = tab.length();

   void addtab();
   void poptab();

   void enter(var        *p);
   void enter(num        *p);
   void enter(str        *p);
   void enter(seq        *p);
   void enter(add        *p);
   void enter(dif        *p);
   void enter(mul        *p);
   void enter(sep        *p);
   void enter(dot        *p);
   void enter(col        *p);
   void enter(therefore  *p);
   void enter(_or        *p);
   void enter(_and       *p);
   void enter(more       *p);
   void enter(_less      *p);
   void enter(more_eq    *p);
   void enter(less_eq    *p);
   void enter(_equal     *p);
   void enter(_not       *p);
   void enter(un_dif     *p);
   void enter(un_add     *p);
   void enter(call       *p);
   void enter(ifel       *p);
   void enter(for_ch     *p);
   void enter(for_each_  *p);
   void enter(if_else    *p);
   void enter(while_loop *p);
   void enter(mapa       *p);
   void enter(assign     *p);
   void enter(body       *p);
   void enter(ret        *p);
   void enter(_break     *p);
   void enter(_continue  *p);
   void enter(assert     *p);
   void enter(func       *p);
   void enter(lam        *p);
   void enter(STRUCT     *p);
};


void showExprPool();
void showStmtPool();
void show_contents_of_pool();

#endif // VIS_PRINTER
