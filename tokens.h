#ifndef TOKENS_H
#define TOKENS_H
#include <set>
#include <vector>

enum class tok {
   num_lit, str_lit,
   id,

   pluss, un_pluss, minus, un_minus, divide, multiply, therefore, more, less, more_eq, less_eq, equal, _not, _or, _and,
   _class, _func, lam, _if, _elif, _else, _while, _for, _where, _assert, _return, _break, _continue,

   dot, colon, assign, eol,
   open_par, close_par,
   open_br, close_br,

   space, comma, eof, unknown,
};
extern std::set<tok> operators;
extern std::set<tok> un_operators;
extern std::set<tok> keyWords;
extern std::set<tok> serviceSymbols;

struct token {
   token(tok _tok) : tok(_tok) {}
   token(tok _tok, int _int_val) : tok(_tok), int_val(_int_val) {}
   token(tok _tok, std::string _str_val) : tok(_tok), str_val(_str_val) {}
   tok tok{ tok::unknown };
   int int_val{ 0 };
   std::string str_val{ "" };
};

extern std::vector<token> tokens;

#endif // ifndef TOKENS_H
