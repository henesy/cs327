#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int		h; /* hardness */
	char	c; /* visual character */
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


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* init the screen */
	screen_height	= 24;
	screen_width	= 80;
	screen = (Tile **) calloc(screen_height * screen_width, sizeof(Tile));

}

