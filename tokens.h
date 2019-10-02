#ifndef TOKENS_H
#define TOKENS_H
#include <set>
#include <vector>

enum class TOK {
   num_lit, str_lit,
   id,

   pluss, un_pluss, minus, un_minus, divide, multiply, therefore, more, less, more_eq, less_eq, equal, _not, _or, _and,
   _class, _func, lam, _if, _elif, _else, _while, _for, _where, _assert, _return, _break, _continue,

   dot, colon, assign, eol,
   open_par, close_par,
   open_br, close_br,

   space, comma, eof, unknown,
};
extern std::set<TOK> operators;
extern std::set<TOK> un_operators;
extern std::set<TOK> keyWords;
extern std::set<TOK> serviceSymbols;

struct TOKEN {
   TOKEN(TOK _tok) : tok(_tok) {}
   TOKEN(TOK _tok, int _int_val) : tok(_tok), int_val(_int_val) {}
   TOKEN(TOK _tok, std::string _str_val) : tok(_tok), str_val(_str_val) {}
   TOK tok{ TOK::unknown };
   int int_val{ 0 };
   std::string str_val{ "" };
};

extern std::vector<TOKEN> tokens;

#endif // ifndef TOKENS_H
