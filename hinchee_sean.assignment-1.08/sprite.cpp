#include "dungeon_generator.h"

/***
setter functions
***/

Sprite * initSprite() {
    return new Sprite;
}

Sprite * initSprites(int n) {
    return new Sprite[n];
}

/***
copy function, deep copy
***/
void copySprite(Sprite * to, Sprite * from) {
    to->p.x  =  from->p.x ;
    to->p.y  = from->p.y ;
    to->c    = from->c   ;
    to->s.in = from->s.in;
    to->s.te = from->s.te;
    to->s.tu = from->s.tu;
    to->s.eb = from->s.eb;
    to->s.s  = from->s.s ;
    to->t    = from->t   ;
    to->to.x = from->to.x;
    to->to.y = from->to.y;
    to->sn   = from->sn  ;
    to->pc.x = from->pc.x;
    to->pc.y = from->pc.y;
    to->a    = from->a   ;
}

void copyASprite(Sprite * to, int n, Sprite * from) {
    (to[n]).p.x  =  from->p.x ;
    (to[n]).p.y  = from->p.y ;
    (to[n]).c    = from->c   ;
    (to[n]).s.in = from->s.in;
    (to[n]).s.te = from->s.te;
    (to[n]).s.tu = from->s.tu;
    (to[n]).s.eb = from->s.eb;
    (to[n]).s.s  = from->s.s ;
    (to[n]).t    = from->t   ;
    (to[n]).to.x = from->to.x;
    (to[n]).to.y = from->to.y;
    (to[n]).sn   = from->sn  ;
    (to[n]).pc.x = from->pc.x;
    (to[n]).pc.y = from->pc.y;
    (to[n]).a    = from->a   ;
    (to[n]).color= from->color;
}

Sprite * thisASprite(Sprite * arr, int i) {
    return arr[i].thisSprite();
}

Sprite * Sprite::thisSprite() {
    return this;
}

/***
getter functions
***/

/* singleton */
int getSpritePX(Sprite * s) {
    return s->p.x;
}

int getSpritePY(Sprite * s) {
    return s->p.y;
}

char getSpriteC(Sprite * s) {
	return s->c;
}

bool getSpriteSIn(Sprite * s) {
    return s->s.in;
}

bool getSpriteSTe(Sprite * s) {
    return s->s.te;
}

bool getSpriteSTu(Sprite * s) {
    return s->s.tu;
}

bool getSpriteSEb(Sprite * s) {
    return s->s.eb;
}

int getSpriteSS(Sprite * s) {
    return s->s.s;
}

int getSpriteT(Sprite * s) {
    return s->t;
}

int getSpriteToX(Sprite * s) {
    return s->to.x;
}

int getSpriteToY(Sprite * s) {
    return s->to.y;
}

int getSpriteSn(Sprite * s) {
    return s->sn;
}

int getSpritePcX(Sprite * s) {
    return s->pc.x;
}

int getSpritePcY(Sprite * s) {
    return s->pc.y;
}

bool getSpriteA(Sprite * s) {
    return s->a;
}

/* array-based */
int getSpriteAPX(Sprite * s, int i) {
    return s[i].p.x;
}

int getSpriteAPY(Sprite * s, int i) {
    return s[i].p.y;
}

char getSpriteAC(Sprite * s, int i) {
    return s[i].c;
}

bool getSpriteASIn(Sprite * s, int i) {
    return s[i].c;
}

bool getSpriteASTe(Sprite * s, int i) {
    return s[i].s.te;
}

bool getSpriteASTu(Sprite * s, int i) {
    return s[i].s.tu;
}

bool getSpriteASEb(Sprite * s, int i) {
    return s[i].s.eb;
}

int getSpriteASS(Sprite * s, int i) {
    return s[i].s.s;
}

int getSpriteAT(Sprite * s, int i) {
    return s[i].t;
}

int getSpriteAToX(Sprite * s, int i) {
    return s[i].to.x;
}

int getSpriteAToY(Sprite * s, int i) {
    return s[i].to.y;
}

int getSpriteASn(Sprite * s, int i) {
    return s[i].sn;
}

int getSpriteAPcX(Sprite * s, int i) {
    return s[i].pc.x;
}

int getSpriteAPcY(Sprite * s, int i) {
    return s[i].pc.y;
}

bool getSpriteAA(Sprite * s, int i) {
    return s[i].a;
}

/***
Setter functions
kill me
***/

/* singleton */
void setSpritePX(Sprite * s, int n) {
    s->p.x = n;
}

void setSpritePY(Sprite * s, int n) {
    s->p.y = n;
}

void setSpriteC(Sprite * s, char c) {
	s->c = c;
}

void setSpriteSIn(Sprite * s, bool b) {
    s->s.in = b;
}

void setSpriteSTe(Sprite * s, bool b) {
    s->s.te = b;
}

void setSpriteSTu(Sprite * s, bool b) {
    s->s.tu = b;
}

void setSpriteSEb(Sprite * s, bool b) {
    s->s.eb = b;
}

void setSpriteSS(Sprite * s, int n) {
    s->s.s = n;
}

void setSpriteT(Sprite * s, int n) {
    s->t = n;
}

void setSpriteToX(Sprite * s, int n) {
    s->to.x = n;
}

void setSpriteToY(Sprite * s, int n) {
    s->to.y = n;
}

void setSpriteSn(Sprite * s, int n) {
    s->sn = n;
}

void setSpritePcX(Sprite * s, int n) {
    s->pc.x = n;
}

void setSpritePcY(Sprite * s, int n) {
    s->pc.y = n;
}

void setSpriteA(Sprite * s, bool b) {
    s->a = b;
}

/* array-based */
void setSpriteAPX(Sprite * s, int i, int n) {
    s[i].p.x = n;
}

void setSpriteAPY(Sprite * s, int i, int n) {
    s[i].p.y = n;
}

void setSpriteAC(Sprite * s, int i, char c) {
    s[i].c = c;
}

void setSpriteASIn(Sprite * s, int i, bool b) {
    s[i].s.in = b;
}

void setSpriteASTe(Sprite * s, int i, bool b) {
    s[i].s.te = b;
}

void setSpriteASTu(Sprite * s, int i, bool b) {
    s[i].s.tu = b;
}

void setSpriteASEb(Sprite * s, int i, bool b) {
    s[i].s.eb = b;
}

void setSpriteASS(Sprite * s, int i, int n) {
    s[i].s.s = n;
}

void setSpriteAT(Sprite * s, int i, int n) {
    s[i].t = n;
}

void setSpriteAToX(Sprite * s, int i, int n) {
    s[i].to.x = n;
}

void setSpriteAToY(Sprite * s, int i, int n) {
    s[i].to.y = n;
}

void setSpriteASn(Sprite * s, int i, int n) {
    s[i].sn = n;
}

void setSpriteAPcX(Sprite * s, int i, int n) {
    s[i].pc.x = n;
}

void setSpriteAPcY(Sprite * s, int i, int n) {
    s[i].pc.y = n;
}

void setSpriteAA(Sprite * s, int i, bool b) {
    s[i].a = b;
}

/***
Position functions
### These exist because positions are used outside of classes for this project ###
can be safely moved out of sprite at a later time
***/

int getPosX(Position * p) {
    return p->x;
}

int getPosY(Position * p) {
    return p->y;
}

void setPosX(Position * p, int n) {
    p->x = n;
}

void setPosY(Position * p, int n) {
    p->y = n;
}

Position * initPos(void) {
	return new Position;
}
