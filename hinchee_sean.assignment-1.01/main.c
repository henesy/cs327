#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
	int		h; /* hardness */
	char	c; /* visual character */
} Tile;

typedef struct {
	int	x; /* x coordinate */
	int	y; /* y coordinate */
} Position;

typedef struct {
	Position	tl;	/* top left coordinate of the room, used as the core point of reference */
	Position	br; /* bottom right coordinate of the room as per above */
	int			w;	/* width */
	int			h;	/* height */
	int			id;	/* room ID, potentially useful for organization */
} Room;

typedef struct {
	Tile **	d;	/* dungeon buffer */
	Tile **	p;	/* print buffer */
	int		h;	/* height */
	int		w;	/* width */
	int		nr; /* number of rooms */
	int		mr;	/* max rooms */
	Room *	r;	/* rooms buffer */
} Dungeon;


/* prints the dungeon */
void print_dungeon(Dungeon * dungeon) {
	int i;
	int j;
	int h;
	
	for(h = 0; h < dungeon->nr; h++) {
		for(i = dungeon->r[h].tl.y; i < dungeon->r[h].tl.y + dungeon->r[h].h; i++) {
			for(j = dungeon->r[h].tl.x; j < dungeon->r[h].tl.x + dungeon->r[h].w; j++) {
				dungeon->p[i][j].c = '.';
			}
		}
	}

	for(i = 0; i < dungeon->h; i++) {
		int j;
		for(j = 0; j < dungeon->w; j++) {
			printf("%c", (dungeon->p[i][j]).c);
		}
		printf("\n");
	}
}

/* initializes the dungeon structure */
Dungeon init_dungeon(int h, int w, int mr) {
	Dungeon new_dungeon;
	new_dungeon.h	= h;
	new_dungeon.w	= w;
	new_dungeon.mr	= mr;

	/* dungeon buffer allocation+0'ing */
	new_dungeon.d = calloc(new_dungeon.h, sizeof(Tile *));
	
	int i;
	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.d[i] = calloc(new_dungeon.w, sizeof(Tile));
	}

	/* dungeon visual buffer allocation+0'ing */
	new_dungeon.p = calloc(new_dungeon.h, sizeof(Tile *));

	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.p[i] = calloc(new_dungeon.w, sizeof(Tile));
	}	

	/* rooms allocation+0'ing */
	new_dungeon.r = calloc(new_dungeon.mr, sizeof(Room));

	return new_dungeon;
}

/* 
(attempt to) place a room within a given dungeon 
__NOTE__: Should wipe the print buffer after room generation has finished
*/
int place_room(Dungeon * dungeon) {
	int x = (rand() % (dungeon->w-1)) +1;
	int y = (rand() % (dungeon->h-1)) +1;
	Room new_room;
	/* 
	set top right to rng number; might be worth making a more detailed placer with a lower 
		fail rate 
	*/
	new_room.tl.x = x; 
	new_room.tl.y = y;	
	/* for RNG, maybe do a rando room width/height and re-set .br */
	new_room.h = 3;
	new_room.w = 4;
	new_room.br.x = x + new_room.w-1;
	new_room.br.y = y + new_room.h-1;

	/* check for rooms loaded into the dungeon buffer already */
	int i;
	int j;
	int placed = -1;
	int passed = 0;
	for(i = y; i < dungeon->h-1 && i < y+3; i++) {
		for(j = x; j < dungeon->w-1 && j < x+4; j++) {
			if(dungeon->p[i][j].c != '.') {
				passed++;
			}
		}
	}

	/* return a failure if not all cells within the "Room" passed */
	if(passed < 12) {
		return placed; /* should be -1 */
	}
	
	/* return a failure if part of the room is out of bounds */
	if(new_room.br.x >= dungeon->w || new_room.br.y >= dungeon->h) {
		return placed;
	}


	/* check for surrounding rooms */	
	
	/* top row */
	for(i = new_room.tl.x-1; i < new_room.br.x+2 && new_room.tl.x-1 >= 0 && new_room.br.x+1 < dungeon->w && new_room.tl.y-1 >= 0; i++) {
		if((dungeon->p[new_room.tl.y-1][i]).c == '.') {
			return placed;
		}
	}

	/* bottom row */
	for(i = new_room.tl.x-1; i < new_room.br.x+2 && new_room.tl.x-1 >= 0 && new_room.br.x+1 < dungeon->w && new_room.br.y+1 < dungeon->h; i++) {
		if((dungeon->p[new_room.br.y+1][i]).c == '.') {
			return placed;
		}
	}

	/* left side */
	for(i = new_room.tl.y; i < new_room.br.y+1 && new_room.br.y+1 < dungeon->h && new_room.tl.x-1 >= 0; i++) {
		if((dungeon->p[i][new_room.tl.x-1]).c == '.') {
			return placed;
		}
	}

	/* right side */
	for(i = new_room.tl.y; i < new_room.br.y+1 && new_room.br.y+1 < dungeon->h && new_room.br.x+1 < dungeon->w; i++) {
		if((dungeon->p[i][new_room.br.x+1]).c == '.') {
			return placed;
		}
	}


	/* successful placement */
	placed = 0;

	/* fill the room into the dungeon buffer and add to room array */	
	for(i = y; i < y+3; i++) {
		for(j = x; j < x+4; j++) {
			dungeon->p[i][j].c = '.';
		}
	}



	new_room.id = dungeon->nr;

	if(dungeon->nr < dungeon->mr) {
		dungeon->nr++;
		dungeon->r[dungeon->nr-1] = new_room;
	} else {
		return -1;
	}


	return placed;
}

/* generate a blank dungeon */
void gen_dungeon(Dungeon * dungeon) {
	/*** top 3 (0, 1, 2) are reserved for the pseudo-HUD ***/	
	int i, j;

	/* set all slots to spaces originally */
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			(dungeon->d[i][j]).c = ' ';	/* all basic rooms are spaces */
			(dungeon->d[i][j]).h = 0; 	/** temporary hardness value to fill in until further notice **/
		}
	}

	/* immut-ify the outside rim */
	for(i = 0; i < dungeon->w; i++) {
		(dungeon->d[0][i]).h = 255;
	}
	for(i = 0; i < dungeon->w; i++) {
		(dungeon->d[dungeon->h-1][i]).h = 255;
	}
	for(i = 0; i < dungeon->h; i++) {
		(dungeon->d[i][0]).h = 255;
	}
	for(i = 0; i < dungeon->h; i++) {
		(dungeon->d[i][dungeon->w-1]).h = 255;
	}

	/* make p equal to d */
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			dungeon->p[i][j] = dungeon->d[i][j];
		}
	}
}


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* process commandline arguments */


	/* init the dungeon with default dungeon size and a max of 12 rooms */
	Dungeon dungeon = init_dungeon(21, 80, 12);
	gen_dungeon(&dungeon);
	
	/* testing until we integrate this into the gen_dungeon() fxn */
	srand(time(NULL));
	int i;
	int cnt = 0;
	int tst = 0;
	for(i = 0; dungeon.nr < dungeon.mr && cnt < 2000; i++) {
		tst = place_room(&dungeon);
		if(tst < 0) {
			cnt++;
		}
	}
	
	print_dungeon(&dungeon);


	/* free our arrays */
	free(dungeon.d);
	free(dungeon.r);
	return 0;
}

