#ifndef VIS_BASE_H
#define VIS_BASE_H
struct VIS_BASE {
   virtual void enter(struct VAR        *s) = 0;
   virtual void enter(struct NUM        *s) = 0;
   virtual void enter(struct SEQ        *s) = 0;
   virtual void enter(struct STR        *s) = 0;
   virtual void enter(struct ADD        *s) = 0;
   virtual void enter(struct DIF        *s) = 0;
   virtual void enter(struct MUL        *s) = 0;
   virtual void enter(struct SEP        *s) = 0;
   virtual void enter(struct DOT        *s) = 0;
   virtual void enter(struct COL        *s) = 0;
   virtual void enter(struct THEREFORE  *s) = 0;
   virtual void enter(struct OR         *s) = 0;
   virtual void enter(struct AND        *s) = 0;
   virtual void enter(struct MORE       *s) = 0;
   virtual void enter(struct LESS       *s) = 0;
   virtual void enter(struct MORE_EQ    *s) = 0;
   virtual void enter(struct LESS_EQ    *s) = 0;
   virtual void enter(struct EQUAL      *s) = 0;
   virtual void enter(struct NOT        *s) = 0;
   virtual void enter(struct UN_DIF     *s) = 0;
   virtual void enter(struct UN_ADD     *s) = 0;
   virtual void enter(struct CALL       *s) = 0;
   virtual void enter(struct IFEL       *s) = 0;
   virtual void enter(struct FOR_CH     *s) = 0;
   virtual void enter(struct FOR_EACH   *s) = 0;
   virtual void enter(struct IF_ELSE    *s) = 0;
   virtual void enter(struct WHILE_LOOP *s) = 0;
   virtual void enter(struct MAPA       *s) = 0;
   virtual void enter(struct ASSIGN     *s) = 0;
   virtual void enter(struct RET        *s) = 0;
   virtual void enter(struct BODY       *s) = 0;
   virtual void enter(struct BREAK      *s) = 0;
   virtual void enter(struct CONTINUE   *s) = 0;
   virtual void enter(struct ASSERT     *s) = 0;
   virtual void enter(struct LAM        *s) = 0;
   virtual void enter(struct FUNC       *s) = 0;
   virtual void enter(struct STRUCT     *s) = 0;
};
#endif // ifndef VIS_BASE_H