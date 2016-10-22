#include <stdio.h>
#include "stuff.h"

int main() {

	/* array */
	Thing * things;
	things = initThings(3);

	setAX(things, 1, 9);
	
	int n = -1;

	n = getAX(things, 1);

	printf("%d\n", n);
	
	/* singleton */
	Thing * t;
	t = initThing();

	int r = -1;

	setX(t, 8);
	r = getX(t);

	printf("%d\n", r);

	return 0;
}

