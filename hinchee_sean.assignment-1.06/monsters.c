#include <stdio.h>
#include <stdlib.h>
#include "dungeon_generator.h"
#include "binheap.h"

/* check if we can move to a location (objectively) */
Bool test_loc(Dungeon * dungeon, int x, int y, Sprite *s) {
	if(x > 0 && x < dungeon->w-1 && y > 0 && y < dungeon->h-1) {
		int hard = dungeon->d[y][x].h;
		if(dungeon->d[y][x].h < 255) {
			if(s->s.tu == FALSE && hard > 0)
				return FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

/* checks if a given sprite shares a room with the PC */
void with_pc(Dungeon * dungeon, Sprite * s, Bool *in) {
	int pc_rid	= -1;
	int s_rid	= -1;
	Sprite *pc = &(dungeon->ss[dungeon->pc]);

	int i;
	for(i = 0; i < dungeon->nr; i++) {
		if(s->p.x <= dungeon->r[i].br.x && s->p.y <= dungeon->r[i].br.y && s->p.x >= dungeon->r[i].tl.x && s->p.y >= dungeon->r[i].tl.y) {
			s_rid = i;
		}
		if(pc->p.x <= dungeon->r[i].br.x && pc->p.y <= dungeon->r[i].br.y && pc->p.x >= dungeon->r[i].tl.x && pc->p.y >= dungeon->r[i].tl.y) {
			pc_rid = i;
		}
	}
	if(pc_rid > 0 && s_rid > 0 && pc_rid == s_rid)
		*in = TRUE;
}

void gen_move_sprite(Dungeon * dungeon, int sn) {
	//make ss[sn] when possible
	int sx = dungeon->ss[sn].p.x;
	int sy = dungeon->ss[sn].p.y;
	int xs[8] = {-1,0,1,1,1,0,-1,-1};
	int ys[8] = {-1,-1,-1,0,1,1,1,0};

	Sprite *s = &(dungeon->ss[sn]);

	/* increment the turn */
	dungeon->ss[sn].t += (100 / s->s.s);

	Position new = {-1, -1};

	dungeon->d[s->p.y][s->p.x].h -= 85;
	if(dungeon->d[s->p.y][s->p.x].h < 0)
		dungeon->d[s->p.y][s->p.x].h = 0;

	// make sure we're alive
	if(s->a == TRUE) {
		int i;
		int j;
		int eb = rand() % 2; /* we have a 50% chance to behave erratically */
		for(i = 0; i < 8; i++) {
			int px = sx + xs[i];
			int py = sy + ys[i];

			if(px >= 0 && px < dungeon->w && py >= 0 && py < dungeon->h) {
				/* drunken PC movement as per assignment 1.04 */

				/* check erratic behaviour */
				if(s->s.eb == FALSE || (s->s.eb == TRUE && eb)) {
					/** check if intelligent / telepathic **/
					//new.x = sx;
					//new.y = sy;
					if(s->s.te == FALSE) {
						/* see if you're in the same room */
						Bool in_room = FALSE;
						with_pc(dungeon, s, &in_room);
						if(in_room == TRUE) {
							//cache PC location
							s->pc = dungeon->ss[dungeon->pc].p;

							IN: ;
							if(s->s.in == TRUE) {
								/* we are intelligent and can see our mark (tele or otherwise) */
								int k;
								int lowest = 0;
								Bool set = FALSE;
								if(s->s.tu) {
									//tunneling
									for(k = 0; k < 8; k++) {
										if(xs[k]+sx >= 0 && xs[k]+sx < dungeon->w && ys[k]+sy >= 0 && ys[k]+sy < dungeon->h) {
											if(dungeon->d[ys[k]+sy][xs[k]+sx].h < 255 && dungeon->cst[ys[k]+sy][xs[k]+sx] < dungeon->cst[ys[lowest]+sy][xs[lowest]+sx] && test_loc(dungeon, xs[k]+sx, ys[k]+sy, s) == TRUE && test_loc(dungeon, xs[lowest]+sx, ys[lowest]+sy, s) == TRUE) {
												lowest = k;
												set = TRUE;
											}
										}
									}
								} else {
									//non-tunneling
									for(k = 0; k < 8; k++) {
										px = xs[k]+sx;
										py = ys[k]+sy;
										if(px >= 0 && px < dungeon->w && py >= 0 && py < dungeon->h) {
											if(dungeon->d[py][px].h == 0 && dungeon->csnt[py][px] <= dungeon->csnt[ys[lowest]+sy][xs[lowest]+sx]) {
												lowest = k;
												set = TRUE;
											}
										}
									}
										/*
										for(k = 0; k < 8; k++) {
											if(xs[k]+sx >= 0 && xs[k]+sx < dungeon->w && ys[k]+sy >= 0 && ys[k]+sy < dungeon->h) {
												if(dungeon->d[ys[k]+sy][xs[k]+sx].h < 255 && dungeon->csnt[ys[k]+sy][xs[k]+sx] < dungeon->csnt[ys[lowest]+sy][xs[lowest]+sx] && test_loc(dungeon, xs[k]+sx, ys[k]+sy, s) == TRUE && test_loc(dungeon, xs[lowest]+sx, ys[lowest]+sy, s) == TRUE) {
													lowest = k;
													set = TRUE;
												}
											}
										}
										*/
								}
								if(set == TRUE) {
									new.x = xs[lowest] + sx;
									new.y = ys[lowest] + sy;
									break;
								} else {
									new.x = sx;
									new.y = sy;
									break;
								}

									/*
									if(test_loc(dungeon, px, py, s) == TRUE) {
										//if we can move to the point
										if(s->s.tu) {
											//tunneling
											if(new.x > 0 && new.y > 0 && new.x != sx && new.y != sy) {
												if(dungeon->cst[py][px] < dungeon->cst[new.y][new.x]) {
													new.x = px;
													new.y = py;
												}
											} else {
												new.x = px;
												new.y = py;
											}
										} else {
											//non-tunneling
											if(new.x > 0 && new.y > 0 && new.x != sx && new.y != sy) {
												if(dungeon->csnt[py][px] < dungeon->csnt[new.y][new.x]) {
													new.x = px;
													new.y = py;
												}
											} else {
												new.x = px;
												new.y = py;
											}
										}
									}
									*/
							} else {
								//if not intelligent
								if(s->pc.x < sx)
									px = sx - 1;
								if(s->pc.x > sx)
									px = sx + 1;
								if(s->pc.y < sy)
									py = sy - 1;
								if(s->pc.y > sy)
									py = sy + 1;

								if(test_loc(dungeon, px, py, s) == TRUE) {
									new.x = px;
									new.y = py;
									break;
								}
							}
						} else {
							//not in the same room and not telepathic
							//randomize
							goto PCEB;
						}
					} else {
						//we know where the PC is
						s->pc = dungeon->ss[dungeon->pc].p;
						/**
						intelligence test still applies
						we just treat it as if we're always in the room " "
						**/
						goto IN;
					}

					//printf("%c in room? %d\n",s->c , in_room);
				} else {
					/* we are erratically behaving */
					PCEB: ;
					j = 0;
					EB: ;
					int c = rand() % 9;
					px = xs[c] + sx;
					py = ys[c] + sy;
					/* try to find a place to go up to n times */
					if(test_loc(dungeon, px, py, s) == FALSE && j < 8) {
						j++;
						goto EB;
					}
					if(test_loc(dungeon, px, py, s) == TRUE) {
						/* if the location is okay, commit it*/
						new.x = px;
						new.y = py;
					}

					break;
				}
			}
		}
	}

	/* safety net */
	if(new.x < 0)
		new.x = sx;
	if(new.y < 0)
		new.y = sy;

	dungeon->ss[sn].to.x = new.x;
	dungeon->ss[sn].to.y = new.y;

	if(new.x == dungeon->ss[dungeon->pc].p.x && new.y == dungeon->ss[dungeon->pc].p.y)
		dungeon->go = TRUE;

	/* check if we have to kill another sprite */
	int i;
	for(i = 0; i < dungeon->ns; i++) {
		if(i != sn) {
			if((dungeon->ss[i].to.x == dungeon->ss[sn].to.x) && (dungeon->ss[i].to.y == dungeon->ss[sn].to.y) && dungeon->ss[sn].a == TRUE)
				dungeon->ss[i].a = FALSE;
			/*
			else if(dungeon->ss[i].p.x == dungeon->ss[sn].p.x && dungeon->ss[i].p.y == dungeon->ss[sn].p.y && dungeon->ss[i].a == TRUE)
				dungeon->ss[sn].a = FALSE;
			*/
		}
	}
}

/* parse and apply a movement */
void parse_move(Dungeon * dungeon, int sn) {
	dungeon->ss[sn].p.x = dungeon->ss[sn].to.x;
	dungeon->ss[sn].p.y = dungeon->ss[sn].to.y;
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
Sprite * gen_sprite(Dungeon * dungeon, char c, int x, int y, int r) {
	Sprite * s = initSprite();

	setSpriteC(s, c);
	setSpriteA(s, TRUE);

    /* set stats */
    if(s.c == '@') {
        setSpriteSS(s, 10);
        setSpriteSTu(s, FALSE);
		setSpriteSEb(s, FALSE);
		setSpriteSTe(s, FALSE);
		setSpriteSIn(s, FALSE);
	} else {
        /* if not the pc a value 5-20 */
        setSpriteSS(s, (rand() % 16) + 5);
        /* generate stats */
        setSpriteSIn(s, rand() % 2);
        setSpriteSTe(s, rand() % 2);
        setSpriteSTu(s, rand() % 2);
        setSpriteSEb(s, rand() % 2);

        /* set character as per assignment 4 */
        if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, '0');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, '1');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, '2');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, '3');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, '4');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, '5');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, '6');
        else if(getSpriteSIn(s) == FALSE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, '7');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, '8');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, '9');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, 'a');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == FALSE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, 'b');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, 'c');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == FALSE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, 'd');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == FALSE)
            setSpriteC(s, 'e');
        else if(getSpriteSIn(s) == TRUE && getSpriteSTe(s) == TRUE && getSpriteSTu(s) == TRUE && getSpriteSEb(s) == TRUE)
            setSpriteC(s, 'f');
    }

    /* put the tunneling monsters anywhere */
    if(getSpriteSTu(s) == TRUE) {
		int t = 0;
		PRT: ;
        /* randomize location anywhere in the dungeon */
        if(x < 0 || x > dungeon->w) {
            x = (rand() % (dungeon->w-2)) + 1;
        }
        if(y < 0 || y > dungeon->h) {
            y = (rand() % (dungeon->h-2)) + 1;
        }

		if(getSpriteC(s) != '@' && dungeon->nr > 1) {
			setSpritePX(s, x);
		    setSpritePY(s, y);

			Bool w_pc = FALSE;
			with_pc(dungeon, s, &w_pc);
			if(w_pc == TRUE && t < 8) {
				t++;
				goto PRT;
			}
		}
    }

    /* place in a room if 1 or more. implicitly random ;; force in a room even if tunneling */
    if(r > 0 || getSpriteSTu(s) == FALSE) {
		int t = 0;
		PRNT: ;
        int r_id = rand() % dungeon->nr;
        x = (rand() % dungeon->r[r_id].w) + getPosX(dungeon->r[r_id].tl);
        y = (rand() % dungeon->r[r_id].h) + getPosY(dungeon->r[r_id].tl);

		if(getSpriteC(s) != '@' && dungeon->nr > 1) {
			setSpritePX(s, x);
		    setSpritePY(s, y);

			Bool w_pc = FALSE;
			with_pc(dungeon, s, &w_pc);
			if(w_pc == TRUE && t < 8) {
				t++;
				goto PRNT;
			}
		}
    } else {

    }

    setSpritePX(s, x);
    setSpritePY(s, y);
	setSpriteToX(s, x);
	setSpriteToY(s, y);
	//s.t = 100/s.s.s;
	setSpriteT(s, 0);

	return s;
}

/* checks if any monsters other than the player are alive */
Bool check_any_monsters(Dungeon * dungeon) {
	int i;
	for(i = 0; i < dungeon->ns; i++) {
		if(getSpriteAA(dungeon->ss, i) == TRUE && i != 0)
			return TRUE;
	}

	return FALSE;
}
