#include <stdio.h>
#include <stdlib.h>
#include "dungeon_generator.h"
#include "binheap.h"

/* check if we can move to a location (objectively) */
Bool test_loc(Dungeon * dungeon, int x, int y, Sprite *s) {
	if(x > 0 && x < dungeon->w-1 && y > 0 && y < dungeon->h-1) {
		int hard = dungeon->d[y][x].h;
		if(dungeon->d[y][x].h < 255) {
			if(getSpriteSTu(s) == FALSE && hard > 0)
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
	// Sprite *pc = &(dungeon->ss[dungeon->pc]);
	Sprite *pc = thisASprite(dungeon->ss, dungeon->pc);

	int i;
	for(i = 0; i < dungeon->nr; i++) {
		if(getSpritePX(s) <= getPosX(dungeon->r[i].br) && getSpritePY(s) <= getPosY(dungeon->r[i].br) && getSpritePX(s) >= getPosX(dungeon->r[i].tl) && getSpritePY(s) >= getPosY(dungeon->r[i].tl)) {
			s_rid = i;
		}
		if(getSpritePX(pc) <= getPosX(dungeon->r[i].br) && getSpritePY(pc) <= getPosY(dungeon->r[i].br) && getSpritePX(pc) >= getPosX(dungeon->r[i].tl) && getSpritePY(pc) >= getPosY(dungeon->r[i].tl)) {
			pc_rid = i;
		}
	}
	if(pc_rid > 0 && s_rid > 0 && pc_rid == s_rid)
		*in = TRUE;
}

void gen_move_sprite(Dungeon * dungeon, int sn) {
	//make ss[sn] when possible
	// int sx = dungeon->ss[sn].p.x;
	// int sy = dungeon->ss[sn].p.y;
	int sx = getSpriteAPX(dungeon->ss, sn);
	int sy = getSpriteAPY(dungeon->ss, sn);
	int xs[8] = {-1,0,1,1,1,0,-1,-1};
	int ys[8] = {-1,-1,-1,0,1,1,1,0};

	//Sprite *s = &(dungeon->ss[sn]);
	Sprite *s = thisASprite(dungeon->ss, sn);

	/* increment the turn */
	//dungeon->ss[sn].t += (100 / s->s.s);
	setSpriteAT(dungeon->ss, sn, getSpriteAT(dungeon->ss, sn) + (100 / getSpriteSS(s)));

	Position * neu = new Position;
	neu = initPos();
	setPosX(neu, -1);
	setPosY(neu, -1);

	dungeon->d[getSpritePY(s)][getSpritePX(s)].h -= 85;
	if(dungeon->d[getSpritePY(s)][getSpritePX(s)].h < 0)
		dungeon->d[getSpritePY(s)][getSpritePX(s)].h = 0;

	// make sure we're alive
	if(s->a == TRUE) {
		Bool in_room;
		int i;
		int j;
		int eb = rand() % 2; /* we have a 50% chance to behave erratically */
		for(i = 0; i < 8; i++) {
			int px = sx + xs[i];
			int py = sy + ys[i];

			if(px >= 0 && px < dungeon->w && py >= 0 && py < dungeon->h) {
				/* drunken PC movement as per assignment 1.04 */

				/* check erratic behaviour */
				if(getSpriteSEb(s) == FALSE || (getSpriteSEb(s) == TRUE && eb)) {
					/** check if intelligent / telepathic **/
					//neu.x = sx;
					//neu.y = sy;
					if(getSpriteSTe(s) == FALSE) {
						/* see if you're in the same room */
						in_room = FALSE;
						with_pc(dungeon, s, &in_room);
						if(in_room == TRUE) {
							//cache PC location
							//s->pc = dungeon->ss[dungeon->pc].p;
							setSpritePcX(s, getSpriteAPX(dungeon->ss, dungeon->pc));
							setSpritePcY(s, getSpriteAPY(dungeon->ss, dungeon->pc));

							IN: ;
							in_room = TRUE;
							if(getSpriteSIn(s) == TRUE) {
								/* we are intelligent and can see our mark (tele or otherwise) */
								int k;
								int lowest = 0;
								Bool set = FALSE;
								if(getSpriteSTu(s)) {
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
									setPosX(neu, xs[lowest] + sx);
									setPosY(neu, ys[lowest] + sy);
									break;
								} else {
									setPosX(neu, sx);
									setPosY(neu, sy);
									break;
								}

									/*
									if(test_loc(dungeon, px, py, s) == TRUE) {
										//if we can move to the point
										if(getSpriteSTu(s)) {
											//tunneling
											if(neu.x > 0 && neu.y > 0 && neu.x != sx && neu.y != sy) {
												if(dungeon->cst[py][px] < dungeon->cst[neu.y][neu.x]) {
													neu.x = px;
													neu.y = py;
												}
											} else {
												neu.x = px;
												neu.y = py;
											}
										} else {
											//non-tunneling
											if(neu.x > 0 && neu.y > 0 && neu.x != sx && neu.y != sy) {
												if(dungeon->csnt[py][px] < dungeon->csnt[neu.y][neu.x]) {
													neu.x = px;
													neu.y = py;
												}
											} else {
												neu.x = px;
												neu.y = py;
											}
										}
									}
									*/
							} else {
								//if not intelligent
								if(getSpritePcX(s) < sx)
									px = sx - 1;
								if(getSpritePcX(s) > sx)
									px = sx + 1;
								if(getSpritePcY(s) < sy)
									py = sy - 1;
								if(getSpritePcY(s) > sy)
									py = sy + 1;

								if(test_loc(dungeon, px, py, s) == TRUE) {
									setPosX(neu, px);
									setPosY(neu, py);

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
						//s->pc = dungeon->ss[dungeon->pc].p;
						setSpritePcX(s, getSpriteAPX(dungeon->ss, dungeon->pc));
						setSpritePcY(s, getSpriteAPY(dungeon->ss, dungeon->pc));
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
						setPosX(neu, px);
						setPosY(neu, py);
					}

					break;
				}
			}
		}
	}

	/* safety net */
	if(getPosX(neu) < 0)
		setPosX(neu, sx);
	if(getPosY(neu) < 0)
		setPosY(neu, sy);

	//dungeon->ss[sn].to.x = neu.x;
	//dungeon->ss[sn].to.y = neu.y;
	setSpriteAToX(dungeon->ss, sn, getPosX(neu));
	setSpriteAToY(dungeon->ss, sn, getPosY(neu));

	/* combat sequence */
	
}

/* parse and apply a movement */
void parse_move(Dungeon * dungeon, int sn) {
	//dungeon->ss[sn].p.x = dungeon->ss[sn].to.x;
	//dungeon->ss[sn].p.y = dungeon->ss[sn].to.y;
	setSpriteAPX(dungeon->ss, sn, getSpriteAToX(dungeon->ss, sn));
	setSpriteAPY(dungeon->ss, sn, getSpriteAToY(dungeon->ss, sn));
}

/* add a sprite to the dungeon */
void add_sprite(Dungeon * dungeon, Sprite * s) {
	if(dungeon->ns < dungeon->ms) {
		dungeon->ns++;
	} else {
		goto END;
	}

	if(getSpriteC(s) == '@') {
		dungeon->pc = dungeon->ns - 1;
		s->color = WHITE;
    }

	//dungeon->ss[dungeon->ns - 1] = s;
	s->sn = dungeon->ns -1;
	copyASprite(dungeon->ss, dungeon->ns -1, s);

	END: ;
}

/* generate a sprite from the factory */
Sprite * gen_sprite_fac(Dungeon * dungeon, char c, int x, int y, int r) {
	Sprite* s = new Sprite;
	
	if(c != '@'){
		s = dungeon->mf->GetMon();
		
		//printf("SIMBUL AFTER FAC: %c\n", s->c);
		
		/* place in a room if 1 or more. implicitly random ;; force in a room even if tunneling */
		if(r > 0 || s->s.tu == true) {
			int t = 0;
			PRNTT: ;
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
					goto PRNTT;
				}
			}
		} else {
			if(x < 0 || x > dungeon->w) {
				x = (rand() % (dungeon->w-2)) + 1;
        	}
			if(y < 0 || y > dungeon->h) {
				y = (rand() % (dungeon->h-2)) + 1;
			}
		}
	} else {
		//pc
		s = initPC(dungeon);
	}
		
	return s;
}

/* checks if any monsters other than the player are alive */
Bool check_any_monsters(Dungeon * dungeon) {
	int i;
	for(i = 0; i < dungeon->ns; i++) {
		if(dungeon->ss[i].a == TRUE && i != 0)
			return TRUE;
	}

	return FALSE;
}
