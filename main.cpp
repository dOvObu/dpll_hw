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
using VEC = vector<T>;

int main()
{
   lex_it("./file.mau");

   PARSER parser;
   parser.s.push_back(PARSER::STATE::prs_body);
   parser.parse(tokens);

   {
      VIS_EXEC exec;
      stmtPool[0]->enter(&exec);
   }

   show_contents_of_pool();
   std::cout << "\n depth:\t" << parser.depth;

   std::cin.get();
}
