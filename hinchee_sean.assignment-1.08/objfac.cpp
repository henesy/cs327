#include "dungeon_generator.h"

ObjFac::ObjFac(void) {
	id = NULL;
	dn = 0;
}

ObjFac::ObjFac(int n, Item * defs) {
	id = defs;
	dn = n;
}
