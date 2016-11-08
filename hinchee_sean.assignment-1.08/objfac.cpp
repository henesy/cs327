#include "dungeon_generator.h"
#include <stdlib.h>

ObjFac::ObjFac(void) {
	id = NULL;
	dn = 0;
}

ObjFac::ObjFac(int n, ItemTemp * defs) {
	id = defs;
	dn = n;
}

Item ObjFac::GetObj() {
	int n = rand() % dn;
	ItemTemp it = id[n];
	Item ni;
	/* populate fields */
	Position p0;
	p0.x = 0;
	p0.y = 0;
	
	ni.n 	= it.n;
	ni.desc = it.desc;
	ni.dl	= it.dl;
	ni.t	= it.t;
	ni.e	= it.e;
	ni.c	= it.c;
	ni.d	= it.d;
	ni.hib	= it.hib->roll();
	ni.dob	= it.dob->roll();
	ni.deb	= it.deb->roll();
	ni.w	= it.w->roll();
	ni.spb	= it.spb->roll();
	ni.sa	= it.sa->roll();
	ni.v	= it.v->roll();
	ni.s	= it.s;
	ni.p 	= p0;
	
	return ni;
}
