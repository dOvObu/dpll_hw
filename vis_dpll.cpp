#include "vis_dpll.h"


void VIS_DPLL::exec(set<set<EXPR*>> _CNF)
{
   CNF = _CNF;
   map<string, int> M;
   clear_interpretation(M);

   set<set<EXPR*>> cnf;
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
void VIS_DPLL::enter(NOT* p)
{
   if (p->r != nullptr) {
      p->r->enter(this);
      res = !res;
   }
}


void VIS_DPLL::enter(VAR* p)
{
   res = _M[p->id];
}


void VIS_DPLL::enter(NUM* p)
{
   res = p->n;
}
// ==----- PRIVATE ------==
bool VIS_DPLL::_dpll(set<set<EXPR*>> S, map<string, int> M)
{
   string Id = select_lit(S, M);

   set<set<EXPR*>>  _S = S;
   map<string, int> _M = M;

   if (is_sat(S, M)) { return true; }
   if (is_unsat(S)) { return false; }

   set<set<EXPR*>> s;

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


set<set<EXPR*>> VIS_DPLL::unit_propagate(set<set<EXPR*>> S, map<string, int>& M)
{
   set<set<EXPR*>> s;

   for (auto& C : S) {

      bool emplace_c = true;

      if (C.size() == 1) {
         EXPR* l = *begin(C);
         string id = get_id_of_lit(l);

         if (M[id] == -1) {
            M[id] = is_positive(l);
         }
      }

      set<EXPR*> c;

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

set<set<EXPR*>> VIS_DPLL::eliminate_pure_literal(set<set<EXPR*>> s, map<string, int>& M)
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

   set<set<EXPR*>> S;

   for (auto& C : s) {
      set<EXPR*> c;
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


set<EXPR*> VIS_DPLL::rm_contrar_lit(set<EXPR*> C)
{
   map<string, pair<bool, bool>> M;

   for (auto& l : C) {
      string id = get_id_of_lit(l);
      if (M.count(id) == 0) M[id] = { false, false };
      bool& was = is_positive(l) ? M[id].was_positive : M[id].was_negative;
      was = true;
   }

   set<EXPR*> c;
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

string VIS_DPLL::select_lit(set<set<EXPR*>>& S, map<string, int>& M)
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


string VIS_DPLL::get_id_of_lit(EXPR* p)
{
   if (is_positive(p)) {
      return ((VAR*)p)->id;
   }
   else {
      return ((VAR*)((NOT*)p)->r)->id;
   }
}


int VIS_DPLL::get_res(EXPR* p, map<string, int>& M)
{
   _M = M;
   p->enter(this);

   return res;
}


bool VIS_DPLL::is_positive(EXPR *p)
{
   VIS_EXPR_TYPE expert;

   return expert.get_type_of(p) != TOK::_not;
}


void VIS_DPLL::clear_interpretation(map<string, int>& M)
{
   M.clear();
   for (auto& C : CNF) {
      for (auto& l : C) {
         VIS_EXPR_TYPE expert;

         if (expert.get_type_of(l) == TOK::_not) {
            M[((VAR*)((NOT*)l)->r)->id] = -1;
         }
         else {
            M[((VAR*)l)->id] = -1;
         }
      }
   }
}


bool VIS_DPLL::is_sat(set<set<EXPR*>>& S, map<string, int>& M)
{
   if (S.empty()) {
      if (!_SAT_) show_interpretation(M);

      _SAT_ = true;
   }
   return S.empty();
}


bool VIS_DPLL::is_unsat(set<set<EXPR*>>& S)
{
   for (auto& C : S) {
      if (C.empty()) {
         *out << "UNSAT" << endl;

         return true;
      }
   }
   return false;
}


void VIS_DPLL::show_interpretation(map<string, int>& M)
{
   cout << "==----- Interp: -----==\n";
   for (auto& m : M) {
      if (m.second != -1) {
         cout << " " << m.first << "\t: " << m.second << endl;
      }
   }
   cout << "==--------   --------==\n";
}


void VIS_DPLL::show_cnf(set<set<EXPR*>>& S)
{
   swap(S, CNF);
   show_cnf();
   swap(S, CNF);
}


void VIS_DPLL::show_cnf()
{
   *out << "\n\n ==---- CNF ----== \n";
   bool first1 = true, first2 = true;
   VIS_PRINTER writer;

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
