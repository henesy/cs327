#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define	TRUE	1
#define	FALSE	0

typedef struct {
	int		h;	/* hardness */
	char	c;	/* visual character */
	int		p;	/* mark 1 if path, 0 if not a path (corridors) */
} Tile;

typedef struct {
	int	x; /* x coordinate */
	int	y; /* y coordinate */
} Position;

/* maybe make these pointers? */
typedef struct {
	int prev; /* previous room in the path (using Room.id) */
	int next; /* room the path leads to (using Room.id) */
} Path;

typedef struct {
	Position	tl;		/* top left coordinate of the room, used as the core point of reference */
	Position	br;		/* bottom right coordinate of the room as per above */
	int			w;		/* width */
	int			h;		/* height */
	int			id;		/* room ID, potentially useful for organization */
	int			p;		/* mark 1 if processed; 0 if not processed (corridors) */
	Position	ctr;	/* "center" point; very rough, might need improved */
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


/* writes the dungeon file */
void write_dungeon(Dungeon * dungeon) {
	/* should use mkdir (2) as stretch goal */
	FILE * file;
	int path_size = 50;
	char * path = calloc(path_size, sizeof(char));
	/* kind of excessive path size; not sure what a good number would be though */
	path = getenv("HOME");
	path = strcat(path, "/.rlg327/dungeon");

	/*
	this should check for safety of the statement; check if existing: remove, create, etc.
	this should be done by checking for null
	*/
	file = fopen(path, "wb+");

	/* write the file-type marker */
	fseek(file, 0, SEEK_SET);
	char marker[7];
	strcpy(marker, "RLG327");
	fwrite(marker, sizeof(char), 6, file);

	/* write the file version marker */
	fseek(file, 6, SEEK_SET);
	uint32_t file_version = 0;
	fwrite(&file_version, sizeof(uint32_t), 1, file);

	/* write the size of the file ;; unsure how to properly calculate */
	fseek(file, 10, SEEK_SET);
 	uint32_t size = 1693 + (4 * dungeon->nr);
	fwrite(&size, sizeof(uint32_t), 1, file);

	/* row-major dungeon matrix */
	fseek(file, 14, SEEK_SET);
	int pos = 14;
	int i;
	int j;

	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			fseek(file, pos, SEEK_SET);
			int8_t h;
			h = (int8_t)(dungeon->d[i][j].h);
			fwrite(&h, sizeof(int8_t), 1, file);
			pos++;
		}
	}

	/* room positions ;; 4 bytes per room */
	fseek(file, 1694, SEEK_SET);
	for(i = 0; i < dungeon->nr; i++) {
		int8_t x = (int8_t) dungeon->r[i].tl.x;
		int8_t w = (int8_t) dungeon->r[i].w;
		int8_t y = (int8_t) dungeon->r[i].tl.y;
		int8_t h = (int8_t) dungeon->r[i].h;

		fwrite(&x, sizeof(int8_t), 1, file);
		fwrite(&w, sizeof(int8_t), 1, file);
		fwrite(&y, sizeof(int8_t), 1, file);
		fwrite(&h, sizeof(int8_t), 1, file);
	}

	/*free(path);*/
	fclose(file);
}

/* prints the dungeon */
void print_dungeon(Dungeon * dungeon) {
	int i;
	int j;
	int h;

	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			dungeon->p[i][j].c = ' ';
		}
	}

	/* add corridors to the print buffer */
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			if(dungeon->d[i][j].p == 1) {
				dungeon->p[i][j].c = '#';
			}
		}
	}

	/* add rooms to the print buffer */
	for(h = 0; h < dungeon->nr; h++) {
		for(i = dungeon->r[h].tl.y; i < dungeon->r[h].br.y+1; i++) {
			for(j = dungeon->r[h].tl.x; j < dungeon->r[h].br.x+1; j++) {
				dungeon->p[i][j].c = '.';
			}
		}
	}


	/* print the print buffer */
	for(i = 0; i < dungeon->h; i++) {
		int j;
		for(j = 0; j < dungeon->w; j++) {
			printf("%c", (dungeon->p[i][j]).c);
		}
		printf("\n");
	}
}

/* (attempt to) place a room within a given dungeon */
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

	HW: ;

	int we = (rand() % 4) + 4; /* width, expanded, up to 4 more */
	int he = (rand() % 4) + 3; /* height, expanded, up to 4 more */

	if(we == he) {
		/* if we have a square, re-generate */
		goto HW;
	}

	new_room.h = he;
	new_room.w = we;

	new_room.br.x = x + new_room.w-1;
	new_room.br.y = y + new_room.h-1;

	/* check for rooms loaded into the dungeon buffer already */
	int i;
	int j;
	int placed = -1;
	int passed = 0;
	for(i = y; i < dungeon->h-1 && i < y+he; i++) {
		for(j = x; j < dungeon->w-1 && j < x+we; j++) {
			if(dungeon->p[i][j].c != '.') {
				passed++;
			}
		}
	}

	/* return a failure if not all cells within the "Room" passed */
	if(passed < we*he) {
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
	for(i = y; i < y+he; i++) {
		for(j = x; j < x+we; j++) {
			dungeon->p[i][j].c = '.';
			dungeon->d[i][j].h = 0;
		}
	}


	if(dungeon->nr < dungeon->mr) {
		dungeon->nr++;
		new_room.id = dungeon->nr-1; /* reflects position in the array */
		new_room.ctr.x = (new_room.w)/2 + new_room.tl.x;
		new_room.ctr.y = (new_room.h)/2 + new_room.tl.y;
		/* printf("%d: (%d, %d)\n", new_room.id, new_room.ctr.x, new_room.ctr.y); */
		dungeon->r[dungeon->nr-1] = new_room;
	} else {
		return -1;
	}


	return placed;
}

/* assistant function for gen_corridors() to check if all rooms are connected */
int all_connected(int * cnxns, int len) {
	int i;

	for(i = 0; i < len; i++) {
		if(cnxns[i] != 1) {
			return FALSE;
		}
	}

	return TRUE;
}

/* generates and marks corridors */
void gen_corridors(Dungeon * dungeon) {
	int connected[dungeon->nr];
	double dists[dungeon->nr];
	int max_paths = dungeon->nr * 3;
	Path paths[max_paths]; /* max paths is 3 * number of rooms */
	int path_cnt = 0;
	int	room_pos = 0; /* current room in use */
	int i;

	for(i = 0; i < dungeon->nr; i++) {
		dists[i] = -1; /* infinite at -1 */
	}
	dists[0] = 0;


	/* primary loop, goal is to connect all rooms; 0 means true */
	while(all_connected(connected, dungeon->nr) == FALSE && path_cnt < max_paths) {
		int i;
		double d;
		Path new_path;

		/* populate dists from the current position */
		for(i = 0; i < dungeon->nr; i++) {
			/* calculate distance */
			d =  sqrt(pow(dungeon->r[i].ctr.x - dungeon->r[room_pos].ctr.x, 2) + pow(dungeon->r[i].ctr.y - dungeon->r[room_pos].ctr.y, 2));
			dists[i] = d;
		}

		/* find the room to path to ;; if not connected already and the distance is shorter and isn't our current position */

		int next = -1;
		for(i = 0; i < dungeon->nr; i++) {
			if(connected[i] != 1 && next == -1 && room_pos != i) {
				next = i;
			} else if(connected[i] != 1 && dists[i] < dists[next] && room_pos != i) {
				next = i;
			}
		}

		/** this would - in the future - be the point of adding extraneous paths **/
		if(next != -1) {
			connected[room_pos] = 1;
			new_path.prev = room_pos;
			new_path.next = next;
			paths[path_cnt] = new_path;
			room_pos = next;
			path_cnt++;
		} else {
			break;
		}
	}

	/* populate the dungeon grid (draw the paths using x/y chasing/pathing) */

	/** temporary print for posterity (compiler Wall Werror) **/
	/*printf("%d\n", path_cnt);
	for(i = 0; i < path_cnt; i++) {
		printf("%d to %d\n", paths[i].prev, paths[i].next);
	}*/

	/* draw dungeon paths in the dungeon grid; start at room 0 as per above */

	for(i = 0; i < path_cnt; i++) {
		int x = dungeon->r[paths[i].prev].ctr.x;
		int y = dungeon->r[paths[i].prev].ctr.y;

		/*printf("%d: (%d, %d)\n", i, x, y);*/

		while(x != dungeon->r[paths[i].next].ctr.x || y != dungeon->r[paths[i].next].ctr.y) {
			int dirx = 0; /* -1 for left, 1 for right */
			int diry = 0; /* -1 for down, 1 for up */

			if(x < dungeon->r[paths[i].next].ctr.x) {
				dirx = 1;
			} else if(x > dungeon->r[paths[i].next].ctr.x) {
				dirx = -1;
			}

			if(y < dungeon->r[paths[i].next].ctr.y) {
				diry = 1;
			} else if(y > dungeon->r[paths[i].next].ctr.y) {
				diry = -1;
			}

			dungeon->d[y][x].p = 1;
			/* don't place corridors in rooms */
			if(dungeon->d[y][x].c != '.') {
				dungeon->d[y][x].c = '#';
				dungeon->d[y][x].h = 0;
			}

			/*
			if(dirx != 0 && diry != 0) {
				int dec = rand() % 2;
				if(dec == 0) {
					diry = 0;
				} else {
					dirx = 0;
				}
			}
			*/

			if(dirx == -1) {
				x--;
			} else if(dirx == 1) {
				x++;
			} else if(diry == -1) {
				y--;
			} else if(diry == 1) {
				y++;
			}
		}

	}



}

/* generate a blank dungeon */
void gen_dungeon(Dungeon * dungeon) {
	/*** top 3 (0, 1, 2) are reserved for the pseudo-HUD ***/
	int i, j;

	/* set all slots to spaces originally */
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			(dungeon->d[i][j]).c = ' ';	/* all basic rooms are spaces */
			int h = rand() % 256;
			(dungeon->d[i][j]).h = h;
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

	/* populate the rooms */
	int cnt = 0;
	int tst = 0;
	for(i = 0; dungeon->nr < dungeon->mr && cnt < 2000; i++) {
		tst = place_room(dungeon);
		if(tst < 0) {
			cnt++;
		}
	}


}

/* initializes the dungeon structure */
Dungeon init_dungeon(int h, int w, int mr) {
	Dungeon new_dungeon;
	new_dungeon.h	= h;
	new_dungeon.w	= w;
	new_dungeon.mr	= mr;
	new_dungeon.nr	= 0;

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


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* process commandline arguments */


	/* init the dungeon with default dungeon size and a max of 12 rooms */
	srand(time(NULL));
	Dungeon dungeon = init_dungeon(21, 80, 12);
	gen_dungeon(&dungeon);
	gen_corridors(&dungeon);


	print_dungeon(&dungeon);
	write_dungeon(&dungeon);


	/* free our arrays */
	free(dungeon.d);
	free(dungeon.r);
	return 0;
}