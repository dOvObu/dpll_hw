#include "vis_exec.h"


void VIS_EXEC::add_native_func(string id)
{
   nat_call_flags[id] = false;
}


void VIS_EXEC::add_native_func(string id, void(*func)(VIS_EXEC *, VEC<EXPR*>&))
{
   nat_call_flags[id] = false;
   call_backs[id] = func;
}


void VIS_EXEC::exec_tseitin(VEC<EXPR*>& args)
{
   if (!args.empty()) {

      VIS_TSEITIN f;
      VIS_EXPR_TYPE expert;

      for (auto& it : args) {
         if (it != nullptr) {
            if (expert.get_type_of(it) == TOK::therefore) {
               f.change_therefore_to_or(it);
            }
            it->enter(&f);
         }
      }
      CNF = f.CNF;

      TOK type = expert.get_type_of(args.back());
      set<EXPR*> c;
      simplify_cnf();

      if (type == TOK::_not) {
         c.emplace(args.back());

      } else if (type == TOK::_and) {
         AND* node = (AND*)args.back();

         c.emplace(node->l);
         CNF.emplace(c);
         c.clear();
         c.emplace(node->r);

      } else if (type == TOK::_or) {
         OR* node = (OR*)args.back();

         c.emplace(node->l);
         c.emplace(node->r);
      }
      CNF.emplace(c);

      simplify_cnf(false);
      show_cnf();
   }
}


void VIS_EXEC::simplify_cnf(bool rm_garbage)
{
   VIS_EQUALITY expert;
   NOT n1(nullptr), n2(nullptr), *not_not = &n1;
   n1.r = &n2;
   set<set<EXPR*>> cnf;
   VEC<EXPR*> garbage;

   for (auto& it : CNF) {
      set<EXPR*> df;
      for (auto& jt : it) {
         EXPR* lit = jt;
         while (expert.is_in(not_not, lit)) {
            NOT* nt = (NOT*)lit;
            NOT* nnt = (NOT*)nt->r;
            lit = nnt->r;
            garbage.push_back(nt);
         }
         df.emplace(lit);
      }
      cnf.emplace(df);
   }

   CNF.clear();
   if (rm_garbage) {
      while (!garbage.empty()) {
         rmExprFromPool(garbage.back());
         garbage.pop_back();
      }
   }
   CNF = cnf;
}


void VIS_EXEC::check_calls_id(EXPR* id)
{
   is_call = true;
   id->enter(this);
   is_call = false;
}
// ==----- OVERRIDES -----==
void VIS_EXEC::enter(ASSIGN* p)
{
   assign_expr_to_id(p->e, p->id, num_variables, str_variables, arch_variables);
}


void VIS_EXEC::enter(BODY *p)
{
   for (auto& stmt : p->stmts) {
      if (return_found) {
         break;
      }
      stmt->enter(this);
   }
}


void VIS_EXEC::enter(RET * p)
{
   VIS_WALKER::enter(p);
   return_type = types_stack.back();
   return_found = true;
}


void VIS_EXEC::enter(THEREFORE* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   TYPES left_type = types_stack.back();
   types_stack.pop_back();
   if (p->r != nullptr) {
      p->r->enter(this);
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (left_type == right_type) {
      if (right_type == TYPES::Integer) {
         int num_buff = num_stack.back();
         num_stack.pop_back();
         num_stack.back() = !num_stack.back() || num_buff;
         types_stack.push_back(right_type);
      }
   }
}


void VIS_EXEC::enter(MUL * p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   TYPES left_type = types_stack.back();
   types_stack.pop_back();
   if (p->r != nullptr) {
      p->r->enter(this);
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (left_type == right_type) {
      if (right_type == TYPES::Integer) {
         int num_buff = num_stack.back();
         num_stack.pop_back();
         num_stack.back() *= num_buff;
         types_stack.push_back(right_type);
      }
   }
}


void VIS_EXEC::enter(ADD* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   TYPES left_type = types_stack.back();
   types_stack.pop_back();
   if (p->r != nullptr) {
      p->r->enter(this);
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (left_type == right_type) {
      if (right_type == TYPES::Integer) {
         int num_buff = num_stack.back();
         num_stack.pop_back();
         num_stack.back() += num_buff;
         types_stack.push_back(right_type);
      }
      else if (right_type == TYPES::String) {
         string str_buff = str_stack.back();
         str_stack.pop_back();
         str_stack.back().append(str_buff);
         types_stack.push_back(right_type);
      }
   }
}


void VIS_EXEC::enter(OR* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   TYPES left_type = types_stack.back();
   types_stack.pop_back();
   if (p->r != nullptr) {
      p->r->enter(this);
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (left_type == right_type) {
      if (right_type == TYPES::Integer) {
         int num_buff = num_stack.back();
         num_stack.pop_back();
         num_stack.back() = num_stack.back() || num_buff;
         types_stack.push_back(right_type);
      }
   }
}


void VIS_EXEC::enter(AND* p)
{
   if (p->l != nullptr) {
      p->l->enter(this);
   }
   TYPES left_type = types_stack.back();
   types_stack.pop_back();
   if (p->r != nullptr) {
      p->r->enter(this);
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (left_type == right_type) {
      if (right_type == TYPES::Integer) {
         int num_buff = num_stack.back();
         num_stack.pop_back();
         num_stack.back() = num_stack.back() && num_buff;
         types_stack.push_back(right_type);
      }
   }
}


void VIS_EXEC::enter(NOT* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);
      res = !res;
   }
   TYPES right_type = types_stack.back();
   types_stack.pop_back();

   if (right_type == TYPES::Integer) {
      num_stack.back() = !num_stack.back();
      types_stack.push_back(right_type);
   }
}


void VIS_EXEC::enter(VAR* p)
{
   if (is_call) {
      for (auto& flag : nat_call_flags) {
         nat_call_flags[flag.first] = p->id == flag.first;
      }
      if (arch_variables.count(p->id)) {
         arch_stack.push_back(arch_variables[p->id]);
         types_stack.push_back(TYPES::Arch);
      } else if (functions.count(p->id)) {
         func_stack.push_back(functions[p->id]);
         types_stack.push_back(TYPES::Func);
      }
   }
   else {
      if (M.count(p->id)) {
         res = M[p->id];
      }
      if (num_variables.count(p->id)) {
         num_stack.push_back(num_variables[p->id]);
         types_stack.push_back(TYPES::Integer);
      } else if (str_variables.count(p->id)) {
         str_stack.push_back(str_variables[p->id]);
         types_stack.push_back(TYPES::String);
      } else if (arch_variables.count(p->id)) {
         arch_stack.push_back(arch_variables[p->id]);
         types_stack.push_back(TYPES::Arch);
      } else if (functions.count(p->id)) {
         func_stack.push_back(functions[p->id]);
         types_stack.push_back(TYPES::Func);
      }
   }
}


void VIS_EXEC::enter(CALL* p)
{
   //VIS_WALKER::enter(p);

   if (p->id != nullptr) {
      check_calls_id(p->id);
   }

   if (!try_to_call_nat_func(p)) {
      VEC<EXPR*>* args{ nullptr };
      STMT* body{ nullptr };
      if (types_stack.back() == TYPES::Arch) {
         LAM* arch = arch_stack.back();
         args = &(arch->argsId);
         body = arch->s;
      } else if (types_stack.back() == TYPES::Func) {
         FUNC* foo = func_stack.back();
         args = &(foo->argsId);
         body = foo->_body;
      }
      types_stack.pop_back();
      VIS_EXEC new_context;
      new_context.functions = functions;
      new_context.nat_call_flags = nat_call_flags;
      new_context.call_backs = call_backs;
      new_context.num_variables = num_variables;
      new_context.func_stack = func_stack;
      new_context.types_stack = types_stack;
      new_context.num_stack.clear();
      // Execute arguments and put in arg_vars
      {
         size_t idx = 0;
         map<string, int>    new_num_vars;
         map<string, string> new_str_vars;
         map<string, LAM*>   new_arch_vars;
         if (args != nullptr) {
            for (auto& arg : *args) {
               new_context.assign_expr_to_id(p->args[idx], ((VAR*)arg)->id, new_num_vars, new_str_vars, new_arch_vars);
               ++idx;
            }
         }
         swap(new_context.num_variables, new_num_vars);
         swap(new_context.str_variables, new_str_vars);
      }
      // Execute body
      if (body != nullptr) body->enter(&new_context);
      // Take from return and put on stack
      if (new_context.return_found) {
         auto ret_type = new_context.return_type;
         types_stack.push_back(ret_type);
         if (ret_type == TYPES::Integer) {
            num_stack.push_back(new_context.num_stack.back());
         } else if (ret_type == TYPES::String) {
            str_stack.push_back(new_context.str_stack.back());
         } else if (ret_type == TYPES::Arch) {
            arch_stack.push_back(new_context.arch_stack.back());
         }
      }
   }
}


void VIS_EXEC::enter(LAM * p)
{
   types_stack.push_back(TYPES::Arch);
   arch_stack.push_back(p);
}


void VIS_EXEC::enter(STR * p)
{
   str_stack.push_back(p->s);
   types_stack.push_back(TYPES::String);
}


void VIS_EXEC::enter(NUM* p)
{
   num_stack.push_back(p->n);
   types_stack.push_back(TYPES::Integer);
}


bool VIS_EXEC::try_to_call_nat_func(CALL* p)
{
   bool res = false;

   for (auto& flag : nat_call_flags) {
      if (nat_call_flags[flag.first]) {
         nat_call_flags[flag.first] = false;

         if (call_backs.count(flag.first)) {

            for (auto& arg : p->args) {
               arg->enter(this);
            }
            call_backs[flag.first](this, p->args);
            res = true;
            break;
         }
      }
   }

   return res;
}


void VIS_EXEC::assign_expr_to_id(EXPR * p, string & id, map<string, int>& num_variables, map<string, string>& str_variables, map<string, LAM*>& arck_variables)
{
   p->enter(this);

   type_of_variable[id] = types_stack.back();
   if (types_stack.back() == TYPES::Integer) {
      num_variables[id] = num_stack.back();
      num_stack.pop_back();
   } else if (types_stack.back() == TYPES::String) {
      str_variables[id] = str_stack.back();
      str_stack.pop_back();
   } else if (types_stack.back() == TYPES::Arch) {
      arch_variables[id] = arch_stack.back();
      arch_stack.pop_back();
   }
   types_stack.pop_back();
}


void VIS_EXEC::show_cnf()
{
   *out << "\n\n ==---- CNF ----== \n";
   bool first1 = true, first2 = true;
   VIS_PRINTER writer;

   for (auto& it : CNF) {

      if (!first1) {
         *out << " & ";
      } else {
         first1 = false;
      }

      *out << '(';

      if (!it.empty()) {
         for (auto& jt : it) {

            if (!first2) {
               *out << " | ";
            } else {
               first2 = false;
            }

            if (jt != nullptr) {
               jt->enter(&writer);
            } else {
               *out << "nil";
            }
         }
      }
      first2 = true;
      *out << ')';
   }
   *out << "\n ==------ ------== \n";
}
