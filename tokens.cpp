#include "tokens.h"

std::set<TOK> operators{ TOK::pluss, TOK::minus, TOK::divide, TOK::multiply, TOK::dot, TOK::colon, TOK::more, TOK::more_eq, TOK::less, TOK::less_eq, TOK::equal, TOK::therefore, TOK::_and, TOK::_or, TOK::_not };
std::set<TOK> un_operators{ TOK::un_minus, TOK::un_pluss, TOK::_not };
std::set<TOK> keyWords{ TOK::_class, TOK::_func, TOK::_if, TOK::_elif, TOK::_else, TOK::_while, TOK::_assert, TOK::_return, TOK::_break, TOK::_continue };
std::set<TOK> serviceSymbols{ TOK::assign, TOK::open_par, TOK::close_par, TOK::open_br, TOK::comma, TOK::close_br };
std::vector<TOKEN> tokens;