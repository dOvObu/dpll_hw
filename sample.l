%{
#include <string>
#include "tokens.h"
using namespace std;
%}
%option noyywrap
%%
[ \t] ;
[\n]  {
         if (!tokens.empty() && tokens.back().tok != tok::eol) {
            tokens.push_back(tok::eol);
         }
      }
[0-9]+\.[0-9]+ {cout << "float\n";}
[0-9]+     {tokens.push_back({tok::num_lit, stoi(yytext)});}
\"([^\"\\]|\\.|\\\n)*\"|\'([^\'\\]|\\.|\\\n)*\' {std::string s = yytext; tokens.push_back({tok::str_lit, s.substr(1,s.size()-2)});}
\+         {tokens.push_back(tok::pluss);}
\-         {tokens.push_back(tok::minus);}
\*         {tokens.push_back(tok::multiply);}
\/         {tokens.push_back(tok::divide);}
\.         {tokens.push_back(tok::dot);}
\=\>       {tokens.push_back(tok::therefore);}
\!         {tokens.push_back(tok::_not);}
\>         {tokens.push_back(tok::more);}
\<         {tokens.push_back(tok::less);}
\>\=       {tokens.push_back(tok::more_eq);}
\<\=       {tokens.push_back(tok::less_eq);}
\=         {tokens.push_back(tok::equal);}
\(         {tokens.push_back(tok::open_par);}
\)         {tokens.push_back(tok::close_par);}
\{         {tokens.push_back(tok::open_br);}
\}         {if (tokens.back().tok != tok::eol) tokens.push_back(tok::eol); tokens.push_back(tok::close_br);}
struct     {tokens.push_back(tok::_class);}
if         {tokens.push_back(tok::_if);}
else       {if (!tokens.empty() && tokens.back().tok != tok::eol) tokens.push_back(tok::eol); tokens.push_back(tok::_else);}
elif       {if (!tokens.empty() && tokens.back().tok != tok::eol) tokens.push_back(tok::eol); tokens.push_back(tok::_elif);}
while      {tokens.push_back(tok::_while);}
for        {tokens.push_back(tok::_for);}
ret        {tokens.push_back(tok::_return);}
assert     {tokens.push_back(tok::_assert);}
break      {tokens.push_back(tok::_break);}
continue   {tokens.push_back(tok::_continue);}
func       {tokens.push_back(tok::_func);}
\\         {tokens.push_back(tok::lam);}
\|\|       {tokens.push_back(tok::_or);}
or         {tokens.push_back(tok::_or);}
\&\&       {tokens.push_back(tok::_and);}
and        {tokens.push_back(tok::_and);}
\|         {tokens.push_back(tok::_where);}
\:\=       {tokens.push_back(tok::assign);}
\:         {tokens.push_back(tok::colon);}
\,         {tokens.push_back(tok::comma);}
[a-zA-Z_]+[a-zA-Z0-9_]* {tokens.push_back({tok::id, yytext});}
. ;
%%
