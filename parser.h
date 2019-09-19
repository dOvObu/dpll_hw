#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include "tokens.h"
#include "parser_nodes.h"
#include "vis_printer.h"

using namespace std;

template <typename T>
using vec = std::vector<T>;


struct parser {
   enum class state {
      prs_class,
      prs_func,

      prs_stmt,
      prs_if,
      prs_elif,
      prs_else,
      prs_ifel,
      prs_while,
      prs_for,
      prs_forch,
      prs_pholdr,
      prs_assert,
      prs_assign,
      prs_mapa,
      prs_body,
      prs_return,
      prs_break,
      prs_continue,

      prs_stmt_expr,
      prs_expr,
      prs_operator,
      prs_num,
      prs_id,
      prs_var,
      prs_str,
      prs_lam,
      prs_call,
      prs_expr_before_first_call,
      prs_one_expr,
      prs_one_un_op_expr,

      stop,
      unknown,
   };
   vec<state> s{ state::unknown };

   enum class action {
      add_class,
      add_func,
      add_stmt,
      add_expr,
      wait,
   };

   enum class assoc { left, right };

   struct operator_and_priority {
      struct {
         tok type{ tok::unknown };
         expr* expr{ nullptr };
      } op;

      int priority{ 0 };
   };

   struct call_and_depth {
      call* call{ nullptr };
      int depth{ 0 };
      vec<operator_and_priority> stack_of_operators;
   };

   struct stacks_of_par_context {
      vector<expr*>                 stack_of_expressions;
      vector<stmt*>                 stack_of_statements;
      vector<operator_and_priority> stack_of_operators;
      vector<call_and_depth>        stack_of_calls;

      void swapWith(parser* parser)
      {
         swap(parser->stack_of_expressions, stack_of_expressions);
         swap(parser->stack_of_statements, stack_of_statements);
         swap(parser->stack_of_operators, stack_of_operators);
         swap(parser->stack_of_calls, stack_of_calls);
      }
   };

   vector<action> log;
   int depth = 0;
   int stepDepth = 5;
   vector<tok>                   stack_of_one_expr_bfr_first_tok;
   vector<int>                   stack_of_one_expr_depths;
   vector<size_t>                stack_of_one_expr_num_of_exprs;
   vector<stmt*>                 stack_of_statements;
   vector<expr*>                 stack_of_expressions;
   vector<operator_and_priority> stack_of_operators;
   vector<call_and_depth>        stack_of_calls;
   vector<stacks_of_par_context> stack_of_parentheses;

   stmt* addStmt(stmt* s);
   expr* addExpr(expr* e);
   call* addCall(call* c);
   void addFunc(func* p);
   void addStruct(STRUCT* p);

   template <typename T> void addOperatorTmpl(tok type);
   template <typename T> void addUnOperatorTmpl(tok type, const vec<token>& t, size_t& idx);
   template <typename T> void evalLastOperatorToExprTmpl();
   template <typename T> void evalLastUnOperatorToExprTmpl();
   void addOperator(tok type);
   void evalLastOperatorToExpr();
   void evalLastUnOperatorToExpr();
   bool hasStateInHistory(state state);
   void goBackInHistoryTo(state state);
   int  getPriorityFor(tok type);
   assoc getAssociativityFor(tok type);
   size_t freezeParDepthScore{ 0 };
   size_t freezeClsPar{ 0 };
   size_t freezeEolExprStmtScore{ 0 };
   void freezeParDepthHandlerForOneStep();
   void freezeEolExprStmtHandlerForOneStep();
   bool conditionToStopParsingOneExpr(const vec<token>& t, size_t & idx);
   void unloadExprsToStmts();

   void  setState(state state);
   state getCurrentState();
   state popState();

   void parseStmt(const vec<token>& tokens, size_t& idx);
   void parseBreak(const vec<token>& tokens, size_t& idx);
   void parseReturn(const vec<token>& tokens, size_t& idx);
   void parseContinue(const vec<token>& tokens, size_t& idx);
   void parseBody(const vec<token>& tokens, size_t& idx);
   void parseAssign(const vec<token>& tokens, size_t& idx);
   void parseMapa(const vec<token>& tokens, size_t& idx);
   void parseStmtExpr(const vec<token>& tokens, size_t& idx);
   void parseExpr(const vec<token>& tokens, size_t& idx);
   void parseLambda(const vec<token>& tokens, size_t& idx);
   void parseOpnPar(const vec<token>& tokens, size_t& idx);
   void parseClsPar(const vec<token>& tokens, size_t& idx);
   void parseOperator(const vec<token>& tokens, size_t& idx);
   void parseNum(const vec<token>& tokens, size_t& idx);
   void parseId(const vec<token>& tokens, size_t& idx);
   void parseVar(const vec<token>& tokens, size_t& idx);
   void parseStr(const vec<token>& tokens, size_t& idx);
   void parseFunc(const vec<token>& tokens, size_t& idx);
   void parseCall(const vec<token>& tokens, size_t& idx);
   void parseWhile(const vec<token>& tokens, size_t& idx);
   void parseIf(const vec<token>& tokens, size_t& idx);
   void parseIfel(const vec<token>& tokens, size_t& idx);
   void parseForEach(const vec<token>& tokens, size_t& idx);
   void parseFor(const vec<token>& tokens, size_t& idx);
   void parseStruct(const vec<token>& tokens, size_t& idx);
   void parseOneExpr(const vec<token>& tokens, size_t& idx);
   void parsePlaceholder(size_t& idx);

   void parse(const vec<token>& tokens);
   void switchStateTo(state state, const vec<token>& tokens, size_t& idx);
};
// ------------------------------------------------------
template<typename T> void parser::addOperatorTmpl(tok type)
{
   int priority = getPriorityFor(type);
   T* o = new T(nullptr, nullptr);
   exprPool.push_back(expr_ptr(o));
   stack_of_operators.push_back({ { type, o }, priority });
}
// ------------------------------------------------------
template<typename T> void parser::addUnOperatorTmpl(tok type, const vec<token>& t, size_t& idx)
{
   int priority = getPriorityFor(t[idx].tok);
   T* o = new T(nullptr);
   exprPool.push_back(expr_ptr(o));
   stack_of_operators.push_back({ { type, o }, priority });
   --idx;
   stack_of_one_expr_num_of_exprs.push_back(stack_of_expressions.size());
   setState(state::prs_one_expr);
}
// ------------------------------------------------------
template<typename T> void parser::evalLastOperatorToExprTmpl()
{
   T* o = (T*)(stack_of_operators.back().op.expr);
   if (!stack_of_expressions.empty()) {
      o->r = stack_of_expressions.back();
      stack_of_expressions.pop_back();
   }
   if (!stack_of_expressions.empty()) {
      o->l = stack_of_expressions.back();
      stack_of_expressions.pop_back();
   }
   if (!stack_of_operators.empty()) {
      stack_of_operators.pop_back();
   }
   stack_of_expressions.push_back(o);
}
// ------------------------------------------------------
template<typename T> void parser::evalLastUnOperatorToExprTmpl()
{
   T* o = (T*)(stack_of_operators.back().op.expr);
   if (!stack_of_expressions.empty()) {
      o->r = stack_of_expressions.back();
      stack_of_expressions.pop_back();
   }
   if (!stack_of_operators.empty()) {
      stack_of_operators.pop_back();
   }
   stack_of_expressions.push_back(o);
}
#endif
