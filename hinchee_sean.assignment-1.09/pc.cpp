#include "dungeon_generator.h"
#include <cstdlib>
#include <ncurses.h>

/* updates a PC's memory after the print buffer has been properly populated */
void updateMemory(Dungeon * dungeon) {

	int i;
	int j;
	for(i = 1; i < dungeon->h-1; i++) {
		for(j = 1; j < dungeon->w-1; j++) {
			if(abs(i - ((PC *) &dungeon->ss[0])->p.y) <= 3 && abs(j - ((PC *) &dungeon->ss[0])->p.x) <= 3 && dungeon->d[i][j].h == 0) {
				//update from print buffer, this should be called before printing time
				dungeon->plyr->mem[i][j].c = dungeon->p[i][j].c;
				dungeon->plyr->mem[i][j].v = true;
				dungeon->plyr->mem[i][j].color = dungeon->p[i][j].color;
			}
		}
	}
}

PC * thisAPC(Sprite * arr, int i) {
	return arr[i].thisPC();
}

PC * Sprite::thisPC() {
    return (PC *) this;
}


char getMem(Dungeon * dungeon, int y, int x) {
	return dungeon->plyr->mem[y][x].c;
}

PC * getPC(Sprite * arr) {
	return (PC *) &(arr[0]);
}


PC * initPC(Dungeon * dungeon) {
	PC * p= new PC;

	p->c = '@';
	p->sn = 0;
	p->a = TRUE;
	
	p->s.hp = 100;
	p->s.a = new Dice(0, 1, 4);

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

	dungeon->plyr = p;


	p->mem = (Memory **) malloc(dungeon->h * sizeof(Memory *));
	int i;
	for(i = 0; i < dungeon->h; i++) {
		p->mem[i] = (Memory *) malloc(dungeon->w * sizeof(Memory));
	}


	int j;
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			p->mem[i][j].c = ' ';
			p->mem[i][j].v = false;
		}
	}

	return p;
}
