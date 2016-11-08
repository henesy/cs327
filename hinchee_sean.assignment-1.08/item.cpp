#include "dungeon_generator.h"
#include <stdlib.h>

void placeitem(Dungeon * dungeon, Item it) {
	int r_id = rand() % dungeon->nr;
	it.p.x = (rand() % dungeon->r[r_id].w) + getPosX(dungeon->r[r_id].tl);
	it.p.y = (rand() % dungeon->r[r_id].h) + getPosY(dungeon->r[r_id].tl);

	if(dungeon->nit < dungeon->ms) {
		dungeon->items[dungeon->nit] = it;
		
		dungeon->nit++;
	}
}
