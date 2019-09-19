#include "tokens.h"

std::set<tok> operators{ tok::pluss, tok::minus, tok::divide, tok::multiply, tok::dot, tok::colon, tok::more, tok::more_eq, tok::less, tok::less_eq, tok::equal, tok::therefore, tok::_and, tok::_or, tok::_not };
std::set<tok> un_operators{ tok::un_minus, tok::un_pluss, tok::_not };
std::set<tok> keyWords{ tok::_class, tok::_func, tok::_if, tok::_elif, tok::_else, tok::_while, tok::_assert, tok::_return, tok::_break, tok::_continue };
std::set<tok> serviceSymbols{ tok::assign, tok::open_par, tok::close_par, tok::open_br, tok::comma, tok::close_br };
std::vector<token> tokens;