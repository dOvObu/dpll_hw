#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include "tokens.h"
#include "parser_nodes.h"
#include "vis_printer.h"

//using namespace std;

template <typename T>
using VEC = std::vector<T>;


struct PARSER {
   enum class STATE {
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

   enum class ASSOC { left, right };

   struct OPERATOR_AND_PRIORITY {
      struct {
         TOK type{ TOK::unknown };
         EXPR* expr{ nullptr };
      } op;

      int priority{ 0 };
   };

   struct CALL_AND_DEPTH {
      CALL* call{ nullptr };
      int depth{ 0 };
      VEC<OPERATOR_AND_PRIORITY> stack_of_operators;
   };

   struct STACK_OF_PAR_CONTEXT {
      VEC<EXPR*>                 stack_of_expressions;
      VEC<STMT*>                 stack_of_statements;
      VEC<OPERATOR_AND_PRIORITY> stack_of_operators;
      VEC<CALL_AND_DEPTH>        stack_of_calls;

      void swapWith(PARSER* parser)
      {
         swap(parser->stack_of_expressions , stack_of_expressions);
         swap(parser->stack_of_statements  , stack_of_statements );
         swap(parser->stack_of_operators   , stack_of_operators  );
         swap(parser->stack_of_calls       , stack_of_calls      );
      }
   };

   VEC<STATE>                 s{ STATE::unknown };
   VEC<TOK>                   stack_of_one_expr_bfr_first_tok;
   VEC<int>                   stack_of_one_expr_depths;
   VEC<size_t>                stack_of_one_expr_num_of_exprs;
   VEC<STMT*>                 stack_of_statements;
   VEC<EXPR*>                 stack_of_expressions;
   VEC<OPERATOR_AND_PRIORITY> stack_of_operators;
   VEC<CALL_AND_DEPTH>        stack_of_calls;
   VEC<STACK_OF_PAR_CONTEXT> stack_of_parentheses;
   int depth{ 0 };
   const int stepDepth{ 5 };

   STMT* addStmt(STMT* s);
   EXPR* addExpr(EXPR* e);
   CALL* addCall(CALL* c);
   void addFunc(FUNC* p);
   void addStruct(STRUCT* p);

   template <typename T> void addOperatorTmpl(TOK type);
   template <typename T> void addUnOperatorTmpl(TOK type, const VEC<TOKEN>& t, size_t& idx);
   template <typename T> void evalLastOperatorToExprTmpl();
   template <typename T> void evalLastUnOperatorToExprTmpl();

   size_t freezeParDepthScore{ 0 };
   size_t freezeClsPar{ 0 };
   size_t freezeEolExprStmtScore{ 0 };

   void   addOperator(TOK type);
   void   evalLastOperatorToExpr();
   void   evalLastUnOperatorToExpr();
   bool   hasStateInHistory(STATE state);
   void   goBackInHistoryTo(STATE state);
   int    getPriorityFor(TOK type);
   ASSOC  getAssociativityFor(TOK type);
   void   freezeParDepthHandlerForOneStep();
   void   freezeEolExprStmtHandlerForOneStep();
   bool   conditionToStopParsingOneExpr(const VEC<TOKEN>& t, size_t & idx);
   void   unloadExprsToStmts();

   void  setState(STATE state);
   STATE getCurrentState();
   STATE popState();

   void parseStmt       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseBreak      (const VEC<TOKEN>& tokens, size_t& idx);
   void parseReturn     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseContinue   (const VEC<TOKEN>& tokens, size_t& idx);
   void parseBody       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseAssign     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseMapa       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseStmtExpr   (const VEC<TOKEN>& tokens, size_t& idx);
   void parseExpr       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseLambda     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseOpnPar     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseClsPar     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseOperator   (const VEC<TOKEN>& tokens, size_t& idx);
   void parseNum        (const VEC<TOKEN>& tokens, size_t& idx);
   void parseId         (const VEC<TOKEN>& tokens, size_t& idx);
   void parseVar        (const VEC<TOKEN>& tokens, size_t& idx);
   void parseStr        (const VEC<TOKEN>& tokens, size_t& idx);
   void parseFunc       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseCall       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseWhile      (const VEC<TOKEN>& tokens, size_t& idx);
   void parseIf         (const VEC<TOKEN>& tokens, size_t& idx);
   void parseIfel       (const VEC<TOKEN>& tokens, size_t& idx);
   void parseForEach    (const VEC<TOKEN>& tokens, size_t& idx);
   void parseFor        (const VEC<TOKEN>& tokens, size_t& idx);
   void parseStruct     (const VEC<TOKEN>& tokens, size_t& idx);
   void parseOneExpr    (const VEC<TOKEN>& tokens, size_t& idx);
   void parsePlaceholder(size_t& idx);

   void parse(const VEC<TOKEN>& tokens);
   void switchStateTo(STATE state, const VEC<TOKEN>& tokens, size_t& idx);
};
// ------------------------------------------------------
template<typename T> void PARSER::addOperatorTmpl(TOK type)
{
   int priority = getPriorityFor(type);
   T* o = new T(nullptr, nullptr);
   exprPool.push_back(EXPR_PTR(o));
   stack_of_operators.push_back({ { type, o }, priority });
}
// ------------------------------------------------------
template<typename T> void PARSER::addUnOperatorTmpl(TOK type, const VEC<TOKEN>& t, size_t& idx)
{
   int priority = getPriorityFor(t[idx].tok);
   T* o = new T(nullptr);
   exprPool.push_back(EXPR_PTR(o));
   stack_of_operators.push_back({ { type, o }, priority });
   --idx;
   stack_of_one_expr_num_of_exprs.push_back(stack_of_expressions.size());
   setState(STATE::prs_one_expr);
}
// ------------------------------------------------------
template<typename T> void PARSER::evalLastOperatorToExprTmpl()
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
template<typename T> void PARSER::evalLastUnOperatorToExprTmpl()
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
