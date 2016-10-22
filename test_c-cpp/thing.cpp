#include "stuff.h"

int getX(Thing * t) {
	return t->x;
}

void setX(Thing * t, int n) {
	t->x = n;
}

Thing * initThing() {
	return new Thing;
}


