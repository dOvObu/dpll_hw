#ifndef VIS_WALKER_H
#define VIS_WALKER_H
#include "vis_base.h"

struct VIS_WALKER : public VIS_BASE {
public:
   void enter(VAR        *p) override {  p->id; }
   void enter(NUM        *p) override {  p->n;  }
   void enter(STR        *p) override {  p->s;  }
   void enter(SEQ        *p) override { for (auto& it : p->s) { if (it != nullptr) it->enter(this); } }
   void enter(ADD        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(DIF        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(MUL        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(SEP        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(DOT        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(COL        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(THEREFORE  *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(OR        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(AND       *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(MORE       *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(LESS      *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(MORE_EQ    *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(LESS_EQ    *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(EQUAL     *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(NOT       *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(UN_DIF     *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(UN_ADD     *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(CALL       *p) override { if (p->id   != nullptr) { p->id  ->enter(this); } for (auto& it : p->args) { if (it != nullptr) { it->enter(this); } } }
   void enter(IFEL       *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->l       != nullptr) { p->l      ->enter(this); } if (p->r    != nullptr) { p->r  ->enter(this); } }
   void enter(FOR_CH     *p) override { if (p->idx  != nullptr) { p->idx ->enter(this); } if (p->val     != nullptr) { p->val    ->enter(this); } if (p->src  != nullptr) { p->src->enter(this); } if (p->cond != nullptr) { p->cond->enter(this); } p->body ->enter(this); }
   void enter(FOR_EACH  *p) override { if (p->idx  != nullptr) { p->idx ->enter(this); } if (p->val     != nullptr) { p->val    ->enter(this); } if (p->src  != nullptr) { p->src->enter(this); } if (p->cond != nullptr) { p->cond->enter(this); } p->modif->enter(this); }
   void enter(WHILE_LOOP *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->body    != nullptr) { p->body   ->enter(this); } }
   void enter(IF_ELSE    *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->success != nullptr) { p->success->enter(this); } if (p->fail != nullptr) { p->fail->enter(this); } }
   void enter(BODY       *p) override { for (auto& s : p->stmts) { if (s != nullptr) { s->enter(this); } } }
   void enter(MAPA       *p) override { if (p->id != nullptr) { p->id->enter(this); } if (p->e != nullptr) { p->e->enter(this); } }
   void enter(ASSIGN     *p) override {                         p->id;                if (p->e != nullptr) { p->e->enter(this); } }
   void enter(RET        *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(BREAK     *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(CONTINUE  *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(ASSERT     *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(FUNC       *p) override {                         p->id; for (auto& it : p->argsId) { if (it != nullptr) it->enter(this); } if (p->_body != nullptr) p->_body->enter(this); }
   void enter(STRUCT     *p) override {                         p->id; if (p->parent != nullptr) { p->parent->id; } for (auto& it : p->fields){ if (it != nullptr) { it->enter(this); } } for (auto& it : p->methods) { if(it != nullptr) { it->enter(this); } } }
   void enter(LAM        *p) override { for (auto& it : p->argsId) { if (it != nullptr) { it->enter(this); } } if (p->s != nullptr) { p->s->enter(this); } }
};
#endif // ifndef VIS_BASE_H