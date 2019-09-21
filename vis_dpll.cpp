#include "vis_dpll.h"


void vis_dpll::exec(set<set<expr*>> _CNF)
{
   CNF = _CNF;
   map<string, int> M;
   clear_interpretation(M);

   set<set<expr*>> cnf;
   for (auto& C : CNF) {
      auto c = rm_contrar_lit(C);

      if (!c.empty()) {
         cnf.emplace(c);
      }
   }

   _dpll(cnf, M);

   if (_SAT_) cout << "SAT" << endl;

   CNF.clear();
}
// ==----- OVERRIDES -----==
void vis_dpll::enter(_not* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);
      res = !res;
   }
}


void vis_dpll::enter(var* p)
{
   res = _M[p->id];
}


void vis_dpll::enter(num* p)
{
   res = p->n;
}
// ==----- PRIVATE ------==
bool vis_dpll::_dpll(set<set<expr*>> S, map<string, int> M)
{
   string Id = select_lit(S, M);

   set<set<expr*>>  _S = S;
   map<string, int> _M = M;

   if (is_sat(S, M)) { return true; }
   if (is_unsat(S)) { return false; }

   set<set<expr*>> s;

   while (s != S) {
      s = unit_propagate(S, M);
      swap(s, S);
   }
   s.clear();

   while (s != S) {
      s = eliminate_pure_literal(S, M);
      swap(s, S);
   }

   if (is_sat(S, M)) { return true; }
   _dpll(S, M);


   if (!_SAT_ && !Id.empty()) {
      _M[Id] = !M[Id];
      _dpll(_S, _M);
   }
   return _SAT_;
}


set<set<expr*>> vis_dpll::unit_propagate(set<set<expr*>> S, map<string, int>& M)
{
   set<set<expr*>> s;

   for (auto& C : S) {

      bool emplace_c = true;

      if (C.size() == 1) {
         expr* l = *begin(C);
         string id = get_id_of_lit(l);

         if (M[id] == -1) {
            M[id] = is_positive(l);
         }
      }

      set<expr*> c;

      for (auto& l : C) {
         bool emplace_l = true;

         string id = get_id_of_lit(l);

         if (M[id] != -1) {
            if (get_res(l, M)) {
               emplace_c = false;

               break;
            }
            else {
               emplace_l = false;
            }
         }

         if (emplace_l) c.emplace(l);
      }

      if (emplace_c) s.emplace(c);
   }
   return s;
}

#define was_positive first
#define was_negative second

set<set<expr*>> vis_dpll::eliminate_pure_literal(set<set<expr*>> s, map<string, int>& M)
{
   map<string, pair<bool, bool>> p;

   for (auto& m : M) {
      p[m.first] = { false, false };
   }

   for (auto& C : s) {
      for (auto& l : C) {
         string id = get_id_of_lit(l);
         bool& was = is_positive(l) ? p[id].was_positive : p[id].was_negative;
         was = true;
      }
   }

   set<set<expr*>> S;

   for (auto& C : s) {
      set<expr*> c;
      for (auto& l : C) {
         bool emplace_l = false;

         string id = get_id_of_lit(l);
         emplace_l = p[id].was_positive && p[id].was_negative;

         if (emplace_l) {
            c.emplace(l);
         }
         else {
            if (M[id] == -1) {
               M[id] = 1;
            }
         }
      }
      S.emplace(c);
   }

   return S;
}


set<expr*> vis_dpll::rm_contrar_lit(set<expr*> C)
{
   map<string, pair<bool, bool>> M;

   for (auto& l : C) {
      string id = get_id_of_lit(l);
      if (M.count(id) == 0) M[id] = { false, false };
      bool& was = is_positive(l) ? M[id].was_positive : M[id].was_negative;
      was = true;
   }

   set<expr*> c;
   for (auto& l : C) {
      string id = get_id_of_lit(l);
      if (!M[id].was_positive || !M[id].was_negative) {
         c.emplace(l);
      }
   }

   return c;
}

#undef was_positive
#undef was_negative

string vis_dpll::select_lit(set<set<expr*>>& S, map<string, int>& M)
{
   for (auto& C : S) {

      for (auto& l : C) {

         string id = get_id_of_lit(l);

         if (M[id] == -1) {
            M[id] = is_positive(l) ? 1 : 0;
            return id;
         }
      }
   }
   return "";
}


string vis_dpll::get_id_of_lit(expr* p)
{
   if (is_positive(p)) {
      return ((var*)p)->id;
   }
   else {
      return ((var*)((_not*)p)->r)->id;
   }
}


int vis_dpll::get_res(expr* p, map<string, int>& M)
{
   _M = M;
   p->enter(this);

   return res;
}


bool vis_dpll::is_positive(expr *p)
{
   vis_expr_type expert;

   return expert.get_type_of(p) != tok::_not;
}


void vis_dpll::clear_interpretation(map<string, int>& M)
{
   M.clear();
   for (auto& C : CNF) {
      for (auto& l : C) {
         vis_expr_type expert;

         if (expert.get_type_of(l) == tok::_not) {
            M[((var*)((_not*)l)->r)->id] = -1;
         }
         else {
            M[((var*)l)->id] = -1;
         }
      }
   }
}


bool vis_dpll::is_sat(set<set<expr*>>& S, map<string, int>& M)
{
   if (S.empty()) {
      if (!_SAT_) show_interpretation(M);

      _SAT_ = true;
   }
   return S.empty();
}


bool vis_dpll::is_unsat(set<set<expr*>>& S)
{
   for (auto& C : S) {
      if (C.empty()) {
         *out << "UNSAT" << endl;

         return true;
      }
   }
   return false;
}


void vis_dpll::show_interpretation(map<string, int>& M)
{
   cout << "==----- Interp: -----==\n";
   for (auto& m : M) {
      if (m.second != -1) {
         cout << " " << m.first << "\t: " << m.second << endl;
      }
   }
   cout << "==--------   --------==\n";
}


void vis_dpll::show_cnf(set<set<expr*>>& S)
{
   swap(S, CNF);
   show_cnf();
   swap(S, CNF);
}


void vis_dpll::show_cnf()
{
   *out << "\n\n ==---- CNF ----== \n";
   bool first1 = true, first2 = true;
   printer writer;

   for (auto& it : CNF) {

      if (!first1) {
         *out << " & ";
      }
      else {
         first1 = false;
      }

      *out << '(';

      if (!it.empty()) {
         for (auto& jt : it) {

            if (!first2) {
               *out << " | ";
            }
            else {
               first2 = false;
            }

            if (jt != nullptr) {
               jt->enter(&writer);
            }
            else {
               *out << "nil";
            }
         }
      }
      first2 = true;
      *out << ')';
   }
   *out << "\n ==------ ------== \n";
}
