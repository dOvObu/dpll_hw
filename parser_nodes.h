#ifndef PARSER_NODES_H
#define PARSER_NODES_H
#include <vector>
#include <memory>
#include <set>
#include "vis_base.h"

using namespace std;

template <typename T>
using  VEC = std::vector<T>;

struct STMT { virtual void enter(VIS_BASE* s) = 0; };
using  STMT_PTR = shared_ptr<STMT>;
extern VEC<STMT_PTR> stmtPool;

static void rmStmtFromPool(STMT* ptr)
{
   for (size_t idx = 0; idx < stmtPool.size(); ++idx) {
      if (stmtPool[idx].get() == ptr) {
         stmtPool.erase(begin(stmtPool) + idx);
         return;
      }
   }
}

struct EXPR : STMT {};
using EXPR_PTR = shared_ptr<EXPR>;
extern VEC<EXPR_PTR> exprPool;

static void rmExprFromPool(EXPR* ptr)
{
   for (size_t idx = 0; idx < exprPool.size(); ++idx) {
      if (exprPool[idx].get() == ptr) {
         exprPool.erase(begin(exprPool) + idx);
         return;
      }
   }
}

struct VAR : EXPR { VAR(string    _id) : id(_id) {}   string    id;  void enter(VIS_BASE* s) { s->enter(this); } };

struct LIT : EXPR {};
struct NUM : LIT  { NUM(int        _n) : n(_n)   {}   int        n;  void enter(VIS_BASE* s) { s->enter(this); } };
struct STR : LIT  { STR(string     _s) : s(_s)   {}   string     s;  void enter(VIS_BASE* s) { s->enter(this); } };
struct SEQ : LIT  { SEQ(VEC<EXPR*> _s) : s(_s)   {}   VEC<EXPR*> s;  void enter(VIS_BASE* s) { s->enter(this); } };

struct ADD : EXPR { ADD(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct DIF : EXPR { DIF(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct MUL : EXPR { MUL(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct SEP : EXPR { SEP(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct DOT : EXPR { DOT(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct COL : EXPR { COL(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };

struct THEREFORE : EXPR { THEREFORE(EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct OR        : EXPR { OR(       EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct AND       : EXPR { AND(      EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct MORE      : EXPR { MORE(     EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct LESS      : EXPR { LESS(     EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct MORE_EQ   : EXPR { MORE_EQ(  EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct LESS_EQ   : EXPR { LESS_EQ(  EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct EQUAL     : EXPR { EQUAL(    EXPR* l, EXPR* r); EXPR* l, *r; void enter(VIS_BASE* s) { s->enter(this); } };
struct NOT       : EXPR { NOT(      EXPR* r);          EXPR* r;     void enter(VIS_BASE* s) { s->enter(this); } };
struct UN_DIF    : EXPR { UN_DIF(   EXPR* r);          EXPR* r;     void enter(VIS_BASE* s) { s->enter(this); } };
struct UN_ADD    : EXPR { UN_ADD(   EXPR* r);          EXPR* r;     void enter(VIS_BASE* s) { s->enter(this); } };

struct CALL      : EXPR { CALL(EXPR* id,   const VEC<EXPR*>& args);  EXPR* id; VEC<EXPR*> args;  void enter(VIS_BASE* s) { s->enter(this); } };
struct IFEL      : EXPR { IFEL(EXPR* cond, EXPR* l,  EXPR* r     );  EXPR *cond, *l, *r;         void enter(VIS_BASE* s) { s->enter(this); } };

struct FOR_EACH  : EXPR { FOR_EACH(VAR*  idx,  VAR* val, EXPR* src, EXPR* cond, EXPR* modif);   VAR *idx, *val;  EXPR *src, *cond, *modif;     void enter(VIS_BASE* s) { s->enter(this); } };
struct FOR_CH    : STMT { FOR_CH  (VAR*  idx,  VAR* val, EXPR* src, EXPR* cond, STMT* body );   VAR *idx, *val;  EXPR *src, *cond; STMT *body; void enter(VIS_BASE* s) { s->enter(this); } };

struct IF_ELSE : STMT {
   EXPR* cond;
   STMT *success, *fail;

   IF_ELSE(EXPR* cond, STMT* success, STMT* fail);
   void enter(VIS_BASE* s) { s->enter(this); }
};


struct WHILE_LOOP : STMT {
   EXPR* cond;
   STMT* body;

   WHILE_LOOP(EXPR* cond, STMT* body);
   void enter(VIS_BASE* s) { s->enter(this); }
};

struct BODY      : STMT { BODY     (const VEC<STMT*>& stmts); VEC<STMT*>   stmts;   void enter(VIS_BASE* s) { s->enter(this); } };
struct ASSIGN    : STMT { ASSIGN   (string id, EXPR* e);      string id; EXPR* e;   void enter(VIS_BASE* s) { s->enter(this); } };
struct MAPA      : STMT { MAPA     (EXPR*  id, EXPR* e);      EXPR  *id,      *e;   void enter(VIS_BASE* s) { s->enter(this); } };

struct RET       : STMT { RET      (EXPR* e); EXPR* e; void enter(VIS_BASE* s) { s->enter(this); } };
struct BREAK     : STMT { BREAK    (EXPR* e); EXPR* e; void enter(VIS_BASE* s) { s->enter(this); } };
struct CONTINUE  : STMT { CONTINUE (EXPR* e); EXPR* e; void enter(VIS_BASE* s) { s->enter(this); } };
struct ASSERT    : STMT { ASSERT   (EXPR* e); EXPR* e; void enter(VIS_BASE* s) { s->enter(this); } };

struct LAM : EXPR {
   VEC<EXPR*> argsId;
   STMT* s;

   LAM(const VEC<EXPR*>& argsId, STMT* stmt);
   void enter(VIS_BASE* s) { s->enter(this); }
};


struct FUNC {
   string id;
   VEC<EXPR*> argsId;
   BODY* _body;

   FUNC(string id, const VEC<EXPR*>& argsId, BODY* body);
   void enter(VIS_BASE* s) { s->enter(this); }
};
using FUNC_PTR = shared_ptr<FUNC>;
extern VEC<FUNC_PTR> funcPool;


struct STRUCT {
   string        id;
   STRUCT*       parent;

   VEC<ASSIGN*>  fields;
   VEC< FUNC* >  methods;
   set<string >  fieldIds;
   set<string >  methodIds;

   STRUCT(string id, STRUCT* parent, const VEC<ASSIGN*>& fields, const VEC<FUNC*>& methods);
   void enter(VIS_BASE* s) { s->enter(this); }
};
using STRUCT_PTR = shared_ptr<STRUCT>;
extern VEC<STRUCT_PTR> structPool;

#endif // PARSER_NODES_H
