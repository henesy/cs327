#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
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
	int			c;		/* if connected or not; TRUE/FALSE switch */
} Room;

typedef struct {
	Tile **	d;	/* dungeon buffer */
	Tile **	p;	/* print buffer */
	int		h;	/* height */
	int		w;	/* width */
	int		nr; /* number of rooms */
	int		mr;	/* max rooms */
	Room *	r;	/* rooms buffer */
	int		v;	/* file version */
	int		s;	/* file size */
} Dungeon;


/* reads from a dungeon file */
void read_dungeon(Dungeon * dungeon, char * path) {
	FILE * file;
	file = fopen(path, "rb+");
	if(file == NULL) {
		fprintf(stderr, "FILE ERROR: Could not open dungeon file at %s! read_dungeon()\n", path);
        exit(1);
	}

	/* read the file-type marker */
	fseek(file, 0, SEEK_SET);
	char marker[6];
	fread(marker, 1, 6, file);

	/* read the file version marker */
	fseek(file, 6, SEEK_SET);
	uint32_t file_version;
	uint32_t file_version_be;
	fread(&file_version_be, sizeof(uint32_t), 1, file);
	file_version = be32toh(file_version_be);
	dungeon->v = file_version;

	/* read the size of file */
	fseek(file, 10, SEEK_SET);
	uint32_t size;
	uint32_t size_be;
	fread(&size_be, sizeof(uint32_t), 1, file);
	size = be32toh(size_be);
	dungeon->s = size;

	/* read the hardness values in */
	fseek(file, 14, SEEK_SET);
	int i;
	int j;
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			int h;
			int8_t h_8;
			fread(&h_8, sizeof(int8_t), 1, file);
			h = (int) h_8;
			dungeon->d[i][j].h = h;
		}
	}

	/* read in rooms in dungeon */
	fseek(file, 1694, SEEK_SET);
	/* might want to make this just counted in 4's by the loop below, but w/e, math, amirite? */
	int room_i = 0;
	int room_count = (size - 1693) / 4;
	dungeon->nr = room_count;
	dungeon->r = calloc(room_count, sizeof(Room));
	/* could probably be replaced with a getpos() call for complete-ness */
	int pos;
	for(pos = 1694; pos < size; pos += 4) {
		int x_8;
		int w_8;
		int y_8;
		int h_8;
		fread(&x_8, sizeof(int8_t), 1, file);
		fread(&w_8, sizeof(int8_t), 1, file);
		fread(&y_8, sizeof(int8_t), 1, file);
		fread(&h_8, sizeof(int8_t), 1, file);

		dungeon->r[room_i].tl.x = (int8_t) x_8;
		dungeon->r[room_i].w = (int8_t) w_8;
		dungeon->r[room_i].tl.y = (int8_t) y_8;
		dungeon->r[room_i].h = (int8_t) h_8;
		dungeon->r[room_i].br.x = ((int8_t) x_8) + dungeon->r[room_i].w-1;
		dungeon->r[room_i].br.y = ((int8_t) y_8) + dungeon->r[room_i].h-1;



		room_i++;
	}


	/* populate the rooms and corridors if not in rooms */
	/* add rooms to the dungeon buffer */
	int h;
	for(h = 0; h < dungeon->nr; h++) {
		for(i = dungeon->r[h].tl.y; i < dungeon->r[h].br.y+1; i++) {
			for(j = dungeon->r[h].tl.x; j < dungeon->r[h].br.x+1; j++) {
				dungeon->d[i][j].c = '.';
			}
		}
	}

	/* add corridors to the dungeon buffer */
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			if(dungeon->d[i][j].c != '.' && dungeon->d[i][j].h == 0) {
				dungeon->d[i][j].c = '#';
				dungeon->d[i][j].p = 1;
			}
		}
	}


	fclose(file);
}

/* writes the dungeon file to ~/.rlg327/dungeon */
void write_dungeon(Dungeon * dungeon, char * path) {
	FILE * file;

	/* folder creation logic */
	char * env_home = getenv("HOME");
	char * fdir_path;
	fdir_path = calloc(strlen(env_home) + 9, sizeof(char));
	strcpy(fdir_path, env_home);
	strcat(fdir_path, "/.rlg327");
	mkdir(fdir_path, S_IRWXU);
	/* mkdir will return -1 when it fails, but it will fail if the file exists so it doesn't especially matter to catch it as no output would be provided */


	file = fopen(path, "wb+");
	if(file == NULL) {
		fprintf(stderr, "FILE ERROR: Could not open dungeon file at %s! write_dungeon()\n", path);
        exit(1);
	}

	/* write the file-type marker */
	fseek(file, 0, SEEK_SET);
	char marker[7];
	strcpy(marker, "RLG327");
	fwrite(marker, sizeof(char), 6, file);

	/* write the file version marker */
	fseek(file, 6, SEEK_SET);
	uint32_t file_version = 0;
	uint32_t file_version_be = htobe32(file_version);
	fwrite(&file_version_be, sizeof(uint32_t), 1, file);

	/* write the size of the file ;; unsure how to properly calculate */
	fseek(file, 10, SEEK_SET);
 	uint32_t size = 1693 + (4 * dungeon->nr);
	uint32_t size_be = htobe32(size);
	fwrite(&size_be, sizeof(uint32_t), 1, file);

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

	free(fdir_path);
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
			if(dungeon->d[i][j].p == 1 || dungeon->d[i][j].c == '#') {
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
int all_connected(int * cnxns, Dungeon * dungeon) {
	int i;

	for(i = 0; i < dungeon->nr; i++) {
		if(cnxns[i] != 1 || dungeon->r[i].c != TRUE) {
			return FALSE;
		}
	}

	return TRUE;
}

/* generates and marks corridors */
void gen_corridors(Dungeon * dungeon) {
	int connected[dungeon->nr];
	memset(connected, 0, dungeon->nr * sizeof(int));
	double dists[dungeon->nr];
	memset(dists, 0.0, dungeon->nr * sizeof(double));
	int max_paths = dungeon->nr * 3;
	Path paths[max_paths]; /* max paths is 3 * number of rooms */
	int path_cnt = 0;
	int	room_pos = 0; /* current room in use */
	int i;

	for(i = 0; i < dungeon->nr; i++) {
		dists[i] = -1; /* infinite at -1 */
	}
	dists[0] = 0;

	/* ensure all rooms are disconnected */
	for(i = 0; i < dungeon->nr; i++) {
		dungeon->r[i].c = FALSE;
	}

	/* primary loop, goal is to connect all rooms; 0 means true */
	while(all_connected(connected, dungeon) == FALSE && path_cnt < max_paths) {
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
			dungeon->r[room_pos].c = TRUE;
			dungeon->r[next].c = TRUE;
			connected[room_pos] = 1;
			new_path.prev = room_pos;
			new_path.next = next;
			paths[path_cnt] = new_path;
			room_pos = next;
			path_cnt++;
		} else {
			break;
		}

		/*
		printf("number of paths, max paths: %d, %d\n", path_cnt, max_paths);
		printf("number of rooms: %d\n", dungeon->nr);
		for(i = 0; i < dungeon->nr; i++) {
			printf("room %d is %d in Room and %d in cxns\n", i, dungeon->r[i].c, connected[i]);
		}
		printf("next is: %d\n", next);
		*/

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
			int h = (rand() % 254) + 1;
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

void test_args(char * argv_l, int * s, int * l) {
		if(strcmp(argv_l, "--save") == 0) {
			*s = TRUE;
		} else if(strcmp(argv_l, "--load") == 0) {
			*l = TRUE;
		}
}


/* Basic procedural dungeon generator */
int main(int argc, char * argv[]) {
	/* process commandline arguments */
	int saving = FALSE;
	int loading = FALSE;
	if(argc == 3) {
		/* both --save and --load */
		int i;
		for(i = 1; i < argc; i++) {
			test_args(argv[i], &saving, &loading);
		}
	} else if(argc == 2) {
		/* one of --save or --load */
		test_args(argv[1], &saving, &loading);
	} else if(argc > 3) {
		/* more than 2 commandline arguments, argv[0] is gratuitous */
		printf("Too many arguments!\n");
	} else {
		/* other; most likely 0 */
	}

	/* init the dungeon with default dungeon size and a max of 12 rooms */
	srand(time(NULL));
	
	/* create 2 char pointers so as not to pollute the original HOME variable */
	char * env_path = getenv("HOME");
	char * path = calloc(strlen(env_path) + 17, sizeof(char));
	strcpy(path, env_path);
	strcat(path, "/.rlg327/dungeon");
	Dungeon dungeon = init_dungeon(21, 80, 12);
	
	if(loading == FALSE) {
		gen_dungeon(&dungeon);
		gen_corridors(&dungeon);
	} else {
		read_dungeon(&dungeon, path);
	}

	print_dungeon(&dungeon);

	if(saving == TRUE) {
		write_dungeon(&dungeon, path);
	}

	/* free our arrays */
	int i;
	for(i = 0; i < dungeon.h; i++) {
		free(dungeon.d[i]);
	}
	free(dungeon.d);
	for(i = 0; i < dungeon.h; i++) {
		free(dungeon.p[i]);
	}
	free(dungeon.p);
	free(dungeon.r);
	free(path);
	return 0;
}
