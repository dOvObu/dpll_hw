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

   void addtab() { tabs += tab; }
   void poptab() { tabs = tabs.substr(0, tabs.length() - tabsize); }

   void enter(var        *p) { *out << p->id; }
   void enter(num        *p) { *out << p->n; }
   void enter(str        *p) { *out << '\'' << p->s << '\''; }
   void enter(seq        *p) { *out << '['; bool first = true; for (auto& it : p->s) { if (!first) *out << ", "; else first = !first; if (it != nullptr) it->enter(this); else *out << "nil"; } *out << ']'; }
   void enter(add        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " + ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(dif        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " - ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(mul        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " * ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(sep        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " / ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(dot        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out <<  '.';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(col        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " : ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(therefore  *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " => "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(_or        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " | ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(_and       *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " & ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(more       *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " > ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(_less      *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " < ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(more_eq    *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " >= "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(less_eq    *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " <= "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(_equal     *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " = ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(_not       *p) { *out << '(';                                                             *out <<  '!';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(un_dif     *p) { *out << '(';                                                             *out <<  '-';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(un_add     *p) { *out << '(';                                                             *out <<  '+';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(call       *p) { if (p->id != nullptr) p->id->enter(this); else *out << "nil"; *out << '('; bool first = true; for (auto& it : p->args) { if (it != nullptr) { if (!first) *out << ", "; else first = !first; it->enter(this); } } *out << ')'; }
   void enter(ifel       *p) { *out << '('; if (p->cond != nullptr) p->cond->enter(this); *out << '?'; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << ':';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
   void enter(for_ch     *p) { *out << "for (";   if (p->idx  != nullptr) { p->idx->enter(this); *out << ", "; } if (p->val != nullptr) p->val->enter(this); else *out << "nil"; *out << " : ";   if (p->src != nullptr) p->src->enter(this); else *out << "nil"; if (p->cond != nullptr) { *out << " | "; p->cond->enter(this); } *out << ")"; p->body->enter(this); }
   void enter(for_each_  *p) { *out << "for (";   if (p->idx  != nullptr) { p->idx->enter(this); *out << ", "; } if (p->val != nullptr) p->val->enter(this); else *out << "nil"; *out << " : ";   if (p->src != nullptr) p->src->enter(this); else *out << "nil"; if (p->cond != nullptr) { *out << " | "; p->cond->enter(this); } *out << ").("; p->modif->enter(this); *out << ')'; }
   void enter(if_else    *p) { *out << "if (";    if (p->cond != nullptr) { p->cond->enter(this); } *out << ')'; p->success->enter(this); if (p->fail != nullptr) { *out << '\n' << tabs << "else" << std::endl; p->fail->enter(this); } }
   void enter(while_loop *p) { *out << "while ("; if (p->cond != nullptr) { p->cond->enter(this); } *out << ')'; if (p->body != nullptr) p->body->enter(this); }
   void enter(mapa       *p) { p->id->enter(this); *out << " := "; p->e->enter(this); }
   void enter(assign     *p) {            *out << p->id << " := "; p->e->enter(this); }
   void enter(body       *p) { *out << '\n' << tabs << "{\n"; addtab(); for (auto& s : p->stmts) { *out << tabs; s->enter(this); *out << endl; } poptab(); *out << tabs << '}'; }
   void enter(ret        *p) { *out << "ret ";      if (p->e != nullptr) p->e->enter(this); }
   void enter(_break     *p) { *out << "break ";    if (p->e != nullptr) p->e->enter(this); }
   void enter(_continue  *p) { *out << "continue "; if (p->e != nullptr) p->e->enter(this); }
   void enter(assert     *p) { *out << "assert ";   if (p->e != nullptr) p->e->enter(this); }
   void enter(func       *p) { *out << "func " << p->id << '('; { bool first = true; for (auto& it : p->argsId) { if (!first) *out << ", "; else first = !first; it->enter(this); }} *out << ')'; if (p->_body != nullptr) p->_body->enter(this); *out << endl; }
   void enter(lam        *p) { *out << "\\("; { bool first = true; for (auto& it : p->argsId) { if (!first) *out << ", "; else first = !first; it->enter(this); } } *out << ") "; addtab(); if (p->s != nullptr) p->s->enter(this); else *out << tabs << "nil"; poptab(); }
   void enter(STRUCT     *p) { *out << "struct " << p->id; if (p->parent != nullptr) *out << " : " << p->parent->id; *out << " {" << endl; addtab(); for (auto& it : p->fields) it->enter(this); if (!p->fields.empty()) *out << endl; for (auto& it : p->methods) it->enter(this); poptab(); *out << tabs << "}" << endl; }
};


static void showExprPool()
{
   printer writer;
   cout << "--- expr pool ---" << endl;
   for (auto& it : exprPool) {
      it->enter(&writer);
      cout << endl;
   }
   cout << "-----------------" << endl;
}


static void showStmtPool()
{
   printer writer;
   cout << "--- stmt pool ---" << endl;
   for (auto& it : stmtPool) {
      it->enter(&writer);
      cout << endl;
   }
   cout << "-----------------" << endl;
}


static void show_contents_of_pool()
{
   printer writer;

   if (structPool.empty()) {
      if (funcPool.empty()) {
         if (stmtPool.empty()) {
            if (!exprPool.empty()) {
               showExprPool();
            }
         }
         else { showStmtPool(); }
      }
      else { for (auto& it : funcPool) it->enter(&writer); }
   }
   else { structPool.back()->enter(&writer); }

   cout
      << "\n struct:" << structPool.size()
      << "\n func:\t" << funcPool.size()
      << "\n stmt:\t" << stmtPool.size()
      << "\n expr:\t" << exprPool.size()
      << endl;
}
#endif // VIS_PRINTER
