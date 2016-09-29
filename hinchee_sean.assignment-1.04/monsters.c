#include <stdlib.h>
#include "dungeon_generator.h"
#include "binheap.h"

/* move a sprite following its built in rules */
void gen_move_sprite(Dungeon * dungeon, int sn, binheap_t * h) {
    int xs[8] = {-1,0,1,1,1,0,-1,-1};
	int ys[8] = {-1,-1,-1,0,1,1,1,0};

    int sx = dungeon->ss[sn].p.x;
    int sy = dungeon->ss[sn].p.y;

    int i;
    for(i = 0; i < 8; i++) {
        int x = sx + xs[i];
        int y = sy + ys[i];
        if(x > 0 && x < dungeon->w-1 && y > 0 && y < dungeon->h-1) {
            int hard = dungeon->d[y][x].h;
            if(hard < 255) {
                /* protect against map edges */

            }
        }
    }

}

/* add a sprite to the dungeon */
void add_sprite(Dungeon * dungeon, Sprite s) {
	if(dungeon->ns < dungeon->ms) {
		dungeon->ns++;
	} else {
		goto END;
	}

	if(s.c == '@') {
		dungeon->pc = dungeon->ns - 1;
    }

	dungeon->ss[dungeon->ns - 1] = s;

	END: ;
}

/*
generate a sprite, because in-line structs are icky
if r(oom) = 1 then x and y will be ignored and should be passed as -1 as such (normally meaning random)
*/
Sprite gen_sprite(Dungeon * dungeon, char c, int x, int y, int r) {
	Sprite s;

	s.c = c;

    /* set stats */
    if(s.c == '@') {
        s.s.s = 10;
        s.s.tu = FALSE;
	} else {
        /* if not the pc a value 5-20 */
        s.s.s = (rand() % 16) + 5;
        /* generate stats */
        s.s.in = rand() % 2;
        s.s.te = rand() % 2;
        s.s.tu = rand() % 2;
        s.s.eb = rand() % 2;

        /* set character as per assignment 4 */
        if(s.s.in == FALSE && s.s.te == FALSE && s.s.tu == FALSE && s.s.eb == FALSE)
            s.c = '0';
        else if(s.s.in == FALSE && s.s.te == FALSE && s.s.tu == FALSE && s.s.eb == TRUE)
            s.c = '1';
        else if(s.s.in == FALSE && s.s.te == FALSE && s.s.tu == TRUE && s.s.eb == FALSE)
            s.c = '2';
        else if(s.s.in == FALSE && s.s.te == FALSE && s.s.tu == TRUE && s.s.eb == TRUE)
            s.c = '3';
        else if(s.s.in == FALSE && s.s.te == TRUE && s.s.tu == FALSE && s.s.eb == FALSE)
            s.c = '4';
        else if(s.s.in == FALSE && s.s.te == TRUE && s.s.tu == FALSE && s.s.eb == TRUE)
            s.c = '5';
        else if(s.s.in == FALSE && s.s.te == TRUE && s.s.tu == TRUE && s.s.eb == FALSE)
            s.c = '6';
        else if(s.s.in == FALSE && s.s.te == TRUE && s.s.tu == TRUE && s.s.eb == TRUE)
            s.c = '7';
        else if(s.s.in == TRUE && s.s.te == FALSE && s.s.tu == FALSE && s.s.eb == FALSE)
            s.c = '8';
        else if(s.s.in == TRUE && s.s.te == FALSE && s.s.tu == FALSE && s.s.eb == TRUE)
            s.c = '9';
        else if(s.s.in == TRUE && s.s.te == FALSE && s.s.tu == TRUE && s.s.eb == FALSE)
            s.c = 'a';
        else if(s.s.in == TRUE && s.s.te == FALSE && s.s.tu == TRUE && s.s.eb == TRUE)
            s.c = 'b';
        else if(s.s.in == TRUE && s.s.te == TRUE && s.s.tu == FALSE && s.s.eb == FALSE)
            s.c = 'c';
        else if(s.s.in == TRUE && s.s.te == TRUE && s.s.tu == FALSE && s.s.eb == TRUE)
            s.c = 'd';
        else if(s.s.in == TRUE && s.s.te == TRUE && s.s.tu == TRUE && s.s.eb == FALSE)
            s.c = 'e';
        else if(s.s.in == TRUE && s.s.te == TRUE && s.s.tu == TRUE && s.s.eb == TRUE)
            s.c = 'f';
    }

    /* put the tunneling monsters anywhere */
    if(s.s.tu == TRUE) {
        /* randomize location anywhere in the dungeon */
        if(x < 0 || x > dungeon->w) {
            x = (rand() % (dungeon->w-2)) + 1;
        }
        if(y < 0 || y > dungeon->h) {
            y = (rand() % (dungeon->h-2)) + 1;
        }
    }

    /* place in a room if 1 or more. implicitly random ;; force in a room even if tunneling */
    if(r > 0 || s.s.tu == FALSE) {
        int r_id = rand() % dungeon->nr;
        x = (rand() % dungeon->r[r_id].w) + dungeon->r[r_id].tl.x;
        y = (rand() % dungeon->r[r_id].h) + dungeon->r[r_id].tl.y;
    } else {

    }

    s.p.x = x;
    s.p.y = y;

	return s;
}
