#include "parser_nodes.h"

VEC<STMT_PTR>   stmtPool  ;
VEC<EXPR_PTR>   exprPool  ;
VEC<FUNC_PTR>   funcPool  ;
VEC<STRUCT_PTR> structPool;

#define put_bin_op_constructor(name) name::name(EXPR* _l, EXPR* _r) : l(_l), r(_r) { if (l != nullptr) exprPool.push_back(EXPR_PTR(l)); if (r != nullptr) exprPool.push_back(EXPR_PTR(r)); }
put_bin_op_constructor(ADD)
put_bin_op_constructor(DIF)
put_bin_op_constructor(MUL)
put_bin_op_constructor(SEP)
put_bin_op_constructor(DOT)
put_bin_op_constructor(COL)
put_bin_op_constructor(THEREFORE)
put_bin_op_constructor(OR)
put_bin_op_constructor(AND)
put_bin_op_constructor(MORE)
put_bin_op_constructor(LESS)
put_bin_op_constructor(MORE_EQ)
put_bin_op_constructor(LESS_EQ)
put_bin_op_constructor(EQUAL)
#undef put_bin_op_constructor


#define put_un_op_constructor(name) name::name(EXPR* _r) : r(_r) { if (_r != nullptr) exprPool.push_back(EXPR_PTR(_r)); }
put_un_op_constructor(NOT)
put_un_op_constructor(UN_DIF)
put_un_op_constructor(UN_ADD)
#undef put_un_op_constructor


CALL::CALL(EXPR* _id, const VEC<EXPR*>& _args)
   : id(_id), args(_args)
{
   if (_id != nullptr) exprPool.push_back(EXPR_PTR(_id));
   for (auto& arg : args) if (arg != nullptr) exprPool.push_back(EXPR_PTR(arg));
}


IFEL::IFEL(EXPR* _cond, EXPR* _l, EXPR* _r)
   : cond(_cond), l(_l), r(_r)
{
   if (_cond != nullptr) exprPool.push_back(EXPR_PTR(_cond));
   if (_l    != nullptr) exprPool.push_back(EXPR_PTR(_l   ));
   if (_r    != nullptr) exprPool.push_back(EXPR_PTR(_r   ));
}


FOR_EACH::FOR_EACH(VAR* _idx, VAR* _val, EXPR* _src, EXPR* _cond, EXPR* _modif)
   : idx(_idx), val(_val), src(_src), cond(_cond), modif(_modif)
{
   if (_idx   != nullptr) exprPool.push_back(EXPR_PTR(_idx  ));
   if (_val   != nullptr) exprPool.push_back(EXPR_PTR(_val  ));
   if (_src   != nullptr) exprPool.push_back(EXPR_PTR(_src  ));
   if (_cond  != nullptr) exprPool.push_back(EXPR_PTR(_cond ));
   if (_modif != nullptr) exprPool.push_back(EXPR_PTR(_modif));
}


FOR_CH::FOR_CH(VAR* _idx, VAR* _val, EXPR* _src, EXPR* _cond, STMT* _body)
   : idx(_idx), val(_val), src(_src), cond(_cond), body(_body)
{
   if (_idx != nullptr ) exprPool.push_back(EXPR_PTR(_idx ));
   if (_val != nullptr ) exprPool.push_back(EXPR_PTR(_val ));
   if (_src != nullptr ) exprPool.push_back(EXPR_PTR(_src ));
   if (_cond != nullptr) exprPool.push_back(EXPR_PTR(_cond));
   if (_body != nullptr) stmtPool.push_back(STMT_PTR(_body));
}


IF_ELSE::IF_ELSE(EXPR* _cond, STMT* _success, STMT* _fail)
   : cond(_cond), success(_success), fail(_fail)
{
   if (_cond    != nullptr) exprPool.push_back(EXPR_PTR(_cond   ));
   if (_success != nullptr) stmtPool.push_back(STMT_PTR(_success));
   if (_fail    != nullptr) stmtPool.push_back(STMT_PTR(_fail   ));
}


WHILE_LOOP::WHILE_LOOP(EXPR* _cond, STMT* _body)
   : cond(_cond), body(_body)
{
   if (_cond != nullptr) exprPool.push_back(EXPR_PTR(_cond));
   if (_body != nullptr) stmtPool.push_back(STMT_PTR(_body));
}


FUNC::FUNC(string _id, const VEC<EXPR*>& _argsId, BODY* __body)
   : id(_id), argsId(_argsId), _body(__body)
{
   if (_body != nullptr) for (auto& s : _body->stmts) if (s != nullptr) stmtPool.push_back(STMT_PTR(s));
}


LAM::LAM(const VEC<EXPR*>& _argsId, STMT* _s)
   : argsId(_argsId), s(_s)
{
   for (auto& arg : _argsId) if (arg != nullptr) exprPool.push_back(EXPR_PTR(arg));
   if (_s != nullptr) stmtPool.push_back(STMT_PTR(_s));
}


ASSIGN   ::ASSIGN   (string _id, EXPR* _e) : id(_id), e(_e) { if (_e  != nullptr) exprPool.push_back(EXPR_PTR(_e )); }
MAPA     ::MAPA     (EXPR*  _id, EXPR* _e) : id(_id), e(_e) { if (_id != nullptr) exprPool.push_back(EXPR_PTR(_id)); if (_e != nullptr) exprPool.push_back(EXPR_PTR(_e)); }

ASSERT   ::ASSERT   (EXPR* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(EXPR_PTR(_e)); }
RET      ::RET      (EXPR* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(EXPR_PTR(_e)); }
BREAK   ::BREAK   (EXPR* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(EXPR_PTR(_e)); }
CONTINUE::CONTINUE(EXPR* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(EXPR_PTR(_e)); }

BODY     ::BODY     (const VEC<STMT*>& _stmts) : stmts(_stmts) { for (auto& stmt : _stmts) if (stmt != nullptr)stmtPool.push_back(STMT_PTR(stmt)); }


STRUCT::STRUCT(string _id, STRUCT* _parent, const VEC<ASSIGN*>& _fields, const VEC<FUNC*>& _methods)
   : id(_id), parent(_parent), fields(_fields), methods(_methods)
{
   for (auto& m : _methods) if (m != nullptr) { funcPool.push_back(FUNC_PTR(m)); methodIds.insert(m->id); }
   for (auto& v : _fields)  if (v != nullptr) { stmtPool.push_back(STMT_PTR(v));  fieldIds.insert(v->id); }
   if (_parent != nullptr) {
      for (auto& m : _parent->methods) if (m != nullptr && !methodIds.count(m->id)) methods.push_back(m);
      for (auto& v : _parent->fields)  if (v != nullptr && !fieldIds .count(v->id)) fields.push_back(v);
   }
}
