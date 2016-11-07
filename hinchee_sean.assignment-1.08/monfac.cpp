#include "dungeon_generator.h"

MonFac::MonFac(void) {
	md = NULL;
	dn = 0;
}

MonFac::MonFac(int n, Sprite * defs) {
	md = defs;
	dn = n;
}
