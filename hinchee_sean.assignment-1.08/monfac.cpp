#include "dungeon_generator.h"
#include <stdlib.h>

MonFac::MonFac(void) {
	md = NULL;
	dn = 0;
}

MonFac::MonFac(int n, Sprite * defs) {
	md = defs;
	dn = n;
}

Sprite MonFac::GetMon() {
	int n = rand() % dn;
	Sprite s = md[n];
	return s;
}
