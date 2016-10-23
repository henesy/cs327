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
    return s[i].p.x;
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

}

void setSpritePY(Sprite * s, int n) {

}

void setSpriteC(Sprite * s, char c) {
	s->c = c;
}

void setSpriteSIn(Sprite * s, bool b) {

}

void setSpriteSTe(Sprite * s, bool b) {

}

void setSpriteSTu(Sprite * s, bool b) {

}

void setSpriteSEb(Sprite * s, bool b) {

}

void setSpriteSS(Sprite * s, int n) {

}

void setSpriteT(Sprite * s, int n) {

}

void setSpriteToX(Sprite * s, int n) {

}

void setSpriteToY(Sprite * s, int n) {

}

void setSpriteSn(Sprite * s, int n) {

}

void setSpritePcX(Sprite * s, int n) {

}

void setSpritePcY(Sprite * s, int n) {

}

void setSpriteA(Sprite * s, bool b) {

}

/* array-based */
void setSpriteAPX(Sprite * s, int i, int n) {

}

void setSpriteAPY(Sprite * s, int i, int n) {

}

void setSpriteAC(Sprite * s, int i, char c) {

}

void setSpriteASIn(Sprite * s, int i, bool b) {

}

void setSpriteASTe(Sprite * s, int i, bool b) {

}

void setSpriteASTu(Sprite * s, int i, bool b) {

}

void setSpriteASEb(Sprite * s, int i, bool b) {

}

void setSpriteASS(Sprite * s, int i, int n) {

}

void setSpriteAT(Sprite * s, int i, int n) {

}

void setSpriteAToX(Sprite * s, int i, int n) {

}

void setSpriteAToY(Sprite * s, int i, int n) {

}

void setSpriteASn(Sprite * s, int i, int n) {

}

void setSpriteAPcX(Sprite * s, int i, int n) {

}

void setSpriteAPcY(Sprite * s, int i, int n) {

}

void setSpriteAA(Sprite * s, int i, bool b) {

}
