#include "lexer_wrapper.h"

template<typename T>
using vec = std::vector<T>;
namespace fs = std::experimental::filesystem;


void rmEolsAfterSomething(vec<token>& ts, std::function<bool(tok)> pred)
{
   size_t start = 0;
   bool after = false;

   for (size_t idx = 0; ts.size() > idx; ++idx) {
      if (pred(ts[idx].tok)) {
         start = idx + 1;
         after = ts[start].tok == tok::eol;
         continue;
      }
      if (after && idx > start && ts[idx].tok != tok::eol) {
         ts.erase(begin(ts) + start, begin(ts) + idx);
         idx = start - 1;
         after = false;
      }
   }
}


void rm_eols_after_key_words(vec<token>& ts)
{
   rmEolsAfterSomething(ts,
      [](tok tok) { return keyWords.count(tok); }
   );
}


void rm_eols_after_operators(vec<token>& ts)
{
   rmEolsAfterSomething(ts,
      [](tok tok) { return tok == tok::assign || operators.count(tok); }
   );
}


void rm_eols_after_open_bracket(vec<token>& ts)
{
   rmEolsAfterSomething(ts,
      [](tok tok) {return tok == tok::open_br || tok == tok::open_par;}
   );
}


void lex_it(fs::path path)
{
   tokens.push_back(tok::open_br);
   {
      std::ifstream fin(path);
      yyFlexLexer lexer(fin, std::cout);
      while (lexer.yylex());
      fin.close();
   }
   tokens.push_back(tok::eol);
   tokens.push_back(tok::close_br);
   tokens.push_back(tok::eol);
   tokens.push_back(tok::eof);

   rm_eols_after_key_words(tokens);
   rm_eols_after_operators(tokens);
   rm_eols_after_open_bracket(tokens);

   std::cout << "tokens.size: " << tokens.size() << std::endl;
}
