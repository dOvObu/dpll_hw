#ifndef VIS_WALKER_H
#define VIS_WALKER_H
#include "vis_base.h"

struct vis_walker : public vis_base {
public:
   void enter(var        *p) override {  p->id; }
   void enter(num        *p) override {  p->n;  }
   void enter(str        *p) override {  p->s;  }
   void enter(seq        *p) override { for (auto& it : p->s) { if (it != nullptr) it->enter(this); } }
   void enter(add        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(dif        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(mul        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(sep        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(dot        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(col        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(therefore  *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(_or        *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(_and       *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(more       *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(_less      *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(more_eq    *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(less_eq    *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(_equal     *p) override { if (p->l != nullptr) p->l->enter(this);   if (p->r != nullptr) p->r->enter(this); }
   void enter(_not       *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(un_dif     *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(un_add     *p) override {                                           if (p->r != nullptr) p->r->enter(this); }
   void enter(call       *p) override { if (p->id   != nullptr) { p->id  ->enter(this); } for (auto& it : p->args) { if (it != nullptr) { it->enter(this); } } }
   void enter(ifel       *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->l       != nullptr) { p->l      ->enter(this); } if (p->r    != nullptr) { p->r  ->enter(this); } }
   void enter(for_ch     *p) override { if (p->idx  != nullptr) { p->idx ->enter(this); } if (p->val     != nullptr) { p->val    ->enter(this); } if (p->src  != nullptr) { p->src->enter(this); } if (p->cond != nullptr) { p->cond->enter(this); } p->body ->enter(this); }
   void enter(for_each_  *p) override { if (p->idx  != nullptr) { p->idx ->enter(this); } if (p->val     != nullptr) { p->val    ->enter(this); } if (p->src  != nullptr) { p->src->enter(this); } if (p->cond != nullptr) { p->cond->enter(this); } p->modif->enter(this); }
   void enter(while_loop *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->body    != nullptr) { p->body   ->enter(this); } }
   void enter(if_else    *p) override { if (p->cond != nullptr) { p->cond->enter(this); } if (p->success != nullptr) { p->success->enter(this); } if (p->fail != nullptr) { p->fail->enter(this); } }
   void enter(body       *p) override { for (auto& s : p->stmts) { if (s != nullptr) { s->enter(this); } } }
   void enter(mapa       *p) override { if (p->id != nullptr) { p->id->enter(this); } if (p->e != nullptr) { p->e->enter(this); } }
   void enter(assign     *p) override {                         p->id;                if (p->e != nullptr) { p->e->enter(this); } }
   void enter(ret        *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(_break     *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(_continue  *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(assert     *p) override {                                               if (p->e != nullptr) { p->e->enter(this); } }
   void enter(func       *p) override {                         p->id; for (auto& it : p->argsId) { if (it != nullptr) it->enter(this); } if (p->_body != nullptr) p->_body->enter(this); }
   void enter(STRUCT     *p) override {                         p->id; if (p->parent != nullptr) { p->parent->id; } for (auto& it : p->fields){ if (it != nullptr) { it->enter(this); } } for (auto& it : p->methods) { if(it != nullptr) { it->enter(this); } } }
   void enter(lam        *p) override { for (auto& it : p->argsId) { if (it != nullptr) { it->enter(this); } } if (p->s != nullptr) { p->s->enter(this); } }
};
#endif // ifndef VIS_BASE_H