#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <set>

#include <FlexLexer.h>

#include "vis_base.h"
#include "vis_exec.h"
#include "parser_nodes.h"
#include "vis_printer.h"
#include "tokens.h"
#include "parser.h"
// ------------------------------------------------------
using namespace std;
template<typename T>
using vec = vector<T>;
// ------------------------------------------------------
void rmEolsAfterKeyWords(vec<token>& tokens);
void rmEolsAfterOperators(vec<token>& tokens);
void rmEolsAfterOpenBracket(vec<token>& tokens);
// ------------------------------------------------------
int main()
{
   tokens.push_back(tok::open_br);

   // --- lexer ---
   {
      ifstream fin("file.mau");
      yyFlexLexer lexer(fin, cout);
      while (lexer.yylex());
      fin.close();
   }
   tokens.push_back(tok::eol);
   tokens.push_back(tok::close_br);
   tokens.push_back(tok::eol);
   tokens.push_back(tok::eof);
   cout << "tokens.size: " << tokens.size() << endl;
   //*
      //{tok::dot},
   //*/

   rmEolsAfterKeyWords(tokens);
   rmEolsAfterOperators(tokens);
   rmEolsAfterOpenBracket(tokens);

   parser parser;
   parser.s.push_back(parser::state::prs_body);
   parser.parse(tokens);

   {
      exec exec;
      stmtPool[0]->enter(&exec);
   }

   printer writer;
   if (structPool.empty()) {
      if (funcPool.empty()) {
         if (stmtPool.empty()) {
            if (!exprPool.empty()) {
               showExprPool();
            }
         } else { showStmtPool(); }
      } else { for(auto& it : funcPool) it->enter(&writer); }
   } else { structPool.back()->enter(&writer); }
   
   cout
      << "\n depth:\t" << parser.depth
      << "\n struct:" << structPool.size()
      << "\n func:\t" << funcPool.size()
      << "\n stmt:\t" << stmtPool.size()
      << "\n expr:\t" << exprPool.size()
      << endl;


   //ofstream fout("kek.txt");
   //writer.out = &fout;
   //fout.close();
   //system("\"C:/Program Files/Notepad++/notepad++.exe\" kek.txt");
   //std::cout << "\ntest\n";

   cin.get();
}
// ------------------------------------------------------



// -------------------------------------
void rmEolsAfterKeyWords(vec<token>& ts)
{
   size_t start  = 0;
   bool afterKeyWord = false;

   for (size_t idx = 0; ts.size() > idx; ++idx) {
      if (keyWords.count(ts[idx].tok)) {
         start = idx + 1;
         afterKeyWord = ts[start].tok == tok::eol;
         continue;
      }
      if (afterKeyWord && idx > start && ts[idx].tok != tok::eol) {
         ts.erase(begin(ts) + start, begin(ts) + idx);
         idx = start - 1;
         afterKeyWord = false;
      }
   }
}
void rmEolsAfterOperators(vec<token>& ts)
{
   size_t start = 0;
   bool afterKeyWord = false;

   for (size_t idx = 0; ts.size() > idx; ++idx) {
      if (ts[idx].tok == tok::assign || operators.count(ts[idx].tok)) {
         start = idx + 1;
         afterKeyWord = ts[start].tok == tok::eol;
         continue;
      }
      if (afterKeyWord && idx > start && ts[idx].tok != tok::eol) {
         ts.erase(begin(ts) + start, begin(ts) + idx);
         idx = start - 1;
         afterKeyWord = false;
      }
   }
}
void rmEolsAfterOpenBracket(vec<token>& ts)
{
   size_t start = 0;
   bool afterKeyWord = false;

   for (size_t idx = 0; ts.size() > idx; ++idx) {
      if (ts[idx].tok == tok::open_br || ts[idx].tok == tok::open_par) {
         start = idx + 1;
         afterKeyWord = ts[start].tok == tok::eol;
         continue;
      }
      if (afterKeyWord && idx > start && ts[idx].tok != tok::eol) {
         ts.erase(begin(ts) + start, begin(ts) + idx);
         idx = start - 1;
         afterKeyWord = false;
      }
   }
}
