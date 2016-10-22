#include "stuff.h"

int getX(Thing * t) {
	return t->x;
}

void setX(Thing * t, int n) {
	t->x = n;
}


int getAX(Thing * t, int i) {
	return (t[i]).x;
}

void setAX(Thing * t, int i, int n) {
	(t[i]).x = n;
}


Thing * initThing() {
	return new Thing;
}

Thing * initThings(int n) {
	return new Thing[n];
}


