#include <stdio.h>
#include <ncurses.h>
#include "dungeon_generator.h"


/* print hardness */
void print_hardness(Dungeon * dungeon) {
}

/* prints heatmap */
void print_t_heatmap(Dungeon * dungeon) {
	int i;
	int j;
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			int c = dungeon->cst[i][j];
			if(c >= 0 && c < 10) {
				printf("%d", c);
			} else if(c >= 10 && c < 36) {
				printf("%c", 'a' + (c - 10));
			} else if(c >= 36 && c < 62) {
				printf("%c", 'A' + (c - 36));
			} else {
				printf("%c", dungeon->d[i][j].c);
			}
		}
		printf("\n");
	}

}

/* prints heatmap */
void print_nont_heatmap(Dungeon * dungeon) {
	int i;
	int j;
	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			int c = dungeon->csnt[i][j];
			if(c >= 0 && c < 10) {
				printf("%d", c);
			} else if(c >= 10 && c < 36) {
				printf("%c", 'a' + (c - 10));
			} else if(c >= 36 && c < 62) {
				printf("%c", 'A' + (c - 36));
			} else {
				printf("%c", dungeon->d[i][j].c);
			}
		}
		printf("\n");
	}

}

/* prints the dungeon */
void print_dungeon(Dungeon * dungeon, int nt, int t) {
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
			if(dungeon->d[i][j].p == 1 || dungeon->d[i][j].c == '#' || dungeon->d[i][j].h == 0) {
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

	/* add sprites to the print buffer */
	for(i = 0; i < dungeon->ns; i++) {
		//printf("%d, %d: %c speed: %d turn: %d\n", dungeon->ss[i].p.y, dungeon->ss[i].p.x, dungeon->ss[i].c, dungeon->ss[i].s.s, dungeon->ss[i].t);
		if(dungeon->ss[i].a == TRUE)
			dungeon->p[dungeon->ss[i].p.y][dungeon->ss[i].p.x].c = dungeon->ss[i].c;
	}

	/* print non-tunnelling dijkstra's */
	if(nt > 0) {
		for(i = 0; i < dungeon->h; i++) {
			for(j = 0; j < dungeon->w; j++) {
				if(dungeon->d[i][j].h == 0) {
					int c = dungeon->csnt[i][j];
					if(c >= 0 && c < 10) {
						dungeon->p[i][j].c = '0' + c;
					} else if(c >= 10 && c < 36) {
						dungeon->p[i][j].c = 'a' + (c - 10);
					} else if(c >= 36 && c < 62) {
						dungeon->p[i][j].c = 'A' + (c - 36);
					}
				}
			}
		}
	}

	/* print tunnelling dijkstra's */
	if(t > 0) {
		for(i = 0; i < dungeon->h; i++) {
			for(j = 0; j < dungeon->w; j++) {
				int c = dungeon->cst[i][j];
				if(c >= 0 && c < 10) {
					dungeon->p[i][j].c = '0' + c;
				} else if(c >= 10 && c < 36) {
					dungeon->p[i][j].c = 'a' + (c - 10);
				} else if(c >= 36 && c < 62) {
					dungeon->p[i][j].c = 'A' + (c - 36);
				}
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
