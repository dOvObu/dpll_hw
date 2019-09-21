#ifndef PARSER_NODES_H
#define PARSER_NODES_H
#include <vector>
#include <memory>
#include <set>
#include "vis_base.h"

using namespace std;

template <typename T>
using  vec = std::vector<T>;

struct stmt { virtual void enter(vis_base* s) = 0; };
using  stmt_ptr = shared_ptr<stmt>;
extern vec<stmt_ptr> stmtPool;


static void rmStmtFromPool(stmt* ptr)
{
   for (size_t idx = 0; idx < stmtPool.size(); ++idx) {
      if (stmtPool[idx].get() == ptr) {
         stmtPool.erase(begin(stmtPool) + idx);
         return;
      }
   }
}


struct expr : stmt {};
using expr_ptr = shared_ptr<expr>;
extern vec<expr_ptr> exprPool;

static void rmExprFromPool(expr* ptr)
{
   for (size_t idx = 0; idx < exprPool.size(); ++idx) {
      if (exprPool[idx].get() == ptr) {
         exprPool.erase(begin(exprPool) + idx);
         return;
      }
   }
}
// ------------------------------------------------------
struct var : expr { var(string    _id) : id(_id) {}   string    id;  void enter(vis_base* s) { s->enter(this); } };

struct lit : expr {};
struct num : lit  { num(int        _n) : n(_n)   {}   int        n;  void enter(vis_base* s) { s->enter(this); } };
struct str : lit  { str(string     _s) : s(_s)   {}   string     s;  void enter(vis_base* s) { s->enter(this); } };
struct seq : lit  { seq(vec<expr*> _s) : s(_s)   {}   vec<expr*> s;  void enter(vis_base* s) { s->enter(this); } };


struct add : expr { add(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct dif : expr { dif(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct mul : expr { mul(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct sep : expr { sep(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct dot : expr { dot(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct col : expr { col(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };

struct therefore : expr { therefore(expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct _or       : expr { _or(      expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct _and      : expr { _and(     expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct more      : expr { more(     expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct _less     : expr { _less(    expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct more_eq   : expr { more_eq(  expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct less_eq   : expr { less_eq(  expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct _equal    : expr { _equal(   expr* l, expr* r); expr* l, *r; void enter(vis_base* s) { s->enter(this); } };
struct _not      : expr { _not(     expr* r);          expr* r;     void enter(vis_base* s) { s->enter(this); } };
struct un_dif    : expr { un_dif(   expr* r);          expr* r;     void enter(vis_base* s) { s->enter(this); } };
struct un_add    : expr { un_add(   expr* r);          expr* r;     void enter(vis_base* s) { s->enter(this); } };

struct call      : expr { call(expr* id,   const vec<expr*>& args);  expr* id; vec<expr*> args;  void enter(vis_base* s) { s->enter(this); } };
struct ifel      : expr { ifel(expr* cond, expr* l,  expr* r     );  expr *cond, *l, *r;         void enter(vis_base* s) { s->enter(this); } };

struct for_each_ : expr { for_each_(var*  idx,  var* val, expr* src, expr* cond, expr* modif);   var *idx, *val;  expr *src, *cond, *modif;     void enter(vis_base* s) { s->enter(this); } };
struct for_ch    : stmt { for_ch   (var*  idx,  var* val, expr* src, expr* cond, stmt* body );   var *idx, *val;  expr *src, *cond; stmt *body; void enter(vis_base* s) { s->enter(this); } };

struct if_else : stmt {
   expr* cond;
   stmt *success, *fail;

   if_else(expr* cond, stmt* success, stmt* fail);
   void enter(vis_base* s) { s->enter(this); }
};

struct while_loop : stmt {
   expr* cond;
   stmt* body;

   while_loop(expr* cond, stmt* body);
   void enter(vis_base* s) { s->enter(this); }
};

struct body      : stmt { body     (const vec<stmt*>& stmts); vec<stmt*>   stmts;   void enter(vis_base* s) { s->enter(this); } };
struct assign    : stmt { assign   (string id, expr* e);      string id; expr* e;   void enter(vis_base* s) { s->enter(this); } };
struct mapa      : stmt { mapa     (expr*  id, expr* e);      expr  *id,      *e;   void enter(vis_base* s) { s->enter(this); } };


struct ret       : stmt { ret      (expr* e); expr* e; void enter(vis_base* s) { s->enter(this); } };
struct _break    : stmt { _break   (expr* e); expr* e; void enter(vis_base* s) { s->enter(this); } };
struct _continue : stmt { _continue(expr* e); expr* e; void enter(vis_base* s) { s->enter(this); } };
struct assert    : stmt { assert   (expr* e); expr* e; void enter(vis_base* s) { s->enter(this); } };

struct lam : expr {
   vec<expr*> argsId;
   stmt* s;

   lam(const vec<expr*>& argsId, stmt* stmt);
   void enter(vis_base* s) { s->enter(this); }
};


struct func {
   string id;
   vec<expr*> argsId;
   body* _body;

   func(string id, const vec<expr*>& argsId, body* body);
   void enter(vis_base* s) { s->enter(this); }
};
using func_ptr = shared_ptr<func>;
extern vec<func_ptr> funcPool;


struct STRUCT {
   string        id;
   STRUCT*       parent;

   vec<assign*>  fields;
   vec< func* >  methods;
   set<string >  fieldIds;
   set<string >  methodIds;

   STRUCT(string id, STRUCT* parent, const vec<assign*>& fields, const vec<func*>& methods);
   void enter(vis_base* s) { s->enter(this); }
};
using struct_ptr = shared_ptr<STRUCT>;
extern vec<struct_ptr> structPool;

#endif // PARSER_NODES_H
