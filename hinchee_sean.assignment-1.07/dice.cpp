#include "dungeon_generator.h"
#include <stdlib.h>


int	Dice::roll(void) {
	return b + n * ((rand() % d) + 1);
}

Dice::Dice(int base, int num, int die) {
	/* format to resemble: rolling 2+1d6 (2 + 1*[1→6]) ∴ (b + n*[1→d]) */
	b = base;
	n = num;
	d = die;
}


