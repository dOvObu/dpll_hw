#include "parser.h"
using namespace std;
#define curr t[idx]
#define next t[idx + 1]
#define prew t[idx - 1]


void PARSER::parse(const VEC<TOKEN>& t)
{
   for (size_t idx = 0; idx < t.size() - 1 && curr.tok != TOK::eof; ++idx) {
      switchStateTo(getCurrentState(), t, idx);
   }
}


void PARSER::switchStateTo(STATE currentState, const VEC<TOKEN>& t, size_t& idx)
{
   switch (currentState) {
   case STATE::prs_stmt:                   parseStmt(t, idx);      break;
   case STATE::prs_break:                  parseBreak(t, idx);     break;
   case STATE::prs_return:                 parseReturn(t, idx);    break;
   case STATE::prs_body:                   parseBody(t, idx);      break;
   case STATE::prs_continue:               parseContinue(t, idx);  break;
   case STATE::prs_assign:                 parseAssign(t, idx);    break;
   case STATE::prs_mapa:                   parseMapa(t, idx);      break;
   case STATE::prs_stmt_expr:              parseStmtExpr(t, idx);  break;
   case STATE::prs_one_expr:               parseOneExpr(t, idx);   break;
   case STATE::prs_lam:                    parseLambda(t, idx);    break;
   case STATE::prs_operator:               parseOperator(t, idx);  break;
   case STATE::prs_num:                    parseNum(t, idx);       break;
   case STATE::prs_id:                     parseId(t, idx);        break;
   case STATE::prs_var:                    parseVar(t, idx);       break;
   case STATE::prs_str:                    parseStr(t, idx);       break;
   case STATE::prs_func:                   parseFunc(t, idx);      break;
   case STATE::prs_call:                   parseCall(t, idx);      break;
   case STATE::prs_while:                  parseWhile(t, idx);     break;
   case STATE::prs_if:                     parseIf(t, idx);        break;
   case STATE::prs_ifel:                   parseIfel(t, idx);      break;
   case STATE::prs_for:                    parseFor(t, idx);       break;
   case STATE::prs_forch:                  parseForEach(t, idx);   break;
   case STATE::prs_pholdr:                 parsePlaceholder(idx);  break;
   case STATE::prs_expr_before_first_call:
   case STATE::prs_expr:                   parseExpr(t, idx);      break;
   default: break;
   }
}
// ------------------------------------- parser's tools
int PARSER::getPriorityFor(TOK type)
{
   int priority{ 0 };

        if (type == TOK::colon                             ) priority = 10;
   else if (type == TOK::dot                               ) priority =  9;
   else if (type == TOK::_not     || type == TOK::un_pluss
      ||    type == TOK::un_minus                          ) priority =  8;
   else if (type == TOK::multiply || type == TOK::divide   ) priority =  7;
   else if (type == TOK::pluss    || type == TOK::minus    ) priority =  6;
   else if (type == TOK::less     || type == TOK::more
      ||    type == TOK::less_eq  || type == TOK::more_eq  ) priority =  5;
   else if (type == TOK::equal                             ) priority =  4;
   else if (type == TOK::therefore                         ) priority =  3;
   else if (type == TOK::_and                              ) priority =  2;
   else if (type == TOK::_or                               ) priority =  1;
   
   priority += depth;
   return priority;
}


PARSER::ASSOC PARSER::getAssociativityFor(TOK type)
{
   ASSOC ASSOC{ ASSOC::left };
   if (type == TOK::colon || type == TOK::_not) ASSOC = ASSOC::right;
   return ASSOC;
}


bool PARSER::hasStateInHistory(PARSER::STATE _state)
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


void PARSER::goBackInHistoryTo(PARSER::STATE _state)
{
   while (true) {
      if (getCurrentState() == _state || s.empty()) break;
      popState();
   }
}


void PARSER::addOperator(TOK type)
{
#define tryWorkOnOp(_Token, _T) if (type == _Token) { addOperatorTmpl<_T>(type); }
   tryWorkOnOp(TOK::pluss     ,  ADD       )
   tryWorkOnOp(TOK::minus     ,  DIF       )
   tryWorkOnOp(TOK::multiply  ,  MUL       )
   tryWorkOnOp(TOK::divide    ,  SEP       )
   tryWorkOnOp(TOK::dot       ,  DOT       )
   tryWorkOnOp(TOK::colon     ,  COL       )
   tryWorkOnOp(TOK::less      ,  LESS     )
   tryWorkOnOp(TOK::more      ,  MORE      )
   tryWorkOnOp(TOK::less_eq   ,  LESS_EQ   )
   tryWorkOnOp(TOK::more_eq   ,  MORE_EQ   )
   tryWorkOnOp(TOK::equal     ,  EQUAL    )
   tryWorkOnOp(TOK::therefore ,  THEREFORE )
   tryWorkOnOp(TOK::_and      ,  AND      )
   tryWorkOnOp(TOK::_or       ,  OR       )
#undef tryWorkOnOp
}


void PARSER::evalLastOperatorToExpr()
{
   TOK type = stack_of_operators.back().op.type;
   if (!stack_of_expressions.empty())
      if (stack_of_expressions.back() == nullptr || (stack_of_expressions.size() > 1 && stack_of_expressions[stack_of_expressions.size() - 2] == nullptr)) {
         //cout << "\nevolution failed, becouse " << (stack_of_expressions.back() == nullptr ? "right" : "left") << " argument is nil\n";
         return;
      }
#define tryWorkOnOp(_Token, _T) if (type == _Token) { evalLastOperatorToExprTmpl<_T>(); }
   tryWorkOnOp(TOK::pluss     ,  ADD       )
   tryWorkOnOp(TOK::minus     ,  DIF       )
   tryWorkOnOp(TOK::multiply  ,  MUL       )
   tryWorkOnOp(TOK::divide    ,  SEP       )
   tryWorkOnOp(TOK::dot       ,  DOT       )
   tryWorkOnOp(TOK::colon     ,  COL       )
   tryWorkOnOp(TOK::less      ,  LESS     )
   tryWorkOnOp(TOK::more      ,  MORE      )
   tryWorkOnOp(TOK::less_eq   ,  LESS_EQ   )
   tryWorkOnOp(TOK::more_eq   ,  MORE_EQ   )
   tryWorkOnOp(TOK::equal     ,  EQUAL    )
   tryWorkOnOp(TOK::therefore ,  THEREFORE )
   tryWorkOnOp(TOK::_and      ,  AND      )
   tryWorkOnOp(TOK::_or       ,  OR       )
#undef tryWorkOnOp
   //VIS_PRINTER writer;
   //cout << "\nafter last evolution of operator to expr:\n   ";
   //stack_of_expressions.back()->enter(&writer);
   //cout << endl;
}


void PARSER::evalLastUnOperatorToExpr()
{
   TOK type = stack_of_operators.back().op.type;
   if (!stack_of_one_expr_num_of_exprs.empty()) stack_of_one_expr_num_of_exprs.pop_back();
   if (!stack_of_expressions.empty())
      if (stack_of_expressions.back() == nullptr) {
         //cout << "\nevolution failed, becouse right argument is nil\n";
         return;
      }
#define tryWorkOnOp(_Token, _T) if (type == _Token) { evalLastUnOperatorToExprTmpl<_T>(); }
      tryWorkOnOp(TOK::un_pluss, UN_ADD)
      tryWorkOnOp(TOK::un_minus, UN_DIF)
      tryWorkOnOp(TOK::_not, NOT)
#undef tryWorkOnOp
   //VIS_PRINTER writer;
   //cout << "\nafter last evolution of operator to expr:\n   ";
   //stack_of_expressions.back()->enter(&writer);
   //cout << endl;
}


STMT* PARSER::addStmt(STMT* s)
{
   stmtPool.push_back(STMT_PTR(s));
   stack_of_statements.push_back(s);
   return s;
}


EXPR* PARSER::addExpr(EXPR* e)
{
   exprPool.push_back(EXPR_PTR(e));
   stack_of_expressions.push_back(e);
   return e;
}


CALL* PARSER::addCall(CALL* c)
{
   exprPool.push_back(EXPR_PTR(c));
   stack_of_calls.push_back({ c, depth });
   return c;
}


void PARSER::addFunc(FUNC* p)
{
   funcPool.push_back(FUNC_PTR(p));
}


void PARSER::addStruct(STRUCT* p)
{
   structPool.push_back(STRUCT_PTR(p));
}


PARSER::STATE PARSER::getCurrentState()
{
   return s.back();
}


void PARSER::setState(PARSER::STATE state)
{
   s.push_back(state);
}


PARSER::STATE PARSER::popState()
{
   auto state = s.back();
   s.pop_back();
   return state;
}


void PARSER::parseStmt(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::eol) ++idx;

   if (curr.tok == TOK::id) {
      if (next.tok == TOK::assign) {
         unloadExprsToStmts();
         setState(stack_of_expressions.empty() ? STATE::prs_assign : STATE::prs_mapa);
      }
   } else if (curr.tok == TOK::_break) {
      unloadExprsToStmts();
      setState(STATE::prs_break);
   } else if (curr.tok == TOK::_return) {
      unloadExprsToStmts();
      setState(STATE::prs_return);
   } else if (curr.tok == TOK::_continue) {
      unloadExprsToStmts();
      setState(STATE::prs_continue);
   } else if (curr.tok == TOK::_if) {
      unloadExprsToStmts();
      setState(STATE::prs_if);
   } else if (curr.tok == TOK::_while) {
      unloadExprsToStmts();
      setState(STATE::prs_while);
   } else if (curr.tok == TOK::_for) {
      unloadExprsToStmts();
      while (!stack_of_expressions.empty()) {
         stack_of_statements.push_back(stack_of_expressions.back());
         stack_of_expressions.pop_back();
      }
      setState(STATE::prs_for);

   } else if (curr.tok == TOK::_func || next.tok == TOK::_func) {

      while (!stack_of_expressions.empty()) {
         stack_of_statements.push_back(stack_of_expressions.back());

         stack_of_expressions.pop_back();
      }

      if (next.tok == TOK::_func) ++idx;
      setState(STATE::prs_func);
   }

   if (getCurrentState() == STATE::prs_stmt) {
      if (curr.tok == TOK::close_br || next.tok == TOK::close_br) {
         popState();
      } else {
         setState(STATE::prs_stmt_expr);
         idx = freezeEolExprStmtScore;
      }
   }
   --idx;
}


void PARSER::parseReturn(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_return) {
      setState(STATE::prs_expr);
      addStmt(new RET(nullptr));
      --idx;
   } else {
      popState();
      RET* r = (RET*)stack_of_statements.back();

      if (stack_of_expressions.size() == 1) {
         r->e = stack_of_expressions.back();
         stack_of_expressions.clear();
      } else {
         SEQ* sq;
         r->e = addExpr(sq = new SEQ({}));
         stack_of_expressions.pop_back();
         swap(sq->s, stack_of_expressions);
      }
   }
}


void PARSER::parseBreak(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_break) {
      setState(STATE::prs_expr);
      addStmt(new BREAK(nullptr));
      --idx;
   } else {
      popState();

      if (!stack_of_expressions.empty()) {
         ((BREAK*)stack_of_statements.back())->e = stack_of_expressions.back();

         stack_of_expressions.pop_back();
      }
   }
}


void PARSER::parseContinue(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_continue) {
      setState(STATE::prs_expr);
      addStmt(new CONTINUE(nullptr));
      --idx;
   } else {
      popState();

      if (!stack_of_expressions.empty()) {
         ((CONTINUE*)stack_of_statements.back())->e = stack_of_expressions.back();

         stack_of_expressions.pop_back();
      }
   }
}


void PARSER::parseBody(const VEC<TOKEN>& t, size_t & idx)
{
   if ((curr.tok == TOK::eol && next.tok != TOK::close_br) || curr.tok == TOK::open_br) {

      setState(STATE::prs_stmt);

      if (curr.tok != TOK::open_br) --idx;
      else freezeEolExprStmtScore = idx;

      return;
   }

   if (next.tok == TOK::close_br || curr.tok == TOK::close_br) {

      if (curr.tok == TOK::close_br) {
         freezeEolExprStmtScore = idx;

         if (s.size() > 1 && s[s.size()-2] == STATE::prs_func) ++idx;
      }
      BODY* body_ptr;

      stmtPool.push_back(STMT_PTR(body_ptr = new BODY({})));
      unloadExprsToStmts();
      swap(body_ptr->stmts, stack_of_statements);
      stack_of_statements.push_back(body_ptr);

      popState();
   }
}


void PARSER::parseAssign(const VEC<TOKEN>& t, size_t& idx)
{
   if (next.tok == TOK::assign && curr.tok == TOK::id) {
      unloadExprsToStmts();
      addStmt(new ASSIGN(curr.str_val, nullptr));
      setState(STATE::prs_expr);

   } else if (curr.tok == TOK::eol) {
      ASSIGN* stmt = (ASSIGN*)(stack_of_statements.back());

      if (stack_of_expressions.size() > 1) {
         stmt->e = addExpr(new SEQ(stack_of_expressions));
      } else {
         if (!stack_of_expressions.empty()) {
            stmt->e = stack_of_expressions[0];
         } else {
            stmt->e = addExpr(new SEQ({}));
         }
      }
      stack_of_expressions.clear();
      popState();
   }
}


void PARSER::parseMapa(const VEC<TOKEN>& t, size_t& idx)
{
   if (next.tok == TOK::assign && !stack_of_expressions.empty()) {
      MAPA* ptr;
      addStmt(ptr = new MAPA(nullptr, nullptr));

      if (stack_of_expressions.size() > 1) {
         SEQ* s;
         ptr->id = addExpr(s = new SEQ({}));

         stack_of_expressions.pop_back();
         s->s = stack_of_expressions;
      } else {
         ptr->id = stack_of_expressions.back();
      }

      stack_of_expressions.clear();
      setState(STATE::prs_expr);
   }

   if (curr.tok == TOK::eol) {
      MAPA* stmt = (MAPA*)(stack_of_statements.back());
      stmt->e = stack_of_expressions.back();

      stack_of_expressions.pop_back();
      popState();

      if (getCurrentState() == STATE::prs_stmt_expr) popState();
   }
}


void PARSER::parseStmtExpr(const VEC<TOKEN>& t, size_t& idx)
{
   if (stack_of_expressions.empty() && (freezeEolExprStmtScore == idx || curr.tok != TOK::eol)) {
      setState(STATE::prs_expr);
   } else {
      size_t size = stack_of_expressions.size();

      if (size > 1) {
         SEQ* sq;
         addStmt(sq = new SEQ({}));
         sq->s = stack_of_expressions;
      } else if (size == 1) {
         stack_of_statements.push_back(stack_of_expressions[0]);
      }
      stack_of_expressions.clear();
      popState();
      ++idx;
   }
   --idx;
}


void PARSER::parseExpr(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::open_par) {
      parseOpnPar(t, idx);
   } else if (curr.tok == TOK::close_par && idx != freezeClsPar) {
      parseClsPar(t, idx);

      if (getCurrentState() == STATE::prs_expr_before_first_call) {
         popState();

         return;
      }
   } else if (curr.tok == TOK::eol && freezeEolExprStmtScore != idx){

      if (depth == 0 && !keyWords.count(prew.tok)) {
         popState();

         while (!stack_of_operators.empty()) {
            if (un_operators.count(stack_of_operators.back().op.type)) {
               evalLastUnOperatorToExpr();
            } else {
               evalLastOperatorToExpr();
            }
         }
         freezeEolExprStmtScore = idx;

         if (getCurrentState() == STATE::prs_stmt_expr) {
            popState();
         }
         --idx;
         return;
      }
   }

   if (next.tok == TOK::id) {
      setState(STATE::prs_id);
   } else if (next.tok == TOK::num_lit) {
      setState(STATE::prs_num);
   } else if (next.tok == TOK::str_lit) {
      setState(STATE::prs_str);
   } else if (next.tok == TOK::lam) {
      setState(STATE::prs_lam);
   } else if (next.tok == TOK::_if) {
      setState(STATE::prs_ifel);
   } else if (next.tok == TOK::_for) {
      setState(STATE::prs_forch);
   } else if (next.tok == TOK::_where) {
      setState(STATE::prs_pholdr);
   }

   if (next.tok == TOK::assign) {
      popState();
      setState(STATE::prs_mapa);
      --idx;
   }

   if (operators.count(next.tok)) {
      setState(STATE::prs_operator);
   }

   if (((next.tok == TOK::comma /*|| (stack_of_expressions.size() > stack_of_operators.size() && next.tok != TOK::open_par)*/) && idx > 0 /*&& operators.count(prew.tok)*/) && !stack_of_operators.empty()) {
      evalLastOperatorToExpr();
   }
   else {
      cout << "";
   }
}


void PARSER::parseOpnPar(const VEC<TOKEN>& t, size_t& idx)
{
   if (idx > 0 && (prew.tok == TOK::close_par || (prew.tok != TOK::comma && prew.tok != TOK::eol && !serviceSymbols.count(prew.tok) && !keyWords.count(prew.tok))) && !operators.count(prew.tok)) {
      TOK tok = prew.tok;
      setState(STATE::prs_call);
      parseCall(t, idx);
   } else {
      stack_of_parentheses.push_back({ {},{},{},{} });
      stack_of_parentheses.back().swapWith(this);
   }
   depth += stepDepth;
}


void PARSER::parseClsPar(const VEC<TOKEN>& t, size_t& idx)
{
   if (freezeClsPar != idx) {
      depth -= stepDepth;

      if (s.size() < 2 || (s[s.size() - 2] == STATE::prs_call && !stack_of_calls.empty() && stack_of_calls.back().depth == depth)) {

         popState();
         parseCall(t, idx);

         if (hasStateInHistory(STATE::prs_expr_before_first_call)) {
            goBackInHistoryTo(STATE::prs_expr_before_first_call);
         }
      } else {

         if (!stack_of_parentheses.empty()) {

            while (!stack_of_operators.empty()) {
               if (un_operators.count(stack_of_operators.back().op.type)) {
                  evalLastUnOperatorToExpr();
               } else {
                  evalLastOperatorToExpr();
               }
            }
            // a list of free expr-s from parentheses is sequence
            if (stack_of_expressions.size() > 1) {

               EXPR* expr = new SEQ(stack_of_expressions);
               addExpr(expr);
               stack_of_expressions.clear();
               stack_of_expressions.push_back(expr);

               if (curr.tok == TOK::close_par) freezeClsPar = idx;
            }
            // concatination of expr-s before and from parentheses
            auto& lastPar = stack_of_parentheses.back();
            auto& target = lastPar.stack_of_expressions;

            if (!stack_of_expressions.empty()) {
               target.push_back(stack_of_expressions[0]);
            }
            lastPar.swapWith(this);
            stack_of_parentheses.pop_back();
         }
      }
   }
}


void PARSER::parseOperator(const VEC<TOKEN>& t, size_t& idx)
{
   popState();

   if (!stack_of_operators.empty() && !operators.count(prew.tok)) {
      const int& newPriority = getPriorityFor(curr.tok);
      const int& lastPriority = stack_of_operators.back().priority;
      const auto& ASSOC = getAssociativityFor(curr.tok);

      if (lastPriority > newPriority || (ASSOC == ASSOC::left && lastPriority == newPriority)) {

         if (stack_of_expressions.size() > 1 && !operators.count(prew.tok)) {
            evalLastOperatorToExpr();
         } else {
            evalLastUnOperatorToExpr();
         }
      }
   }

   if (stack_of_expressions.size() < stack_of_operators.size() + 1 || prew.tok == TOK::comma) {
#define tryWorkOnOp(_Token, _Token2, _T) if (curr.tok == _Token) { addUnOperatorTmpl<_T>(_Token2, t, idx); }
      tryWorkOnOp(TOK::pluss , TOK::un_pluss , UN_ADD )
      tryWorkOnOp(TOK::minus , TOK::un_minus , UN_DIF )
      tryWorkOnOp(TOK::_not  , TOK::_not     , NOT   )
#undef tryWorkOnOp
   } else {
      addOperator(curr.tok);

      --idx;
   }
}


void PARSER::parseNum(const VEC<TOKEN>& t, size_t& idx)
{
   popState();
   addExpr(new NUM(curr.int_val));
   --idx;
}


void PARSER::parseId(const VEC<TOKEN>& t, size_t& idx)
{
   popState();
   setState(STATE::prs_var);
   --idx;
}


void PARSER::parseVar(const VEC<TOKEN>& t, size_t& idx)
{
   popState();
   addExpr(new VAR(curr.str_val));
   --idx;
}


void PARSER::parseStr(const VEC<TOKEN>& t, size_t& idx)
{
   popState();
   addExpr(new STR(curr.str_val));
   --idx;
}


void PARSER::parseFunc(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_func) {
      setState(STATE::prs_expr_before_first_call);
      --idx;
      return;
   }

   if (curr.tok == TOK::close_par || prew.tok == TOK::close_par) {

      CALL* seq_ptr = (CALL*)stack_of_expressions.back();
      FUNC* func_ptr;

      addFunc(func_ptr = new FUNC(((VAR*)seq_ptr->id)->id, {}, nullptr));
      func_ptr->argsId = seq_ptr->args;

      rmExprFromPool(seq_ptr->id);
      stack_of_expressions.pop_back();
      rmExprFromPool(seq_ptr);
      if (prew.tok == TOK::close_par)  --idx;

      while (next.tok != TOK::open_br) ++idx;

      if (next.tok == TOK::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^
         setState(STATE::prs_body);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_body) {
            ++idx;

            switchStateTo(getCurrentState(), t, idx);
         }

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         func_ptr->_body = (BODY*)stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

      }
      --idx;
      popState();
   }
}


void PARSER::parseCall(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::open_par) {
      addCall(new CALL(nullptr, {}));
      stack_of_calls.back().call->id = stack_of_expressions.back();
      stack_of_expressions.pop_back();
      swap(stack_of_calls.back().call->args, stack_of_expressions);
      swap(stack_of_calls.back().stack_of_operators, stack_of_operators);
      setState(STATE::prs_expr);
      return;
   }

   if (curr.tok == TOK::close_par) {
      popState();
      while (!stack_of_operators.empty()) {
         if (un_operators.count(stack_of_operators.back().op.type)) {
            evalLastUnOperatorToExpr();
         } else {
            evalLastOperatorToExpr();
         }
      }
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


void PARSER::parseForEach(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_for) {
      addExpr(new FOR_EACH(nullptr, nullptr, nullptr, nullptr, nullptr));
      setState(STATE::prs_expr_before_first_call);
      return;
   }

   if (curr.tok == TOK::close_par) {
      CALL*      seq_ptr = (CALL*)stack_of_expressions.back();

      FOR_EACH* for_ptr = (FOR_EACH*)seq_ptr->id;
      int        variant = seq_ptr->args.size();
      COL*       op;

      if (variant == 1) {
         op = (COL*)seq_ptr->args[0];

      } else if (variant == 2) {
         for_ptr->idx = (VAR*)seq_ptr->args[0];
         op = (COL*)seq_ptr->args[1];

      } else if (variant == 3) {
         op = (COL*)seq_ptr->args[0];
         for_ptr->cond = seq_ptr->args[2];

      } else if (variant == 4) {
         for_ptr->idx = (VAR*)seq_ptr->args[0];
         op = (COL*)seq_ptr->args[1];
         for_ptr->cond = seq_ptr->args[3];

      } else {
         abort();
      }
      
      for_ptr->val = (VAR*)op->l;
      for_ptr->src = op->r;

      stack_of_expressions.back() = for_ptr;
      rmExprFromPool(seq_ptr);

      if (next.tok == TOK::dot) {
         // for(i x :Y | pred).expr
         //                  ^
         setState(STATE::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         {
            VEC<EXPR*>                 buff{};
            VEC<OPERATOR_AND_PRIORITY> op_buff;
            VEC<CALL_AND_DEPTH>        call_buff;
            VEC<STACK_OF_PAR_CONTEXT> par_buff;
            VEC<size_t>                num_of_exprs_buff;

            num_of_exprs_buff.push_back(0);
            swap(buff, stack_of_expressions);
            swap(op_buff, stack_of_operators);
            swap(call_buff, stack_of_calls);
            swap(par_buff, stack_of_parentheses);
            swap(num_of_exprs_buff, stack_of_one_expr_num_of_exprs);

            stack_of_one_expr_depths.push_back(depth);
            while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
               ++idx;

               switchStateTo(getCurrentState(), t, idx);
            }

            if (stack_of_expressions.size() == 1) {
               for_ptr->modif = stack_of_expressions[0];
            } else {
               SEQ* seq_ptr;
               for_ptr->modif = addExpr(seq_ptr = new SEQ({}));
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
      } else {
         for_ptr->modif = for_ptr->val;
         --idx;
      }
      freezeParDepthHandlerForOneStep();

      if (t[idx + 1].tok == TOK::close_par) freezeClsPar = idx + 1;
      popState();
   }
}


void PARSER::parseStruct(const VEC<TOKEN>& t, size_t & idx)
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
}


void PARSER::parseFor(const VEC<TOKEN>& t, size_t& idx)
{
   if (curr.tok == TOK::_for) {
      addExpr(new FOR_EACH(nullptr, nullptr, nullptr, nullptr, nullptr));
      setState(STATE::prs_expr_before_first_call);
      return;
   }

   if (curr.tok == TOK::close_par) {
      CALL* seq_ptr = (CALL*)stack_of_expressions.back();
      rmExprFromPool(seq_ptr->id);

      FOR_CH* for_ptr;
      addStmt(for_ptr = new FOR_CH(nullptr, nullptr, nullptr, nullptr, nullptr));

      int  variant = seq_ptr->args.size();
      COL* op;

      if (variant == 1) {
         op = (COL*)seq_ptr->args[0];
      } else if (variant == 2) {
         for_ptr->idx = (VAR*)seq_ptr->args[0];
         op = (COL*)seq_ptr->args[1];
      } else if (variant == 3) {
         op = (COL*)seq_ptr->args[0];
         for_ptr->cond = seq_ptr->args[2];
      } else if (variant == 4) {
         for_ptr->idx = (VAR*)seq_ptr->args[0];
         op = (COL*)seq_ptr->args[1];
         for_ptr->cond = seq_ptr->args[3];
      } else {
         abort();
      }

      for_ptr->val = (VAR*)op->l;
      for_ptr->src = op->r;
      
      stack_of_expressions.pop_back();
      rmExprFromPool(seq_ptr);

      if (next.tok == TOK::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^
         setState(STATE::prs_body);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         for_ptr->body = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      popState();
   }
}

bool PARSER::conditionToStopParsingOneExpr(const VEC<TOKEN>& t, size_t & idx)
{
   bool needsToStop = false;
   bool mayStop =
      (stack_of_one_expr_num_of_exprs .empty() || stack_of_one_expr_num_of_exprs.back() < stack_of_expressions.size()) &&
      (stack_of_one_expr_bfr_first_tok.empty() || stack_of_one_expr_bfr_first_tok.back() == next.tok) &&
      (stack_of_one_expr_depths       .empty() || depth == stack_of_one_expr_depths.back())
      ;

   if (mayStop) {

      if (next.tok == TOK::eol) {
         needsToStop = !keyWords.count(prew.tok);
      }

      if (next.tok != TOK::open_par) {
         needsToStop = !stack_of_expressions.empty();

         if (needsToStop && !stack_of_one_expr_bfr_first_tok.empty()) {

            while (!stack_of_operators.empty()) {

               if (stack_of_operators.size() == 1 && stack_of_operators.back().op.type == TOK::colon) {
                  break;
               }
               if (un_operators.count(stack_of_operators.back().op.type)) {
                  evalLastUnOperatorToExpr();
               } else {
                  evalLastOperatorToExpr();
               }
            }
         }
      }
   }
   return needsToStop;
}


void PARSER::parseOneExpr(const VEC<TOKEN>& t, size_t & idx)
{

   if (curr.tok == TOK::open_par) {
      parseOpnPar(t, idx);
   } else if (curr.tok == TOK::close_par) {
      parseClsPar(t, idx);
   }

   if (conditionToStopParsingOneExpr(t, idx)) {
      popState();
      if (curr.tok == TOK::eol) {
         freezeEolExprStmtScore = idx;
      }

      if (!stack_of_operators.empty() && un_operators.count(stack_of_operators.back().op.type)) {
         evalLastUnOperatorToExpr();
         --idx;
      } else {
         --idx;
      }
      return;
   }

   if (next.tok == TOK::id) {
      setState(STATE::prs_id);
   } else if (next.tok == TOK::num_lit) {
      setState(STATE::prs_num);
   } else if (next.tok == TOK::str_lit) {
      setState(STATE::prs_str);
   } else if (next.tok == TOK::lam) {
      setState(STATE::prs_lam);
   } else if (next.tok == TOK::_if) {
      setState(STATE::prs_ifel);
   } else if (next.tok == TOK::_for) {
      setState(STATE::prs_forch);
   } else if (next.tok == TOK::_where) {
      setState(STATE::prs_pholdr);
   }

   if (next.tok == TOK::assign) {
      popState();
      setState(STATE::prs_mapa);
      --idx;
   }

   if (operators.count(next.tok)) {
      setState(STATE::prs_operator);
   }

   if (next.tok != TOK::open_par && idx > 0 && stack_of_expressions.size() > stack_of_operators.size()/*&& operators.count(prew.tok)*/ && !stack_of_operators.empty()) {
      if (!un_operators.count(stack_of_operators.back().op.type)) {
         evalLastOperatorToExpr();
      }
   }
}


void PARSER::parsePlaceholder(size_t& idx)
{
   while (!stack_of_operators.empty()) {
      if (un_operators.count(stack_of_operators.back().op.type)) {
         evalLastUnOperatorToExpr();
      } else {
         evalLastOperatorToExpr();
      }
   }
   stack_of_expressions.push_back(nullptr);
   popState();
   --idx;
}


// sorry
void PARSER::parseLambda(const VEC<TOKEN>& t, size_t & idx)
{
   if (curr.tok == TOK::lam) {
      addExpr(new LAM({}, {}));
      setState(STATE::prs_expr_before_first_call);
      return;
   }

   if (prew.tok == TOK::close_par) {
      CALL* call_ptr = (CALL*)stack_of_expressions.back();
      LAM* lambda = (LAM*)call_ptr->id;

      lambda->argsId = call_ptr->args;
      stack_of_expressions.back() = lambda;
      rmExprFromPool(call_ptr);

      while (curr.tok == TOK::eol) ++idx;
      if (curr.tok == TOK::dot) {
         // lam(a b).expr
         //        ^
         --idx;
         setState(STATE::prs_one_expr);

         size_t sizeOfTheStack = s.size();
         {
            RET* ret_ptr;
            lambda->s = addStmt(ret_ptr = new RET(nullptr));

            VEC<EXPR*> buff{};
            swap(buff, stack_of_expressions);
            stack_of_one_expr_depths.push_back(depth);
            while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
               ++idx;
               switchStateTo(getCurrentState(), t, idx);
            }
            stack_of_one_expr_depths.pop_back();

            if (stack_of_expressions.size() == 1) {
               ret_ptr->e = stack_of_expressions[0];
            } else {
               SEQ* seq_ptr;
               ret_ptr->e = addExpr(seq_ptr = new SEQ({}));
               stack_of_expressions.pop_back();
               seq_ptr->s = stack_of_expressions;
            }
            stack_of_statements.pop_back();
            swap(buff, stack_of_expressions);
         }
         if (curr.tok != TOK::close_par && next.tok == TOK::close_par) {
            freezeClsPar = idx + 1;
         }
      }
      if (curr.tok == TOK::open_br) {
         // lam(a b){ stmt stmt stmt }
         //        ^
         --idx;
         freezeClsPar = idx;
         setState(STATE::prs_body);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};
         
         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         lambda->s = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
         --idx;
      }
      popState();
   }
}


void PARSER::parseWhile(const VEC<TOKEN>& t, size_t & idx)
{
   if (curr.tok == TOK::_while) {
      WHILE_LOOP* fork;
      addStmt(fork = new WHILE_LOOP(nullptr, nullptr));
      {
         setState(STATE::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         VEC<EXPR*> buff{};
         VEC<OPERATOR_AND_PRIORITY> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(TOK::open_br);
         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }
         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         while (curr.tok != TOK::open_br) ++idx;
      }
      // while a==b { stmt stmt stmt }
      //          ^

      if (curr.tok == TOK::open_br) {
         // while(a b){ stmt stmt stmt }
         //          ^
         --idx;

         setState(STATE::prs_body);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_body) {
            ++idx;
            switchStateTo(getCurrentState(), t, idx);
         }

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
}

void PARSER::parseIf(const VEC<TOKEN>& t, size_t & idx)
{
   if (curr.tok == TOK::_if || curr.tok == TOK::_elif) {
      IF_ELSE* fork;
      addStmt(fork = new IF_ELSE(nullptr, nullptr, nullptr));
      {
         setState(STATE::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         VEC<EXPR*> buff{};
         VEC<OPERATOR_AND_PRIORITY> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(TOK::open_br);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
            ++idx;

            switchStateTo(getCurrentState(), t, idx);
         }

         stack_of_one_expr_bfr_first_tok.pop_back();
         stack_of_one_expr_depths.pop_back();

         fork->cond = stack_of_expressions.back();
         stack_of_expressions.pop_back();
         swap(par_buff, stack_of_operators);
         swap(buff, stack_of_expressions);
         while (curr.tok != TOK::open_br) ++idx;
      }
      // if a==b { stmt stmt stmt }
      //       ^
      
      if (curr.tok == TOK::open_br) {
         // if(a b){ stmt stmt stmt }
         //       ^
         --idx;

         setState(STATE::prs_body);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_body) {
            ++idx;

            switchStateTo(getCurrentState(), t, idx);
         }

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         fork->success = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }

      if (t[idx + 2].tok == TOK::_else || t[idx + 2].tok == TOK::_elif) {

         STATE enter_state = (t[idx + 2].tok == TOK::_else) ? STATE::prs_body : STATE::prs_if;

         ++idx;
         popState();
         setState(enter_state);
         size_t sizeOfTheStack = s.size();

         VEC<STMT*> stmt_buff{};
         VEC<EXPR*> expr_buff{};

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);

         VEC<OPERATOR_AND_PRIORITY> op_buff;
         VEC<CALL_AND_DEPTH>        call_buff;
         VEC<STACK_OF_PAR_CONTEXT> par_buff;

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         while (s.size() > sizeOfTheStack || getCurrentState() == enter_state) {
            ++idx;

            switchStateTo(getCurrentState(), t, idx);
         }

         swap(op_buff, stack_of_operators);
         swap(call_buff, stack_of_calls);
         swap(par_buff, stack_of_parentheses);

         fork->fail = stack_of_statements.back();
         stack_of_statements.pop_back();

         swap(stmt_buff, stack_of_statements);
         swap(expr_buff, stack_of_expressions);
      }
      popState();
   }
}


void PARSER::parseIfel(const VEC<TOKEN>& t, size_t & idx)
{
   if (curr.tok == TOK::_if) {
      IFEL* fork;
      addExpr(fork = new IFEL(nullptr, nullptr, nullptr));
      {
         setState(STATE::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         VEC<EXPR*> buff{};
         VEC<OPERATOR_AND_PRIORITY> par_buff{};
         swap(buff, stack_of_expressions);
         swap(par_buff, stack_of_operators);

         if (next.tok != TOK::open_par) --idx;
         stack_of_one_expr_depths.push_back(depth);
         stack_of_one_expr_bfr_first_tok.push_back(TOK::dot);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
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
      while (next.tok != TOK::dot) {
         ++idx;
      }

      {
         setState(STATE::prs_one_expr);
         size_t sizeOfTheStack = s.size();
         VEC<EXPR*> buff{};
         VEC<OPERATOR_AND_PRIORITY> op_buff{};
         swap(buff, stack_of_expressions);
         swap(op_buff, stack_of_operators);

         stack_of_one_expr_depths.push_back(depth);

         while (s.size() > sizeOfTheStack || getCurrentState() == STATE::prs_one_expr) {
            ++idx;

            switchStateTo(getCurrentState(), t, idx);
         }

         stack_of_one_expr_depths.pop_back();

         if (stack_of_expressions.size() == 1) {
            COL* op = (COL*)stack_of_expressions[0];
            fork->l = op->l;
            fork->r = op->r;
            stack_of_expressions.pop_back();
            rmExprFromPool(op);
         } else {
            abort();
         }

         swap(op_buff, stack_of_operators);
         swap(buff, stack_of_expressions);

         freezeParDepthHandlerForOneStep();
         if (curr.tok == TOK::close_par) freezeClsPar = idx;
         --idx;
      }
      popState();
   }
}


void PARSER::unloadExprsToStmts()
{
   while (!stack_of_expressions.empty()) {
      stack_of_statements.push_back(stack_of_expressions.back());

      stack_of_expressions.pop_back();
   }
}


void PARSER::freezeParDepthHandlerForOneStep()
{
   ++freezeParDepthScore;
}


void PARSER::freezeEolExprStmtHandlerForOneStep()
{
   ++freezeEolExprStmtScore;
}
#undef curr
#undef next
#undef prew
