#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <set>

#include "vis_base.h"
#include "vis_exec.h"
#include "parser_nodes.h"
#include "vis_printer.h"
#include "tokens.h"
#include "parser.h"
#include "lexer_wrapper.h"

using namespace std;
template<typename T>
using vec = vector<T>;

int main()
{
   lex_it("./file.mau");

   parser parser;
   parser.s.push_back(parser::state::prs_body);
   parser.parse(tokens);

   {
      exec exec;
      stmtPool[0]->enter(&exec);
   }

   show_contents_of_pool();
   cout << "\n depth:\t" << parser.depth;

   cin.get();
}
