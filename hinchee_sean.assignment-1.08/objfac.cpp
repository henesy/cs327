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
	return ni;
}
