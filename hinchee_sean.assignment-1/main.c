#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int		h; /* hardness */
	char	c; /* visual character */
	int		i; /* immutable */
} Tile;


/* just handle screen and dungeon as separated (screen is top 3x80, dungeon is 21x80) */
Tile ** screen;
int screen_height;
int screen_width;

/* prints the dungeon */
void print_dungeon() {
	int i;
	for(i = 0; i < screen_height; i++) {
		int j;
		for(j = 0; j < screen_width; j++) {
			printf("%c", (screen[i][j]).c);
		}
		printf("\n");
	}
}

/* initializes the dungeon */
void init_dungeon(int h, int w) {
	screen_height	= h;
	screen_width	= w;
	
	screen = calloc(h, sizeof(Tile *));
	
	int i;
	for(i = 0; i < h; i++) {
		screen[i] = calloc(w, sizeof(Tile));
	}
}

/* procedurally generate the dungeon */
void gen_dungeon() {
	
}


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* process commandline arguments */


	/* init the screen with default screen size */
	init_dungeon(24, 80);
	gen_dungeon();
	print_dungeon();

	free(screen);
	return 0;
}

