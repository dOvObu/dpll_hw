#include "vis_printer.h"


void VIS_PRINTER::addtab() { tabs += tab; }
void VIS_PRINTER::poptab() { tabs = tabs.substr(0, tabs.length() - tabsize); }

void VIS_PRINTER::enter(VAR        *p) { *out << p->id; }
void VIS_PRINTER::enter(NUM        *p) { *out << p->n; }
void VIS_PRINTER::enter(STR        *p) { *out << '\'' << p->s << '\''; }
void VIS_PRINTER::enter(SEQ        *p) { *out << '['; bool first = true; for (auto& it : p->s) { if (!first) *out << ", "; else first = !first; if (it != nullptr) it->enter(this); else *out << "nil"; } *out << ']'; }
void VIS_PRINTER::enter(ADD        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " + ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(DIF        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " - ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(MUL        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " * ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(SEP        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " / ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(DOT        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out <<  '.';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(COL        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " : ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(THEREFORE  *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " => "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(OR        *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " | ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(AND       *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " & ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(MORE       *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " > ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(LESS      *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " < ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(MORE_EQ    *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " >= "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(LESS_EQ    *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " <= "; if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(EQUAL     *p) { *out << '('; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << " = ";  if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(NOT       *p) { *out << '(';                                                             *out <<  '!';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(UN_DIF     *p) { *out << '(';                                                             *out <<  '-';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(UN_ADD     *p) { *out << '(';                                                             *out <<  '+';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(CALL       *p) { if (p->id != nullptr) p->id->enter(this); else *out << "nil"; *out << '('; bool first = true; for (auto& it : p->args) { if (it != nullptr) { if (!first) *out << ", "; else first = !first; it->enter(this); } } *out << ')'; }
void VIS_PRINTER::enter(IFEL       *p) { *out << '('; if (p->cond != nullptr) p->cond->enter(this); *out << '?'; if (p->l != nullptr) p->l->enter(this); else *out << "nil"; *out << ':';   if (p->r != nullptr) p->r->enter(this); else *out << "nil"; *out << ')'; }
void VIS_PRINTER::enter(FOR_CH     *p) { *out << "for (";   if (p->idx  != nullptr) { p->idx->enter(this); *out << ", "; } if (p->val != nullptr) p->val->enter(this); else *out << "nil"; *out << " : ";   if (p->src != nullptr) p->src->enter(this); else *out << "nil"; if (p->cond != nullptr) { *out << " | "; p->cond->enter(this); } *out << ")"; p->body->enter(this); }
void VIS_PRINTER::enter(FOR_EACH  *p) { *out << "for (";   if (p->idx  != nullptr) { p->idx->enter(this); *out << ", "; } if (p->val != nullptr) p->val->enter(this); else *out << "nil"; *out << " : ";   if (p->src != nullptr) p->src->enter(this); else *out << "nil"; if (p->cond != nullptr) { *out << " | "; p->cond->enter(this); } *out << ").("; p->modif->enter(this); *out << ')'; }
void VIS_PRINTER::enter(IF_ELSE    *p) { *out << "if (";    if (p->cond != nullptr) { p->cond->enter(this); } *out << ')'; p->success->enter(this); if (p->fail != nullptr) { *out << '\n' << tabs << "else" << std::endl; p->fail->enter(this); } }
void VIS_PRINTER::enter(WHILE_LOOP *p) { *out << "while ("; if (p->cond != nullptr) { p->cond->enter(this); } *out << ')'; if (p->body != nullptr) p->body->enter(this); }
void VIS_PRINTER::enter(MAPA       *p) { p->id->enter(this); *out << " := "; p->e->enter(this); }
void VIS_PRINTER::enter(ASSIGN     *p) {            *out << p->id << " := "; p->e->enter(this); }
void VIS_PRINTER::enter(BODY       *p) { *out << '\n' << tabs << "{\n"; addtab(); for (auto& s : p->stmts) { *out << tabs; s->enter(this); *out << endl; } poptab(); *out << tabs << '}'; }
void VIS_PRINTER::enter(RET        *p) { *out << "ret ";      if (p->e != nullptr) p->e->enter(this); }
void VIS_PRINTER::enter(BREAK     *p) { *out << "break ";    if (p->e != nullptr) p->e->enter(this); }
void VIS_PRINTER::enter(CONTINUE  *p) { *out << "continue "; if (p->e != nullptr) p->e->enter(this); }
void VIS_PRINTER::enter(ASSERT     *p) { *out << "assert ";   if (p->e != nullptr) p->e->enter(this); }
void VIS_PRINTER::enter(FUNC       *p) { *out << "func " << p->id << '('; { bool first = true; for (auto& it : p->argsId) { if (!first) *out << ", "; else first = !first; it->enter(this); }} *out << ')'; if (p->_body != nullptr) p->_body->enter(this); *out << endl; }
void VIS_PRINTER::enter(LAM        *p) { *out << "\\("; { bool first = true; for (auto& it : p->argsId) { if (!first) *out << ", "; else first = !first; it->enter(this); } } *out << ") "; addtab(); if (p->s != nullptr) p->s->enter(this); else *out << tabs << "nil"; poptab(); }
void VIS_PRINTER::enter(STRUCT     *p) { *out << "struct " << p->id; if (p->parent != nullptr) *out << " : " << p->parent->id; *out << " {" << endl; addtab(); for (auto& it : p->fields) it->enter(this); if (!p->fields.empty()) *out << endl; for (auto& it : p->methods) it->enter(this); poptab(); *out << tabs << "}" << endl; }


void showExprPool()
{
   VIS_PRINTER writer;
   cout << "--- expr pool ---" << endl;
   for (auto& it : exprPool) {
      it->enter(&writer);
      cout << endl;
   }
   cout << "-----------------" << endl;
}


void showStmtPool()
{
   VIS_PRINTER writer;
   cout << "--- stmt pool ---" << endl;
   for (auto& it : stmtPool) {
      it->enter(&writer);
      cout << endl;
   }
   cout << "-----------------" << endl;
}


void show_contents_of_pool()
{
   VIS_PRINTER writer;

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
