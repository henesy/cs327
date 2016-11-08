#include "dungeon_generator.h"
#include <stdlib.h>

MonFac::MonFac(void) {
	md = NULL;
	dn = 0;
}

MonFac::MonFac(int n, SpriteTemp * defs) {
	md = defs;
	dn = n;
}

Sprite* MonFac::GetMon() {
	int n = rand() % dn;
	SpriteTemp s = md[n];
	Sprite * ns = new Sprite;
	/* populate fields */
	Position p0;
	p0.x = 0;
	p0.y = 0;
	Position pn1;
	pn1.x = -1;
	pn1.y = -1;
	
	ns->p = p0;
	ns->c = s.c;
	
	ns->s.in = s.s.in;
	ns->s.te = s.s.te;
	ns->s.tu = s.s.tu;
	ns->s.eb = s.s.eb;
	ns->s.pa = s.s.pa;
	ns->s.s = s.s.s->roll();
	ns->s.a = s.s.a;
	ns->s.hp = s.s.hp->roll();
	
	ns->t = 0;
	ns->to = p0;
	ns->sn = -1;
	ns->pc = pn1;
	ns->a = true;
	ns->n = s.n;
	ns->color = s.color;
	ns->desc = s.desc;
	ns->dl = s.dl;
	
	
	
	return ns;
}
