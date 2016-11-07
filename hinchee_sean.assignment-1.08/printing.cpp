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
	clear();
	refresh();
	
	/* color definitions */
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
	
	

	int i;
	int j;
	int h;

	for(i = 0; i < dungeon->h; i++) {
		for(j = 0; j < dungeon->w; j++) {
			dungeon->p[i][j].c = ' ';
			dungeon->p[i][j].color = COLOR_BLACK;
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
		for(i = getPosY(dungeon->r[h].tl); i < getPosY(dungeon->r[h].br)+1; i++) {
			for(j = getPosX(dungeon->r[h].tl); j < getPosX(dungeon->r[h].br)+1; j++) {
				dungeon->p[i][j].c = '.';
			}
		}
	}

	dungeon->p[getPosY(dungeon->su)][getPosX(dungeon->su)].c = '<';
	dungeon->p[getPosY(dungeon->sd)][getPosX(dungeon->sd)].c = '>';

	/* add items map to buffer */
	for(i = 0; i < dungeon->nit; i++) {
		char s = '*';
		
		switch(dungeon->items[i].t) {
		case WEAPON: s = '|'; break;
		case OFFHAND: s = ')'; break;
		case RANGED: s = '}'; break;
		case ARMOR: s = '['; break;
		case HELMET: s = ']'; break;
		case CLOAK: s = '('; break;
		case GLOVES: s = '{'; break;
		case BOOTS: s = '\\'; break;
		case RING: s = '='; break;
		case AMULET: s = '\"'; break;
		case LIGHT: s = '_'; break;
		case SCROLL: s = '~'; break;
		case BOOK: s = '\?'; break;
		case FLASK: s = '!'; break;
		case GOLD: s = '$'; break;
		case AMMUNITION: s = '/'; break;
		case FOOD: s = ','; break;
		case WAND: s = '-'; break;
		case CONTAINER: s = '%'; break;
		default: break;
		}
		
		/* item colors */
		int c = COLOR_WHITE;
		
		switch(dungeon->items[i].c) {
		case RED: c = COLOR_RED; break;
		case  GREEN: c = COLOR_GREEN; break;
		case  BLUE: c = COLOR_BLUE; break;
		case  CYAN: c = COLOR_CYAN; break;
		case  YELLOW: c = COLOR_YELLOW; break;
		case  MAGENTA: c = COLOR_MAGENTA; break;
		case  WHITE: c = COLOR_WHITE; break;
		case  BLACK: c = COLOR_BLACK; break;
		default: c = COLOR_WHITE; break;
		}
		
		dungeon->p[dungeon->items[i].p.y][dungeon->items[i].p.x].c = s;
		dungeon->p[dungeon->items[i].p.y][dungeon->items[i].p.x].color = c;
	}

	/* add sprites to the print buffer */
	for(i = 0; i < dungeon->ns; i++) {
		if(getSpriteAA(dungeon->ss, i) == TRUE) {
			
			/* colors */
			int c = COLOR_WHITE;
			
			switch(dungeon->ss[i].color) {
			case RED: c = COLOR_RED; break;
			case  GREEN: c = COLOR_GREEN; break;
			case  BLUE: c = COLOR_BLUE; break;
			case  CYAN: c = COLOR_CYAN; break;
			case  YELLOW: c = COLOR_YELLOW; break;
			case  MAGENTA: c = COLOR_MAGENTA; break;
			case  WHITE: c = COLOR_WHITE; break;
			case  BLACK: c = COLOR_BLACK; break;
			default: c = COLOR_WHITE; break;
			}
			
			dungeon->p[dungeon->ss[i].p.y][dungeon->ss[i].p.x].color = c;
			
			dungeon->p[getSpriteAPY(dungeon->ss, i)][getSpriteAPX(dungeon->ss, i)].c = getSpriteAC(dungeon->ss, i);
		}
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

	updateMemory(dungeon);

	/* print the print buffer */
	for(i = 0; i < dungeon->h; i++) {
		int j;
		for(j = 0; j < dungeon->w; j++) {
			attron(COLOR_PAIR(dungeon->p[i][j].color));
			//attron(COLOR_PAIR(COLOR_RED));
			
			mvaddch(i+1, j, getMem(dungeon, i, j));
			refresh();
			
			attroff(COLOR_PAIR(dungeon->p[i][j].color));
		}
	}
    //clear();
    //refresh();
}

/* prints the dungeon */
void print_dungeon_nnc(Dungeon * dungeon, int nt, int t) {
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
		for(i = getPosY(dungeon->r[h].tl); i < getPosY(dungeon->r[h].br)+1; i++) {
			for(j = getPosX(dungeon->r[h].tl); j < getPosX(dungeon->r[h].br)+1; j++) {
				dungeon->p[i][j].c = '.';
			}
		}
	}
	

	/* add sprites to the print buffer */
	for(i = 0; i < dungeon->ns; i++) {
		if(getSpriteAA(dungeon->ss, i) == TRUE)
			//dungeon->p[dungeon->ss[i].p.y][dungeon->ss[i].p.x].c = dungeon->ss[i].c;
			
			/* item colors */
			/*
			
			*/
			dungeon->p[getSpriteAPY(dungeon->ss, i)][getSpriteAPX(dungeon->ss, i)].c = getSpriteAC(dungeon->ss, i);
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
