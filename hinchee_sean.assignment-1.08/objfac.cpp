#include "dungeon_generator.h"
#include <stdlib.h>

ObjFac::ObjFac(void) {
	id = NULL;
	dn = 0;
}

ObjFac::ObjFac(int n, Item * defs) {
	id = defs;
	dn = n;
}

Item ObjFac::GetObj() {
	int n = rand() % dn;
	Item it = id[n];
	return it;
}
