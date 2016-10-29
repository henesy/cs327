#include <stdlib.h>
#include <math.h>
#include "dungeon_generator.h"


/* (attempt to) place a room within a given dungeon */
int place_room(Dungeon * dungeon) {
	int x = (rand() % (dungeon->w-1)) +1;
	int y = (rand() % (dungeon->h-1)) +1;
	Room new_room;
	new_room.br = initPos();
	new_room.tl = initPos();
	new_room.ctr = initPos();
	/*
	set top right to rng number; might be worth making a more detailed placer with a lower
		fail rate
	*/
	setPosX(new_room.tl, x);
	setPosY(new_room.tl, y);
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

	setPosX(new_room.br, x + new_room.w-1);
	setPosY(new_room.br, y + new_room.h-1);

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
	if(getPosX(new_room.br) >= dungeon->w || getPosY(new_room.br) >= dungeon->h) {
		return placed;
	}


	/* check for surrounding rooms */

	/* top row */
	for(i = getPosX(new_room.tl)-1; i < getPosX(new_room.br)+2 && getPosX(new_room.tl)-1 >= 0 && getPosX(new_room.br)+1 < dungeon->w && getPosY(new_room.tl)-1 >= 0; i++) {
		if((dungeon->p[getPosY(new_room.tl)-1][i]).c == '.') {
			return placed;
		}
	}

	/* bottom row */
	for(i = getPosX(new_room.tl)-1; i < getPosX(new_room.br)+2 && getPosX(new_room.tl)-1 >= 0 && getPosX(new_room.br)+1 < dungeon->w && getPosY(new_room.br)+1 < dungeon->h; i++) {
		if((dungeon->p[getPosY(new_room.br)+1][i]).c == '.') {
			return placed;
		}
	}

	/* left side */
	for(i = getPosY(new_room.tl); i < getPosY(new_room.br)+1 && getPosY(new_room.br)+1 < dungeon->h && getPosX(new_room.tl)-1 >= 0; i++) {
		if((dungeon->p[i][getPosX(new_room.tl)-1]).c == '.') {
			return placed;
		}
	}

	/* right side */
	for(i = getPosY(new_room.tl); i < getPosY(new_room.br)+1 && getPosY(new_room.br)+1 < dungeon->h && getPosX(new_room.br)+1 < dungeon->w; i++) {
		if((dungeon->p[i][getPosX(new_room.br)+1]).c == '.') {
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
		setPosX(new_room.ctr, (new_room.w)/2 + getPosX(new_room.tl));
		setPosY(new_room.ctr, (new_room.h)/2 + getPosY(new_room.tl));
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
	int i;
	int connected[dungeon->nr];
	for(i = 0; i < dungeon->nr; i++) {
		connected[i] = 0;
	}
	//memset(connected, 0, dungeon->nr * sizeof(int));
	double dists[dungeon->nr];
	for(i = 0; i < dungeon->nr; i++) {
		dists[i] = 0;
	}
	//memset(dists, 0.0, dungeon->nr * sizeof(double));
	int max_paths = dungeon->nr * 3;
	Path paths[max_paths]; /* max paths is 3 * number of rooms */
	int path_cnt = 0;
	int	room_pos = 0; /* current room in use */

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
			d =  sqrt(pow(getPosX(dungeon->r[i].ctr) - getPosX(dungeon->r[room_pos].ctr), 2) + pow(getPosY(dungeon->r[i].ctr) - getPosY(dungeon->r[room_pos].ctr), 2));
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

	}

	/* populate the dungeon grid (draw the paths using x/y chasing/pathing) */

	/* draw dungeon paths in the dungeon grid; start at room 0 as per above */

	for(i = 0; i < path_cnt; i++) {
		int x = getPosX(dungeon->r[paths[i].prev].ctr);
		int y = getPosY(dungeon->r[paths[i].prev].ctr);

		/*printf("%d: (%d, %d)\n", i, x, y);*/

		while(x != getPosX(dungeon->r[paths[i].next].ctr) || y != getPosY(dungeon->r[paths[i].next].ctr)) {
			int dirx = 0; /* -1 for left, 1 for right */
			int diry = 0; /* -1 for down, 1 for up */

			if(x < getPosX(dungeon->r[paths[i].next].ctr)) {
				dirx = 1;
			} else if(x > getPosX(dungeon->r[paths[i].next].ctr)) {
				dirx = -1;
			}

			if(y < getPosY(dungeon->r[paths[i].next].ctr)) {
				diry = 1;
			} else if(y > getPosY(dungeon->r[paths[i].next].ctr)) {
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

	/* set the stairs */
	int x;
	int y;

	int r_id = rand() % dungeon->nr;
	x = (rand() % dungeon->r[r_id].w) + getPosX(dungeon->r[r_id].tl);
	y = (rand() % dungeon->r[r_id].h) + getPosY(dungeon->r[r_id].tl);
	
	//lol
	dungeon->sd = initPos();
	dungeon->su = initPos();
	
	setPosX(dungeon->sd, x);
	setPosY(dungeon->sd, y);
	
	SD: ;
	r_id = rand() % dungeon->nr;
	x = (rand() % dungeon->r[r_id].w) + getPosX(dungeon->r[r_id].tl);
	y = (rand() % dungeon->r[r_id].h) + getPosY(dungeon->r[r_id].tl);
	setPosX(dungeon->su, x);
	setPosY(dungeon->su, y);
	
	if(getPosX(dungeon->su) == getPosX(dungeon->sd) && getPosY(dungeon->su) == getPosY(dungeon->sd))
		goto SD;

}

/* initializes the dungeon structure */
Dungeon init_dungeon(int h, int w, int mr) {
	Dungeon new_dungeon;
	new_dungeon.h	= h;
	new_dungeon.w	= w;
	new_dungeon.mr	= mr;
	new_dungeon.nr	= 0;
	new_dungeon.ns	= 0;
	new_dungeon.ms	= w*h; /* max sprites would be 1 per dungeon slot */
	new_dungeon.t	= 0;
	new_dungeon.go	= FALSE;

	/* dungeon buffer allocation+0'ing */
	new_dungeon.d = (Tile**)calloc(new_dungeon.h, sizeof(Tile *));

	int i;
	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.d[i] = (Tile*)calloc(new_dungeon.w, sizeof(Tile));
	}

	/* dungeon visual buffer allocation+0'ing */
	new_dungeon.p = (Tile**)calloc(new_dungeon.h, sizeof(Tile *));

	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.p[i] = (Tile*)calloc(new_dungeon.w, sizeof(Tile));
	}

	/* rooms allocation+0'ing */
	new_dungeon.r = (Room*)calloc(new_dungeon.mr, sizeof(Room));

	/* sprites allocation */
	new_dungeon.ss = (Sprite*)calloc(new_dungeon.ms, sizeof(Sprite *));

	/* djikstra-based cost map allocation */
	new_dungeon.cst = (int**)calloc(w*h, sizeof(int *));
	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.cst[i] = (int*)calloc(new_dungeon.w, sizeof(int));
	}

	/* djikstra-based cost map allocation */
	new_dungeon.csnt = (int**)calloc(w*h, sizeof(int *));
	for(i = 0; i < new_dungeon.h; i++) {
		new_dungeon.csnt[i] = (int*)calloc(new_dungeon.w, sizeof(int));
	}

	return new_dungeon;
}
