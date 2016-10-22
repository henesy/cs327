#include <stdio.h>
#include "stuff.h"

int main() {

	Thing * t;
	t = initThing();

	int r = -1;

	setX(t, 8);
	r = getX(t);

	printf("%d\n", r);

	return 0;
}

