#ifndef VIS_BASE_H
#define VIS_BASE_H
struct vis_base {
   virtual void enter(struct var        *s) = 0;
   virtual void enter(struct num        *s) = 0;
   virtual void enter(struct seq        *s) = 0;
   virtual void enter(struct str        *s) = 0;
   virtual void enter(struct add        *s) = 0;
   virtual void enter(struct dif        *s) = 0;
   virtual void enter(struct mul        *s) = 0;
   virtual void enter(struct sep        *s) = 0;
   virtual void enter(struct dot        *s) = 0;
   virtual void enter(struct col        *s) = 0;
   virtual void enter(struct therefore  *s) = 0;
   virtual void enter(struct _or        *s) = 0;
   virtual void enter(struct _and       *s) = 0;
   virtual void enter(struct more       *s) = 0;
   virtual void enter(struct _less      *s) = 0;
   virtual void enter(struct more_eq    *s) = 0;
   virtual void enter(struct less_eq    *s) = 0;
   virtual void enter(struct _equal     *s) = 0;
   virtual void enter(struct _not       *s) = 0;
   virtual void enter(struct un_dif     *s) = 0;
   virtual void enter(struct un_add     *s) = 0;
   virtual void enter(struct call       *s) = 0;
   virtual void enter(struct ifel       *s) = 0;
   virtual void enter(struct for_ch     *s) = 0;
   virtual void enter(struct for_each_   *s) = 0;
   virtual void enter(struct if_else    *s) = 0;
   virtual void enter(struct while_loop *s) = 0;
   virtual void enter(struct mapa       *s) = 0;
   virtual void enter(struct assign     *s) = 0;
   virtual void enter(struct ret        *s) = 0;
   virtual void enter(struct body       *s) = 0;
   virtual void enter(struct _break     *s) = 0;
   virtual void enter(struct _continue  *s) = 0;
   virtual void enter(struct assert     *s) = 0;
   virtual void enter(struct lam        *s) = 0;
   virtual void enter(struct func       *s) = 0;
   virtual void enter(struct STRUCT     *s) = 0;
};
#endif // ifndef VIS_BASE_H