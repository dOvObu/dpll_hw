#include "parser.h"
using namespace std;

void parser::parse(const vec<token>& t)
{
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]

   for (size_t idx = 0; idx < t.size() - 1 && curr.tok != tok::eof; ++idx) {
      //tok prewTok = prew.tok;
      tok currTok = curr.tok;
      tok nextTok = next.tok;

      switchStateTo(getCurrentState(), t, idx);
   }
#undef curr
#undef next
#undef prew
}

void parser::switchStateTo(state currentState, const vec<token>& t, size_t& idx)
{
   switch (currentState) {
   case state::prs_stmt:                   parseStmt(t, idx);      break;
   case state::prs_break:                  parseBreak(t, idx);     break;
   case state::prs_return:                 parseReturn(t, idx);    break;
   case state::prs_body:                   parseBody(t, idx);      break;
   case state::prs_continue:               parseContinue(t, idx);  break;
   case state::prs_assign:                 parseAssign(t, idx);    break;
   case state::prs_mapa:                   parseMapa(t, idx);      break;
   case state::prs_stmt_expr:              parseStmtExpr(t, idx);  break;
   case state::prs_one_expr:               parseOneExpr(t, idx);   break;
   case state::prs_lam:                    parseLambda(t, idx);    break;
   case state::prs_operator:               parseOperator(t, idx);  break;
   case state::prs_num:                    parseNum(t, idx);       break;
   case state::prs_id:                     parseId(t, idx);        break;
   case state::prs_var:                    parseVar(t, idx);       break;
   case state::prs_str:                    parseStr(t, idx);       break;
   case state::prs_func:                   parseFunc(t, idx);      break;
   case state::prs_call:                   parseCall(t, idx);      break;
   case state::prs_while:                  parseWhile(t, idx);     break;
   case state::prs_if:                     parseIf(t, idx);        break;
   case state::prs_ifel:                   parseIfel(t, idx);      break;
   case state::prs_for:                    parseFor(t, idx);       break;
   case state::prs_forch:                  parseForEach(t, idx);   break;
   case state::prs_pholdr:                 parsePlaceholder(idx);  break;
   case state::prs_expr_before_first_call:
   case state::prs_expr:                   parseExpr(t, idx);      break;
   default: break;
   }
}
// ------------------------------------- parser's tools
int parser::getPriorityFor(tok type)
{
   int priority{ 0 };

        if (type == tok::colon                             ) priority = 10;
   else if (type == tok::dot                               ) priority =  9;
   else if (type == tok::_not     || type == tok::un_pluss
      ||    type == tok::un_minus                          ) priority =  8;
   else if (type == tok::multiply || type == tok::divide   ) priority =  7;
   else if (type == tok::pluss    || type == tok::minus    ) priority =  6;
   else if (type == tok::less     || type == tok::more
      ||    type == tok::less_eq  || type == tok::more_eq  ) priority =  5;
   else if (type == tok::equal                             ) priority =  4;
   else if (type == tok::therefore                         ) priority =  3;
   else if (type == tok::_and                              ) priority =  2;
   else if (type == tok::_or                               ) priority =  1;
   
   priority += depth;
   return priority;
}

parser::assoc parser::getAssociativityFor(tok type)
{
   assoc assoc{ assoc::left };
   if (type == tok::colon || type == tok::_not) assoc = assoc::right;
   return assoc;
}

bool parser::hasStateInHistory(parser::state _state)
{
   bool res = false;
   size_t i = s.size() - 1;
   while (true) {
      if (s[i] == _state) {
         res = true;
         break;
      }
      if (i == 0) break;
      --i;
   }
   return res;
}

void parser::goBackInHistoryTo(parser::state _state)
{
   while (true) {
      if (getCurrentState() == _state || s.empty()) break;
      popState();
   }
}

void parser::addOperator(tok type)
{
#define tryWorkOnOp(_Token, _T) if (type == _Token) { addOperatorTmpl<_T>(type); }
   tryWorkOnOp(tok::pluss     ,  add       )
   tryWorkOnOp(tok::minus     ,  dif       )
   tryWorkOnOp(tok::multiply  ,  mul       )
   tryWorkOnOp(tok::divide    ,  sep       )
   tryWorkOnOp(tok::dot       ,  dot       )
   tryWorkOnOp(tok::colon     ,  col       )
   tryWorkOnOp(tok::less      ,  _less     )
   tryWorkOnOp(tok::more      ,  more      )
   tryWorkOnOp(tok::less_eq   ,  less_eq   )
   tryWorkOnOp(tok::more_eq   ,  more_eq   )
   tryWorkOnOp(tok::equal     ,  _equal    )
   tryWorkOnOp(tok::therefore ,  therefore )
   tryWorkOnOp(tok::_and      ,  _and      )
   tryWorkOnOp(tok::_or       ,  _or       )
#undef tryWorkOnOp
}

void parser::evalLastOperatorToExpr()
{
   tok type = stack_of_operators.back().op.type;
   if (!stack_of_expressions.empty())
      if (stack_of_expressions.back() == nullptr || (stack_of_expressions.size() > 1 && stack_of_expressions[stack_of_expressions.size() - 2] == nullptr)) {
         cout << "\nevolution failed, becouse " << (stack_of_expressions.back() == nullptr ? "right" : "left") << " argument is nil\n";
         return;
      }
#define tryWorkOnOp(_Token, _T) if (type == _Token) { evalLastOperatorToExprTmpl<_T>(); }
   tryWorkOnOp(tok::pluss     ,  add       )
   tryWorkOnOp(tok::minus     ,  dif       )
   tryWorkOnOp(tok::multiply  ,  mul       )
   tryWorkOnOp(tok::divide    ,  sep       )
   tryWorkOnOp(tok::dot       ,  dot       )
   tryWorkOnOp(tok::colon     ,  col       )
   tryWorkOnOp(tok::less      ,  _less     )
   tryWorkOnOp(tok::more      ,  more      )
   tryWorkOnOp(tok::less_eq   ,  less_eq   )
   tryWorkOnOp(tok::more_eq   ,  more_eq   )
   tryWorkOnOp(tok::equal     ,  _equal    )
   tryWorkOnOp(tok::therefore ,  therefore )
   tryWorkOnOp(tok::_and      ,  _and      )
   tryWorkOnOp(tok::_or       ,  _or       )
#undef tryWorkOnOp
/*
#define tryWorkOnOp(_Token, _T) if (type == _Token) { evalLastUnOperatorToExprTmpl<_T>(); }
      tryWorkOnOp(tok::un_pluss, un_add)
      tryWorkOnOp(tok::un_minus, un_dif)
      tryWorkOnOp(tok::_not, _not)
#undef tryWorkOnOp
//*/
      printer writer;
   cout << "\nafter last evolution of operator to expr:\n   ";
   stack_of_expressions.back()->enter(&writer);
   cout << endl;
}

void parser::evalLastUnOperatorToExpr()
{
   tok type = stack_of_operators.back().op.type;
   if (!stack_of_one_expr_num_of_exprs.empty()) stack_of_one_expr_num_of_exprs.pop_back();
   if (!stack_of_expressions.empty())
      if (stack_of_expressions.back() == nullptr) {
         cout << "\nevolution failed, becouse right argument is nil\n";
         return;
      }
#define tryWorkOnOp(_Token, _T) if (type == _Token) { evalLastUnOperatorToExprTmpl<_T>(); }
      tryWorkOnOp(tok::un_pluss, un_add)
      tryWorkOnOp(tok::un_minus, un_dif)
      tryWorkOnOp(tok::_not, _not)
#undef tryWorkOnOp
      printer writer;
   cout << "\nafter last evolution of operator to expr:\n   ";
   stack_of_expressions.back()->enter(&writer);
   cout << endl;
}

stmt* parser::addStmt(stmt* s)
{
   stmtPool.push_back(stmt_ptr(s));
   stack_of_statements.push_back(s);
   return s;
}

expr* parser::addExpr(expr* e)
{
   exprPool.push_back(expr_ptr(e));
   stack_of_expressions.push_back(e);
   return e;
}

call* parser::addCall(call* c)
{
   exprPool.push_back(expr_ptr(c));
   stack_of_calls.push_back({ c, depth });
   return c;
}

void parser::addFunc(func* p)
{
   funcPool.push_back(func_ptr(p));
}

void parser::addStruct(STRUCT* p)
{
   structPool.push_back(struct_ptr(p));
}

parser::state parser::getCurrentState()
{
   return s.back();
}

void parser::setState(parser::state state)
{
   s.push_back(state);
}

parser::state parser::popState()
{
   auto state = s.back();
   s.pop_back();
   return state;
}

#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]

void parser::parseStmt(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::eol) {
      ++idx;
   }
   if (curr.tok == tok::id) {
      if (next.tok == tok::assign) {
         unloadExprsToStmts();
         setState(stack_of_expressions.empty() ? state::prs_assign : state::prs_mapa);
      }
   }
   else if (curr.tok == tok::_break) {
      unloadExprsToStmts();
      setState(state::prs_break);
   }
   else if (curr.tok == tok::_return) {
      unloadExprsToStmts();
      setState(state::prs_return);
   }
   else if (curr.tok == tok::_continue) {
      unloadExprsToStmts();
      setState(state::prs_continue);
   }
   else if (curr.tok == tok::_if) {
      unloadExprsToStmts();
      setState(state::prs_if);
   }
   else if (curr.tok == tok::_while) {
      unloadExprsToStmts();
      setState(state::prs_while);
   }
   else if (curr.tok == tok::_for) {
      unloadExprsToStmts();
      while (!stack_of_expressions.empty()) {
         stack_of_statements.push_back(stack_of_expressions.back());
         stack_of_expressions.pop_back();
      }
      setState(state::prs_for);
   }
   else if (curr.tok == tok::_func || next.tok == tok::_func) {
      while (!stack_of_expressions.empty()) {
         stack_of_statements.push_back(stack_of_expressions.back());
         stack_of_expressions.pop_back();
      }
      if (next.tok == tok::_func) {
         ++idx;
      }
      setState(state::prs_func);
   }
   if (getCurrentState() == state::prs_stmt) {
      if (curr.tok == tok::close_br || next.tok == tok::close_br) {
         popState();
      }
      else {
         setState(state::prs_stmt_expr);
         idx = freezeEolExprStmtScore;
      }
   }

   --idx;
}

void parser::parseReturn(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_return) {
      setState(state::prs_expr);
      addStmt(new ret(nullptr));
      --idx;
   }
   else {
      popState();
      ret* r = (ret*)stack_of_statements.back();
      if (stack_of_expressions.size() == 1) {
         r->e = stack_of_expressions.back();
         stack_of_expressions.clear();
      }
      else {
         seq* sq;
         r->e = addExpr(sq = new seq({}));
         stack_of_expressions.pop_back();
         swap(sq->s, stack_of_expressions);
      }
   }
}

void parser::parseBreak(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_break) {
      setState(state::prs_expr);
      addStmt(new _break(nullptr));
      --idx;
   }
   else {
      popState();
      if (!stack_of_expressions.empty()) {
         ((_break*)stack_of_statements.back())->e = stack_of_expressions.back();
         stack_of_expressions.pop_back();
      }
   }
}

void parser::parseContinue(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_continue) {
      setState(state::prs_expr);
      addStmt(new _continue(nullptr));
      --idx;
   }
   else {
      popState();
      if (!stack_of_expressions.empty()) {
         ((_continue*)stack_of_statements.back())->e = stack_of_expressions.back();
         stack_of_expressions.pop_back();
      }
   }
}

void parser::parseBody(const vec<token>& t, size_t & idx)
{
   if ((curr.tok == tok::eol && next.tok != tok::close_br) || curr.tok == tok::open_br) {
      setState(state::prs_stmt);
      if (curr.tok != tok::open_br) --idx;
      else freezeEolExprStmtScore = idx;
      return;
   }
   if (next.tok == tok::close_br || curr.tok == tok::close_br) {
      //if (next.tok == tok::close_br) ++idx;
      if (curr.tok == tok::close_br) {
         freezeEolExprStmtScore = idx;
         if (s.size() > 1 && s[s.size()-2] == state::prs_func) ++idx;
      }
      body* body_ptr;
      stmtPool.push_back(stmt_ptr(body_ptr = new body({})));
      unloadExprsToStmts();
      swap(body_ptr->stmts, stack_of_statements);
      stack_of_statements.push_back(body_ptr);
      popState();
   }
}

void parser::parseAssign(const vec<token>& t, size_t& idx)
{
   if (next.tok == tok::assign && curr.tok == tok::id) {
      unloadExprsToStmts();
      addStmt(new assign(curr.str_val, nullptr));
      setState(state::prs_expr);
   }
   else if (curr.tok == tok::eol) {
      assign* stmt = (assign*)(stack_of_statements.back());
      if (stack_of_expressions.size() > 1) {
         stmt->e = addExpr(new seq(stack_of_expressions));
      }
      else {
         if (!stack_of_expressions.empty()) {
            stmt->e = stack_of_expressions[0];
         } else {
            stmt->e = addExpr(new seq({}));
         }
      }
      stack_of_expressions.clear();
      popState();
   }
}

void parser::parseMapa(const vec<token>& t, size_t& idx)
{
   if (next.tok == tok::assign && !stack_of_expressions.empty()) {
      mapa* ptr;
      addStmt(ptr = new mapa(nullptr, nullptr));
      if (stack_of_expressions.size() > 1) {
         seq* s;
         ptr->id = addExpr(s = new seq({}));
         stack_of_expressions.pop_back();
         s->s = stack_of_expressions;
      }
      else {
         ptr->id = stack_of_expressions.back();
      }
      stack_of_expressions.clear();
      setState(state::prs_expr);
   }
   if (curr.tok == tok::eol) {
      mapa* stmt = (mapa*)(stack_of_statements.back());
      stmt->e = stack_of_expressions.back();
      stack_of_expressions.pop_back();
      popState();
      if (getCurrentState() == state::prs_stmt_expr) popState();
   }
}

void parser::parseStmtExpr(const vec<token>& t, size_t& idx)
{
   if (stack_of_expressions.empty() && (freezeEolExprStmtScore == idx || curr.tok != tok::eol)) {
      setState(state::prs_expr);
   }
   else {
      size_t size = stack_of_expressions.size();
      if (size > 1) {
         seq* sq;
         addStmt(sq = new seq({}));
         sq->s = stack_of_expressions;
      }
      else if (size == 1) {
         stack_of_statements.push_back(stack_of_expressions[0]);
      }
      stack_of_expressions.clear();
      popState();
      //if (curr.tok == tok::eol) freezeEolExprStmtScore = idx;
      ++idx;
   }
   --idx;
}

void parser::parseExpr(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::open_par) {
      parseOpnPar(t, idx);
   }
   else if (curr.tok == tok::close_par && idx != freezeClsPar) {
      parseClsPar(t, idx);
      if (getCurrentState() == state::prs_expr_before_first_call) {
         popState();
         return;
      }
   }
   else if (curr.tok == tok::eol && freezeEolExprStmtScore != idx){
      if (depth == 0 && !keyWords.count(prew.tok)) {
         popState();
         while (!stack_of_operators.empty()) {
            if (un_operators.count(stack_of_operators.back().op.type)) {
               evalLastUnOperatorToExpr();
            }
            else {
               evalLastOperatorToExpr();
            }
         }
         //if (getCurrentState() != state::prs_stmt_expr) {
         freezeEolExprStmtScore = idx;
         //}
         if (getCurrentState() == state::prs_stmt_expr) {
            popState();
         }
         --idx;
         return;
      }
   }
   if (next.tok == tok::id) {
      setState(state::prs_id);
   }
   else if (next.tok == tok::num_lit) {
      setState(state::prs_num);
   }
   else if (next.tok == tok::str_lit) {
      setState(state::prs_str);
   }
   else if (next.tok == tok::lam) {
      setState(state::prs_lam);
   }
   else if (next.tok == tok::_if) {
      setState(state::prs_ifel);
   }
   else if (next.tok == tok::_for) {
      setState(state::prs_forch);
   }
   else if (next.tok == tok::_where) {
      setState(state::prs_pholdr);
   }
   if (next.tok == tok::assign) {
      popState();
      setState(state::prs_mapa);
      --idx;
   }
   if (operators.count(next.tok)) {
      setState(state::prs_operator);
   }
   if ((next.tok == tok::comma /*|| next.tok != tok::open_par*/ && idx > 0 && operators.count(prew.tok)) && !stack_of_operators.empty()) {
      evalLastOperatorToExpr();
   }
}

void parser::parseOpnPar(const vec<token>& t, size_t& idx)
{
   //if (freezeParDepthScore == 0) {
      if (idx > 0 && (prew.tok == tok::close_par || (prew.tok != tok::comma && prew.tok != tok::eol && !serviceSymbols.count(prew.tok) && !keyWords.count(prew.tok))) && !operators.count(prew.tok)) {
         tok tok = prew.tok;
         setState(state::prs_call);
         parseCall(t, idx);
      }
      else {
         stack_of_parentheses.push_back({ {},{},{},{} });
         stack_of_parentheses.back().swapWith(this);
      }
      depth += stepDepth;
   //} else {
   //   --freezeParDepthScore;
   //}
}

void parser::parseClsPar(const vec<token>& t, size_t& idx)
{
   if (freezeClsPar != idx) {
         depth -= stepDepth;
         if (s.size() < 2 || (s[s.size() - 2] == state::prs_call && !stack_of_calls.empty() && stack_of_calls.back().depth == depth)) {

            popState();
            parseCall(t, idx);

            if (hasStateInHistory(state::prs_expr_before_first_call)) {
               goBackInHistoryTo(state::prs_expr_before_first_call);
            }
         }
         else {
            if (!stack_of_parentheses.empty()) {
               while (!stack_of_operators.empty()) {
                  if (un_operators.count(stack_of_operators.back().op.type)) {
                     evalLastUnOperatorToExpr();
                  }else {
                     evalLastOperatorToExpr();
                  }
               }
               if (stack_of_expressions.size() > 1) {
                  expr* expr = new seq(stack_of_expressions);
                  addExpr(expr);
                  stack_of_expressions.clear();
                  stack_of_expressions.push_back(expr);
                  if (curr.tok == tok::close_par) freezeClsPar = idx;
               }
               { // конкатенация expr-ов до скобок и из скобок
                  auto& lastPar = stack_of_parentheses.back();
                  auto& target = lastPar.stack_of_expressions;
                  if (!stack_of_expressions.empty()) {
                     target.push_back(stack_of_expressions[0]);
                  }
                  lastPar.swapWith(this);
               }
               stack_of_parentheses.pop_back();
            }
         }
   //} else {
   //   --freezeParDepthScore;
   }
}

void parser::parseOperator(const vec<token>& t, size_t& idx)
{
   popState();

   if (!stack_of_operators.empty() && !operators.count(prew.tok)) {
      const int& newPriority = getPriorityFor(curr.tok);
      const int& lastPriority = stack_of_operators.back().priority;
      const auto& assoc = getAssociativityFor(curr.tok);
      if (lastPriority > newPriority || (assoc == assoc::left && lastPriority == newPriority)) {
         if (stack_of_expressions.size() > 1 && !operators.count(prew.tok)) {
            evalLastOperatorToExpr();
         }
         else {
            evalLastUnOperatorToExpr();
         }
      }
   }
   if (stack_of_expressions.size() < stack_of_operators.size() + 1 || prew.tok == tok::comma) {
#define tryWorkOnOp(_Token, _Token2, _T) if (curr.tok == _Token) { addUnOperatorTmpl<_T>(_Token2, t, idx); }
      tryWorkOnOp(tok::pluss , tok::un_pluss , un_add )
      tryWorkOnOp(tok::minus , tok::un_minus , un_dif )
      tryWorkOnOp(tok::_not  , tok::_not     , _not   )
#undef tryWorkOnOp
      //if (prew.tok == tok::close_par) --idx;
   }
   else {
      addOperator(curr.tok);
      --idx;
   }
}

void parser::parseNum(const vec<token>& t, size_t& idx)
{
   popState();
   addExpr(new num(curr.int_val));
   --idx;
}

void parser::parseId(const vec<token>& t, size_t& idx)
{
   popState();
   setState(state::prs_var);
   --idx;
}

void parser::parseVar(const vec<token>& t, size_t& idx)
{
   popState();
   addExpr(new var(curr.str_val));
   --idx;
}

void parser::parseStr(const vec<token>& t, size_t& idx)
{
   popState();
   addExpr(new str(curr.str_val));
   --idx;
}

void parser::parseFunc(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_func) {
      --idx;
      setState(state::prs_expr_before_first_call);
      return;
   }
   if (curr.tok == tok::close_par || prew.tok == tok::close_par) {
      if (prew.tok == tok::close_par) {
         --idx;
      }
      call* seq_ptr = (call*)stack_of_expressions.back();
      func* func_ptr;
      addFunc(func_ptr = new func(((var*)seq_ptr->id)->id, {}, nullptr));
      func_ptr->argsId = seq_ptr->args;

      rmExprFromPool(seq_ptr->id);
      stack_of_expressions.pop_back();
      rmExprFromPool(seq_ptr);
      //--freezeParDepthScore;
      //depth -= stepDepth;
      while (next.tok != tok::open_br) ++idx;

      if (next.tok == tok::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^

         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         func_ptr->_body = (body*)stack_of_statements.back();
         stack_of_statements.pop_back();
         /*
         while (!stack_of_expressions.empty()) {
            func_ptr->_body->stmts.push_back(stack_of_expressions.back());
            stack_of_expressions.pop_back();
         }
         //*/
         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

      }
      --idx;
      popState();
   }
}

void parser::parseCall(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::open_par) {
      addCall(new call(nullptr, {}));
      stack_of_calls.back().call->id = stack_of_expressions.back();
      stack_of_expressions.pop_back();
      swap(stack_of_calls.back().call->args, stack_of_expressions);
      swap(stack_of_calls.back().stack_of_operators, stack_of_operators);
      setState(state::prs_expr);
      return;
   }
   if (curr.tok == tok::close_par) {
      popState();
      while (!stack_of_operators.empty()) {
         if (un_operators.count(stack_of_operators.back().op.type)) {
            evalLastUnOperatorToExpr();
         } else {
            evalLastOperatorToExpr();
         }
      }
      //depth += stepDepth;
      swap(stack_of_calls.back().call->args, stack_of_expressions);
      swap(stack_of_calls.back().stack_of_operators, stack_of_operators);
      stack_of_expressions.push_back(stack_of_calls.back().call);
      stack_of_calls.pop_back();
      freezeParDepthHandlerForOneStep();
      freezeClsPar = idx;
      --idx;
      return;
   }
}

void parser::parseForEach(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_for) {
      addExpr(new for_each_(nullptr, nullptr, nullptr, nullptr, nullptr));
      setState(state::prs_expr_before_first_call);
      return;
   }
   if (curr.tok == tok::close_par) {
      call* seq_ptr = (call*)stack_of_expressions.back();
      for_each_* for_ptr = (for_each_*)seq_ptr->id;
      int variant = seq_ptr->args.size();
      col* op;
      if (variant == 1) {
         op = (col*)seq_ptr->args[0];
      }
      else if (variant == 2) {
         for_ptr->idx = (var*)seq_ptr->args[0];
         op = (col*)seq_ptr->args[1];
      }
      else if (variant == 3) {
         op = (col*)seq_ptr->args[0];
         for_ptr->cond = seq_ptr->args[2];
      }
      else if (variant == 4) {
         for_ptr->idx = (var*)seq_ptr->args[0];
         op = (col*)seq_ptr->args[1];
         for_ptr->cond = seq_ptr->args[3];
      }
      else {
         abort();
      }
      
      for_ptr->val = (var*)op->l;
      for_ptr->src = op->r;

      stack_of_expressions.back() = for_ptr;
      rmExprFromPool(seq_ptr);
      //--freezeParDepthScore;
      //depth -= stepDepth;

      if (next.tok == tok::dot) {
         // for(i x :Y | pred).expr
         //                  ^
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         {
            vec<expr*> buff{};
            vec<operator_and_priority> op_buff;
            vec<call_and_depth>        call_buff;
            vec<stacks_of_par_context> par_buff;
            vec<size_t> num_of_exprs_buff;

            num_of_exprs_buff.push_back(0);
            swap(buff, stack_of_expressions);
            swap(op_buff, stack_of_operators);
            swap(call_buff, stack_of_calls);
            swap(par_buff, stack_of_parentheses);
            swap(num_of_exprs_buff, stack_of_one_expr_num_of_exprs);

            stack_of_one_expr_depths.push_back(depth);
            while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
               ++idx;
               switchStateTo(getCurrentState(), t, idx);
            }

            if (stack_of_expressions.size() == 1) {
               for_ptr->modif = stack_of_expressions[0];
            }
            else {
               seq* seq_ptr;
               for_ptr->modif = addExpr(seq_ptr = new seq({}));
               stack_of_expressions.pop_back();
               seq_ptr->s = stack_of_expressions;
            }
            stack_of_one_expr_depths.pop_back();

            swap(buff, stack_of_expressions);
            swap(op_buff, stack_of_operators);
            swap(call_buff, stack_of_calls);
            swap(par_buff, stack_of_parentheses);
            swap(num_of_exprs_buff, stack_of_one_expr_num_of_exprs);
         }
         //if (next.tok == tok::close_par) ++idx;
      }
      else {
         for_ptr->modif = for_ptr->val;
         --idx;
      }
      freezeParDepthHandlerForOneStep();
      //--idx;
      if (t[idx + 1].tok == tok::close_par) freezeClsPar = idx + 1;

      popState();
   }
}

void parser::parseStruct(const vec<token>& t, size_t & idx)
{
   /*
   if (curr.tok == tok::_class) {
      STRUCT* fork;
      //addStruct(fork = new STRUCT());
      {
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         vec<expr*> buff{};
         vec<operator_and_priority> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         //while (next.tok != tok::open_br)
         --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(tok::open_br);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         while (curr.tok != tok::open_br) ++idx;
      }
      // while a==b { stmt stmt stmt }
      //          ^

      if (curr.tok == tok::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^
         --idx;

         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         fork->body = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      popState();
   }
         //*/
         //*/
}

void parser::parseFor(const vec<token>& t, size_t& idx)
{
   if (curr.tok == tok::_for) {
      addExpr(new for_each_(nullptr, nullptr, nullptr, nullptr, nullptr));
      setState(state::prs_expr_before_first_call);
      return;
   }
   if (curr.tok == tok::close_par) {
      call* seq_ptr = (call*)stack_of_expressions.back();
      rmExprFromPool(seq_ptr->id);
      for_ch* for_ptr;
      addStmt(for_ptr = new for_ch(nullptr, nullptr, nullptr, nullptr, nullptr));
      int variant = seq_ptr->args.size();
      col* op;
      if (variant == 1) {
         op = (col*)seq_ptr->args[0];
      }
      else if (variant == 2) {
         for_ptr->idx = (var*)seq_ptr->args[0];
         op = (col*)seq_ptr->args[1];
      }
      else if (variant == 3) {
         op = (col*)seq_ptr->args[0];
         for_ptr->cond = seq_ptr->args[2];
      }
      else if (variant == 4) {
         for_ptr->idx = (var*)seq_ptr->args[0];
         op = (col*)seq_ptr->args[1];
         for_ptr->cond = seq_ptr->args[3];
      }
      else {
         abort();
      }

      for_ptr->val = (var*)op->l;
      for_ptr->src = op->r;
      
      stack_of_expressions.pop_back();
      //stack_of_expressions.pop_back();
      rmExprFromPool(seq_ptr);
      //--freezeParDepthScore;
      //depth -= stepDepth;

      if (next.tok == tok::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^

         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         for_ptr->body = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      //--idx;
      //--idx;

      popState();
   }
}

bool parser::conditionToStopParsingOneExpr(const vec<token>& t, size_t & idx)
{
#define therefore(x, y) !(x) || (y)
   bool needsToStop = false;
   if (therefore(!stack_of_one_expr_num_of_exprs.empty(), stack_of_one_expr_num_of_exprs.back() < stack_of_expressions.size())) {
      if (therefore(!stack_of_one_expr_bfr_first_tok.empty(), stack_of_one_expr_bfr_first_tok.back() == next.tok)) {
         if (therefore(!stack_of_one_expr_depths.empty(), depth == stack_of_one_expr_depths.back())) {
            if (next.tok == tok::eol) {
               needsToStop = !keyWords.count(prew.tok);
            }
            if (next.tok != tok::open_par) {
               needsToStop = !stack_of_expressions.empty();
               if (needsToStop && !stack_of_one_expr_bfr_first_tok.empty()) {
                  while (!stack_of_operators.empty()) {
                     if (stack_of_operators.size() == 1 && stack_of_operators.back().op.type == tok::colon) break;
                     //evalLastOperatorToExpr();

                     if (un_operators.count(stack_of_operators.back().op.type)) {
                        evalLastUnOperatorToExpr();
                     }
                     else {
                        evalLastOperatorToExpr();
                     }
                  }
               }
            }
         }
      }
   }

   return needsToStop;
#undef therefore
}

void parser::parseOneExpr(const vec<token>& t, size_t & idx)
{

   if (curr.tok == tok::open_par) {
      parseOpnPar(t, idx);
   }
   else if (curr.tok == tok::close_par) {
      parseClsPar(t, idx);
   }

   if (conditionToStopParsingOneExpr(t, idx)) {
      popState();
      if (curr.tok == tok::eol) {
         //if (getCurrentState() != state::prs_stmt_expr) {
            freezeEolExprStmtScore = idx;
         //}
      }
      if (!stack_of_operators.empty() && un_operators.count(stack_of_operators.back().op.type)) {
         evalLastUnOperatorToExpr();
         --idx;
      } else {
         --idx;
      }
      return;
   }

   if (next.tok == tok::id) {
      setState(state::prs_id);
   }
   else if (next.tok == tok::num_lit) {
      setState(state::prs_num);
   }
   else if (next.tok == tok::str_lit) {
      setState(state::prs_str);
   }
   else if (next.tok == tok::lam) {
      setState(state::prs_lam);
   }
   else if (next.tok == tok::_if) {
      setState(state::prs_ifel);
   }
   else if (next.tok == tok::_for) {
      setState(state::prs_forch);
   }
   else if (next.tok == tok::_where) {
      setState(state::prs_pholdr);
   }
   if (next.tok == tok::assign) {
      popState();
      setState(state::prs_mapa);
      --idx;
   }
   if (operators.count(next.tok)) {
      setState(state::prs_operator);
   }
   if (next.tok != tok::open_par && idx > 0 && operators.count(prew.tok) && !stack_of_operators.empty()) {
      if (un_operators.count(stack_of_operators.back().op.type)) {
         int x = 2 + 2;
         //evalLastUnOperatorToExpr();
      } else {
         evalLastOperatorToExpr();
      }
   }
}

void parser::parsePlaceholder(size_t& idx)
{
   while (!stack_of_operators.empty()) {
      if (un_operators.count(stack_of_operators.back().op.type)) {
         evalLastUnOperatorToExpr();
      }
      else {
         evalLastOperatorToExpr();
      }
   }
   stack_of_expressions.push_back(nullptr);
   popState();
   --idx;
}


void parser::parseLambda(const vec<token>& t, size_t & idx)
{
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]
   if (curr.tok == tok::lam) {
      addExpr(new lam({}, {}));
      setState(state::prs_expr_before_first_call);
      return;
   }
   if (prew.tok == tok::close_par) {
      call* call_ptr = (call*)stack_of_expressions.back();
      lam* lambda = (lam*)call_ptr->id;

      lambda->argsId = call_ptr->args;
      stack_of_expressions.back() = lambda;
      rmExprFromPool(call_ptr);

      //depth -= stepDepth;
      while (curr.tok == tok::eol) ++idx;
      if (curr.tok == tok::dot) {
         --idx;
         //--freezeParDepthScore;
         // lam(a b).expr
         //        ^
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         {
            ret* ret_ptr;
            lambda->s = addStmt(ret_ptr = new ret(nullptr));

            vec<expr*> buff{};
            swap(buff, stack_of_expressions);
            stack_of_one_expr_depths.push_back(depth);
            while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
               ++idx;
               switchStateTo(getCurrentState(), t, idx);
            }
            stack_of_one_expr_depths.pop_back();

            if (stack_of_expressions.size() == 1) {
               ret_ptr->e = stack_of_expressions[0];
            }
            else {
               seq* seq_ptr;
               ret_ptr->e = addExpr(seq_ptr = new seq({}));
               stack_of_expressions.pop_back();
               seq_ptr->s = stack_of_expressions;
            }
            stack_of_statements.pop_back();
            swap(buff, stack_of_expressions);
         }
         if (curr.tok != tok::close_par && next.tok == tok::close_par) {
            freezeClsPar = idx + 1;
            //++idx;
         }
         if (curr.tok == tok::close_par && !freezeParDepthScore) {
            //freezeParDepthHandlerForOneStep();
            //--idx;
         }
      }
      if (curr.tok == tok::open_br) {
         // lam(a b){ stmt stmt stmt }
         //        ^
         --idx;
         //--freezeParDepthScore;
         freezeClsPar = idx;
         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();
         //setState(state::prs_stmt);

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};
         
         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         lambda->s = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
         --idx;
      }
      popState();
   }
#undef curr
#undef next
#undef prew
}

void parser::parseWhile(const vec<token>& t, size_t & idx)
{
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]

   if (curr.tok == tok::_while) {
      while_loop* fork;
      addStmt(fork = new while_loop(nullptr, nullptr));
      {
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         vec<expr*> buff{};
         vec<operator_and_priority> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         //while (next.tok != tok::open_br)
         --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(tok::open_br);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         while (curr.tok != tok::open_br) ++idx;
      }
      // while a==b { stmt stmt stmt }
      //          ^

      if (curr.tok == tok::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^
         --idx;

         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         fork->body = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      popState();
   }
#undef curr
#undef next
#undef prew
}

void parser::parseIf(const vec<token>& t, size_t & idx)
{
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]

   if (curr.tok == tok::_if || curr.tok == tok::_elif) {
      if_else* fork;
      addStmt(fork = new if_else(nullptr, nullptr, nullptr));
      {
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         vec<expr*> buff{};
         vec<operator_and_priority> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         //if (next.tok != tok::open_br)
         --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(tok::open_br);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         while (curr.tok != tok::open_br) ++idx;
      }
      // if a==b { stmt stmt stmt }
      //       ^
      
      if (curr.tok == tok::open_br) {
         // if(a b){ stmt stmt stmt }
         //       ^
         --idx;

         setState(state::prs_body);
         size_t sizeOfTheStack = s.size();

         vec<stmt*> stmt_buff{};
         vec<expr*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         vec<operator_and_priority> op_buff;
         vec<call_and_depth>        call_buff;
         vec<stacks_of_par_context> par_buff;
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         //*
         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);
         //*/
         fork->success = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      if (t[idx + 2].tok == tok::_else || t[idx + 2].tok == tok::_elif) {

         state enter_state = (t[idx + 2].tok == tok::_else) ? state::prs_body : state::prs_if;

         idx += 1;
         //if (next.tok == tok::open_br) {
            popState();
            setState(enter_state);
            size_t sizeOfTheStack = s.size();
            //setState(state::prs_stmt);

            vec<stmt*> stmt_buff{};
            vec<expr*> expr_buff{};

            swap(stmt_buff, stack_of_statements);
            swap(expr_buff, stack_of_expressions);

            vec<operator_and_priority> op_buff;
            vec<call_and_depth>        call_buff;
            vec<stacks_of_par_context> par_buff;
            //*
            swap(op_buff, stack_of_operators);
            swap(call_buff, stack_of_calls);
            swap(par_buff, stack_of_parentheses);
            //*/
            while (s.size() > sizeOfTheStack || getCurrentState() == enter_state) {
               ++idx;
               switchStateTo(getCurrentState(), t, idx);
            }
            //*
            swap(op_buff, stack_of_operators);
            swap(call_buff, stack_of_calls);
            swap(par_buff, stack_of_parentheses);
            //*/
            fork->fail = stack_of_statements.back();
            stack_of_statements.pop_back();

            swap(stmt_buff, stack_of_statements);
            swap(expr_buff, stack_of_expressions);
         //}
      }
      popState();
   }
#undef curr
#undef next
#undef prew
}

void parser::parseIfel(const vec<token>& t, size_t & idx)
{
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]

   if (curr.tok == tok::_if) {
      ifel* fork;
      addExpr(fork = new ifel(nullptr, nullptr, nullptr));
      {
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         vec<expr*> buff{};
         vec<operator_and_priority> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         if (next.tok != tok::open_par) --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(tok::dot);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
      }
      // if a==b.expr
      //       ^
      while (next.tok != tok::dot) { ++idx; }

      {
         setState(state::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         vec<expr*> buff{};
         vec<operator_and_priority> op_buff{};
         swap(buff, stack_of_expressions);
         swap(op_buff, stack_of_operators);

         stack_of_one_expr_depths.push_back(depth);
         while (s.size() > sizeOfTheStack || getCurrentState() == state::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_depths.pop_back();
         if (stack_of_expressions.size() == 1) {
            col* op = (col*)stack_of_expressions[0];
            fork->l = op->l;
            fork->r = op->r;
            stack_of_expressions.pop_back();
            rmExprFromPool(op);
         }
         else {
            abort();
         }
         swap(op_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         //if (next.tok == tok::close_par) {
         //}
         freezeParDepthHandlerForOneStep();
         if (curr.tok == tok::close_par) freezeClsPar = idx;
         --idx;
         //--idx;
      }
      popState();
   }
#undef curr
#undef next
#undef prew
}

void parser::unloadExprsToStmts()
{
   while (!stack_of_expressions.empty()) {
      stack_of_statements.push_back(stack_of_expressions.back());
      stack_of_expressions.pop_back();
   }
}

void parser::freezeParDepthHandlerForOneStep()
{
   ++freezeParDepthScore;
}

void parser::freezeEolExprStmtHandlerForOneStep()
{
   ++freezeEolExprStmtScore;
}
