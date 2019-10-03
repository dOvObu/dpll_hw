#include "mau_tools.h"


using namespace std;
template<typename T>
using VEC = vector<T>;

std::map<string, void(*)(VIS_EXEC*, VEC<EXPR*>&)> call_backs;


void nat_print(VIS_EXEC* context, VEC<EXPR*>& args)
{
   cout << endl;
   VIS_PRINTER printer;
   for (auto& arg : args) arg->enter(&printer);
}


void nat_put(VIS_EXEC* context, VEC<EXPR*>& args)
{
   for (size_t idx = 0; idx < args.size(); ++idx) {
      auto type = context->types_stack.back();
      if (type == VIS_EXEC::TYPES::String) {
         cout << context->str_stack.back();
         context->str_stack.pop_back();
      } else if (type == VIS_EXEC::TYPES::Integer) {
         cout << context->num_stack.back();
         context->num_stack.pop_back();
      }
      context->types_stack.pop_back();
   }
   cout << endl;
}

void nat_tseitin(VIS_EXEC* context, VEC<EXPR*>& args)
{
   context->exec_tseitin(args);
}

void nat_dpll(VIS_EXEC* context, VEC<EXPR*>& args)
{
   VIS_DPLL expert;
   expert.exec(context->CNF);
}
//------------------------------------------------------------------------------
void run_script(std::experimental::filesystem::path path)
{
   exprPool.clear();
   stmtPool.clear();
   funcPool.clear();
   structPool.clear();
   tokens.clear();

   lex_it(path);

   PARSER parser;
   parser.s.push_back(PARSER::STATE::prs_body);
   parser.parse(tokens);

   {
      VIS_EXEC context;
      for (auto& it : call_backs) {
         context.add_native_func(it.first, it.second);
      }
      for (auto& f : funcPool) {
         context.functions[f->id] = f.get();
      }
      context.add_native_func("print", nat_print);
      context.add_native_func("put", nat_put);
      context.add_native_func("Tseitin", nat_tseitin);
      context.add_native_func("DPLL", nat_dpll);

      if (context.functions.count("main")) {
         context.functions["main"]->_body->enter(&context);
      } else {
         stmtPool.back()->enter(&context);
      }
   }
}
