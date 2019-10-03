#include "lexer_wrapper.h"

template<typename T>
using VEC = std::vector<T>;
namespace fs = std::experimental::filesystem;


void rmEolsAfterSomething(VEC<TOKEN>& ts, std::function<bool(TOK)> pred)
{
   size_t start = 0;
   bool after = false;

   for (size_t idx = 0; ts.size() > idx; ++idx) {
      if (pred(ts[idx].tok)) {
         start = idx + 1;
         after = ts[start].tok == TOK::eol;
         continue;
      }
      if (after && idx > start && ts[idx].tok != TOK::eol) {
         ts.erase(begin(ts) + start, begin(ts) + idx);
         idx = start - 1;
         after = false;
      }
   }
}


void rm_eols_after_key_words(VEC<TOKEN>& ts)
{
   rmEolsAfterSomething(ts,
      [](TOK tok) { return keyWords.count(tok); }
   );
}


void rm_eols_after_operators(VEC<TOKEN>& ts)
{
   rmEolsAfterSomething(ts,
      [](TOK tok) { return tok == TOK::assign || operators.count(tok); }
   );
}


void rm_eols_after_open_bracket(VEC<TOKEN>& ts)
{
   rmEolsAfterSomething(ts,
      [](TOK tok) {return tok == TOK::open_br || tok == TOK::open_par;}
   );
}


void put_eol_befire_close_bracket(VEC<TOKEN>& ts)
{
   for (size_t idx = 0; idx < ts.size() - 1; ++idx) {
      if (ts[idx + 1].tok == TOK::close_br) {
         ts.insert(begin(ts) + idx, TOK::eol);
         ++idx;
      }
   }
}


void lex_it(fs::path path)
{
   tokens.push_back(TOK::open_br);
   {
      std::ifstream fin(path);
      yyFlexLexer lexer(fin, std::cout);
      while (lexer.yylex());
      fin.close();
   }
   tokens.push_back(TOK::eol);
   tokens.push_back(TOK::close_br);
   tokens.push_back(TOK::eol);
   tokens.push_back(TOK::eof);

   rm_eols_after_key_words(tokens);
   rm_eols_after_operators(tokens);
   rm_eols_after_open_bracket(tokens);
   put_eol_befire_close_bracket(tokens);

   //std::cout << "tokens.size: " << tokens.size() << std::endl;
}
