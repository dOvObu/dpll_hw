#include "parser_nodes.h"

vec<stmt_ptr>   stmtPool  ;
vec<expr_ptr>   exprPool  ;
vec<func_ptr>   funcPool  ;
vec<struct_ptr> structPool;
// ------------------------------------------------------
#define put_bin_op_constructor(name) name::name(expr* _l, expr* _r) : l(_l), r(_r) { if (l != nullptr) exprPool.push_back(expr_ptr(l)); if (r != nullptr) exprPool.push_back(expr_ptr(r)); }
put_bin_op_constructor(add)
put_bin_op_constructor(dif)
put_bin_op_constructor(mul)
put_bin_op_constructor(sep)
put_bin_op_constructor(dot)
put_bin_op_constructor(col)
put_bin_op_constructor(therefore)
put_bin_op_constructor(_or)
put_bin_op_constructor(_and)
put_bin_op_constructor(more)
put_bin_op_constructor(_less)
put_bin_op_constructor(more_eq)
put_bin_op_constructor(less_eq)
put_bin_op_constructor(_equal)
#undef put_bin_op_constructor
;


_not::_not(expr* _r) : r(_r)
{
   if (_r != nullptr) exprPool.push_back(expr_ptr(r));
}

un_dif::un_dif(expr* _r) : r(_r)
{
   if (_r != nullptr) exprPool.push_back(expr_ptr(r));
}

un_add::un_add(expr* _r) : r(_r)
{
   if (_r != nullptr) exprPool.push_back(expr_ptr(r));
}

call::call(expr* _id, const vec<expr*>& _args) : id(_id), args(_args)
{
   if (_id != nullptr) exprPool.push_back(expr_ptr(_id));
   for (auto& arg : args) if (arg != nullptr) exprPool.push_back(expr_ptr(arg));
}

ifel::ifel(expr* _cond, expr* _l, expr* _r)
   : cond(_cond), l(_l), r(_r)
{
   if (_cond != nullptr) exprPool.push_back(expr_ptr(_cond));
   if (_l != nullptr) exprPool.push_back(expr_ptr(_l));
   if (_r != nullptr) exprPool.push_back(expr_ptr(_r));
}

for_each_::for_each_(var* _idx, var* _val, expr* _src, expr* _cond, expr* _modif)
   : idx(_idx), val(_val), src(_src), cond(_cond), modif(_modif)
{
   if (_idx != nullptr) exprPool.push_back(expr_ptr(_idx));
   if (_val != nullptr) exprPool.push_back(expr_ptr(_val));
   if (_src != nullptr) exprPool.push_back(expr_ptr(_src));
   if (_cond != nullptr) exprPool.push_back(expr_ptr(_cond));
   if (_modif != nullptr) exprPool.push_back(expr_ptr(_modif));
}

for_ch::for_ch(var* _idx, var* _val, expr* _src, expr* _cond, stmt* _body)
   : idx(_idx), val(_val), src(_src), cond(_cond), body(_body)
{
   if (_idx != nullptr) exprPool.push_back(expr_ptr(_idx));
   if (_val != nullptr) exprPool.push_back(expr_ptr(_val));
   if (_src != nullptr) exprPool.push_back(expr_ptr(_src));
   if (_cond != nullptr) exprPool.push_back(expr_ptr(_cond));
   if (_body != nullptr) stmtPool.push_back(stmt_ptr(_body));
}

if_else::if_else(expr* _cond, stmt* _success, stmt* _fail)
   : cond(_cond), success(_success), fail(_fail)
{
   if (cond != nullptr)    exprPool.push_back(expr_ptr(cond));
   if (success != nullptr) stmtPool.push_back(stmt_ptr(success));
   if (fail != nullptr)    stmtPool.push_back(stmt_ptr(fail));
}

while_loop::while_loop(expr* _cond, stmt* _body)
   : cond(_cond), body(_body)
{
   if (cond != nullptr) exprPool.push_back(expr_ptr(cond));
   if (body != nullptr) stmtPool.push_back(stmt_ptr(body));
}

func::func(string _id, const vec<expr*>& _argsId, body* __body)
   : id(_id), argsId(_argsId), _body(__body)
{
   
   if (_body != nullptr) for (auto& s : _body->stmts) if (s != nullptr) stmtPool.push_back(stmt_ptr(s));
}

lam::lam(const vec<expr*>& _argsId, stmt* _s)
   : argsId(_argsId), s(_s)
{
   for (auto& arg : _argsId) if (arg != nullptr) exprPool.push_back(expr_ptr(arg));
   if (_s != nullptr) stmtPool.push_back(stmt_ptr(_s));
}

assign::assign(string _id, expr* _e) : id(_id), e(_e) { if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }

mapa::mapa(expr* _id, expr* _e) : id(_id), e(_e) { if (_id != nullptr) exprPool.push_back(expr_ptr(_id)); if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }

assert::assert(expr* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }

ret::ret(expr* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }
body::body(const vec<stmt*>& _stmts) : stmts(_stmts) { for (auto& stmt : _stmts)if (stmt != nullptr)stmtPool.push_back(stmt_ptr(stmt)); }
_break::_break(expr* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }
_continue::_continue(expr* _e) : e(_e) { if (_e != nullptr) exprPool.push_back(expr_ptr(_e)); }

STRUCT::STRUCT(string _id, STRUCT* _parent, const vec<assign*>& _fields, const vec<func*>& _methods)
   : id(_id), parent(_parent), fields(_fields), methods(_methods)
{
   for (auto& m : _methods) if (m != nullptr) { funcPool.push_back(func_ptr(m)); methodIds.insert(m->id); }
   for (auto& v : _fields)  if (v != nullptr) { stmtPool.push_back(stmt_ptr(v));  fieldIds.insert(v->id); }
   if (_parent != nullptr) {
      for (auto& m : _parent->methods) if (m != nullptr && !methodIds.count(m->id)) methods.push_back(m);
      for (auto& v : _parent->fields)  if (v != nullptr && !fieldIds.count(v->id))  fields.push_back(v);
   }
}
