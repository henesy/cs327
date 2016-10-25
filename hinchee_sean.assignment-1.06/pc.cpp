#include "dungeon_generator.h"
#include <cstdlib>

/* updates a PC's memory after the print buffer has been properly populated */
void updateMemory(PC * pc, Dungeon * dungeon) {
	if(pc->mem == NULL) {
		pc->mem = (Memory **) malloc(dungeon->h * dungeon->w * sizeof(Memory));
	}

	int i;
	int j;
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			if(abs(i - pc->p.y) == 3 && abs(j - pc->p.x) == 3 && dungeon->d[i][j].h == 0) {
				//update from print buffer, this should be called before printing time
				pc->mem[i][j].c = dungeon->p[i][j].c;
				pc->mem[i][j].v = true;
			} else {
				if(pc->mem[i][j].v == false)
					pc->mem[i][j].c = ' ';
			}
		}
	}
}

PC * initPC(Dungeon * dungeon) {
	PC * p= new PC;

	p->c = '@';
	p->sn = 0;
	p->a = TRUE;

	p->s.s = 10;
	p->s.tu = FALSE;
	p->s.eb = FALSE;
	p->s.te = FALSE;
	p->s.in = FALSE;

	int r_id = rand() % dungeon->nr;
	int x = (rand() % dungeon->r[r_id].w) + getPosX(dungeon->r[r_id].tl);
	int y = (rand() % dungeon->r[r_id].h) + getPosY(dungeon->r[r_id].tl);

	p->p.x = p->to.x = x;
	p->p.y = p->to.y = y;

	p->t = 0;

	return p;
}
