#include <stdio.h>
#include <stdlib.h>

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
	int			w;	/* width */
	int			h;	/* height */
	int			id;	/* room ID, potentially useful for organization */
} Room;

typedef struct {
	Tile **	d;	/* dungeon buffer */
	int		h;	/* height */
	int		w;	/* width */
	int		mr;	/* max rooms */
	Room *	r;	/* rooms buffer */
} Dungeon;


/* prints the dungeon */
void print_dungeon(Dungeon * dungeon) {
	int i;
	for(i = 0; i < dungeon->h; i++) {
		int j;
		for(j = 0; j < dungeon->w; j++) {
			printf("%c", (dungeon->d[i][j]).c);
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

	/* dungeon visual buffer allocation+0'ing */
	new_dungeon.d = calloc(new_dungeon.h, sizeof(Tile *));
	
	int i;
	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.d[i] = calloc(new_dungeon.w, sizeof(Tile));
	}

	/* rooms allocation+0'ing */
	new_dungeon.r = calloc(new_dungeon.mr, sizeof(Room));

	return new_dungeon;
}

/* procedurally generate the dungeon */
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

	/* determine the core locations of the 6 base rooms */


}


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* process commandline arguments */


	/* init the dungeon with default dungeon size and a max of 12 rooms */
	Dungeon dungeon = init_dungeon(21, 80, 12);
	gen_dungeon(&dungeon);
	print_dungeon(&dungeon);

	/* free our arrays */
	free(dungeon.d);
	free(dungeon.r);
	return 0;
}

