#ifndef dungeon_generator
#define dungeon_generator
	#include <vector>
	#include <string>

	/*** utility enums ***/
	typedef enum {RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, BLACK} colour;
	/* itype specifically can specify equipment t/f when itype it is ≥ NONEQUIP for false and < for true (is equipment), this is kind of a hack */
	typedef enum {WEAPON, OFFHAND, RANGED, ARMOR, HELMET, CLOAK, GLOVES, BOOTS, RING, AMULET, LIGHT, NONEQUIP, SCROLL, BOOK, FLASK, GOLD, AMMUNITION, FOOD, WAND, CONTAINER} itype;

	/*** c++ classes ***/
	class PC;
	
	class Dice {
		int	b;	/* base number */
		int	d;	/* die value (d6, d10, d20, etc.) */
		int	n;	/* number of dice to roll */
	public:
		int	roll(void);
		Dice(int, int, int);
		std::string string(void);
	};
	
	class Item {
	public:
		std::string	n;			/* name/title */
		std::string*	desc;	/* description, limited to 77 characters wide */
		itype	t;			/* weapon/item type */
		bool	e;			/* is equipment (assignment1.07.pdf:6) */
		colour	c;			/* color of item */
		Dice*	d;			/* damage */
		int		hib;		/* bonus to hit (applied to probability) */
		int		dob;		/* bonus to dodge (applied to probability) */
		int		deb;		/* bonus to defense (reduces damage) */
		int		w;			/* weight of item */
		int		spb;		/* bonus to speed (movement) */
		int		sa;			/* special attribute */
		int		v;			/* value in Pesos de Ocho */
	};

	class Memory {
	public:
		char c; /* character */
		bool v; /* visited */
	};

	class Position {
	public:
		int	x; /* x coordinate */
		int	y; /* y coordinate */
	};

	class Stats {
	public:
		bool    in; /* intelligence */
		bool    te; /* telepathy */
		bool    tu; /* tunneling ability */
		bool    eb; /* erratic behaviour */
		int     s;  /* speed ;; pc has 10 ; 5-20 */

		bool	pa;	/* pass-through, can move through anything */
		Dice*	a;	/* attack damage */
		int		hp;	/* health points */
	};

	class Sprite {
	public:
		friend class PC;
		friend class Monster;
		Position	p;	/* position of the sprite in the dungeon */
		char		c;	/* character to print for the sprite */
		Stats       s;	/* stats for a sprite */
		int			t;	/* turn count */
		Position	to;	/* to move to */
		int			sn;	/* sprite number */
		Position	pc;	/* last known location of the PC */
		bool		a;	/* alive T/F */
		
		std::string	n; /* name */
		colour	color;	/* color */
		std::string*	desc;	/* sprite description, 77 character string, newline at or before byte 78 */
		
		/* methods */
		Sprite * 	thisSprite(void); /* returns a pointer to the given sprite */
		PC	* 		thisPC(void);
	};

	class Monster : public Sprite {
	public:
		friend class Sprite;

	};

	class PC : public Sprite {
	public:
		friend class Sprite;
		int	view;	/* visible viewing distance */
		Memory	**	mem;	/* map/dungeon view for the PC (memory) */
	};


	/* C stuff for C++ */
	/* set up Booleans */
	#define	TRUE	1
	#define	FALSE	0
	typedef int Bool;

	/* custom structures */
	typedef struct {
		int	h;	/* hardness */
		char	c;	/* visual character */
		int	p;	/* mark 1 if path, 0 if not a path (corridors) */
	} Tile;

	/* maybe make these pointers? */
	typedef struct {
		int prev; /* previous room in the path (using Room.id) */
		int next; /* room the path leads to (using Room.id) */
	} Path;

	typedef struct {
		Position*	tl;	/* top left coordinate of the room, used as the core point of reference */
		Position*	br;	/* bottom right coordinate of the room as per above */
		int		w;	/* width */
		int		h;	/* height */
		int		id;	/* room ID, potentially useful for organization */
		int		p;	/* mark 1 if processed; 0 if not processed (corridors) */
		Position*	ctr;	/* "center" point; very rough, might need improved */
		int		c;	/* if connected or not; TRUE/FALSE switch */
	} Room;

	typedef struct {
		Tile **		d;		/* dungeon buffer */
		Tile **		p;		/* print buffer */
		int			h;		/* height */
		int			w;		/* width */
		int			nr;		/* number of rooms */
		int			mr;		/* max rooms */
		Room *		r;		/* rooms buffer */
		int			v;		/* file version */
		int			s;		/* file size */
		Sprite *	ss;		/* sprite array */
		int			ns;		/* number of sprites */
		int			ms;		/* max number of sprites */
		int **		csnt;
		int	**		cst;	/* costs for djikstra's map */
		int			pc;		/* location of PC in SpriteS array (.ss) */
	    int         t;		/* turn number */
		Bool		go;		/* game over T/F */
		Position *	su;		/* location of the dungeon's up staircase */
		Position *	sd;		/* location of the dungeon's down staircase */
		PC *		plyr;	/* pointer to player character un-restricted by Sprite */
		
		int			mi;		/* max items/objects that can be defined */
		int			di;		/* number of defined items */
		Item *		id;		/* item definitions */
		int			mm;		/* max monsters that can be defined */
		int			dm;		/* number of definted monsters*/
		Monster *	md;		/* monster definitions */
	} Dungeon;

	typedef struct {
		int x;
		int y;
		int cost;
		int v;
	} Tile_Node;

	/*** Function prototypes ***/
	
	/* monsters.c */
	void add_sprite(Dungeon * dungeon, Sprite * s);
	Sprite * gen_sprite(Dungeon * dungeon, char c, int x, int y, int r);
	void gen_move_sprite(Dungeon * dungeon, int sn);
	void parse_move(Dungeon * dungeon, int sn);
	Bool check_any_monsters(Dungeon * dungeon);
	Bool test_loc(Dungeon * dungeon, int x, int y, Sprite *s);
	void with_pc(Dungeon * dungeon, Sprite * s, Bool *in);

	/* world.c */
	void gen_corridors(Dungeon * dungeon);
	void gen_dungeon(Dungeon * dungeon);
	Dungeon init_dungeon(int h, int w, int mr);

	/* printing.c */
	void print_hardness(Dungeon * dungeon);
	void print_t_heatmap(Dungeon * dungeon);
	void print_nont_heatmap(Dungeon * dungeon);
	void print_dungeon(Dungeon * dungeon, int nt, int t);
	void print_dungeon_nnc(Dungeon * dungeon, int nt, int t);

	/* main.c */
	void map_dungeon_t(Dungeon * dungeon);

	/****
	functions to make C++ and C play nicely together on their little hellish playground
	these are all defined in a *.cpp
	****/

	/***
	setter functions
	***/

	Sprite * initSprite();
	Sprite * initSprites(int);

	/***
	copy functions
	***/
	void copySprite(Sprite *, Sprite *);
	void copyASprite(Sprite * to, int n, Sprite * from);
	Sprite * thisASprite(Sprite * arr, int i);

	/***
	getter
	***/
	/* singleton */
	int getSpritePX(Sprite *);
	int getSpritePY(Sprite *);
	char getSpriteC(Sprite *);
	bool getSpriteSIn(Sprite *);
	bool getSpriteSTe(Sprite *);
	bool getSpriteSTu(Sprite *);
	bool getSpriteSEb(Sprite *);
	int getSpriteSS(Sprite *);
	int getSpriteT(Sprite *);
	int getSpriteToX(Sprite *);
	int getSpriteToY(Sprite *);
	int getSpriteSn(Sprite *);
	int getSpritePcX(Sprite *);
	int getSpritePcY(Sprite *);
	bool getSpriteA(Sprite *);
	/* array-based */
	int getSpriteAPX(Sprite *, int);
	int getSpriteAPY(Sprite *, int);
	char getSpriteAC(Sprite *, int);
	bool getSpriteASIn(Sprite *, int);
	bool getSpriteASTe(Sprite *, int);
	bool getSpriteASTu(Sprite *, int);
	bool getSpriteASEb(Sprite *, int);
	int getSpriteASS(Sprite *, int);
	int getSpriteAT(Sprite *, int);
	int getSpriteAToX(Sprite *, int);
	int getSpriteAToY(Sprite *, int);
	int getSpriteASn(Sprite *, int);
	int getSpriteAPcX(Sprite *, int);
	int getSpriteAPcY(Sprite *, int);
	bool getSpriteAA(Sprite *, int);
	/***
	Setter functions
	kill me
	***/
	/* singleton */
	void setSpritePX(Sprite *, int);
	void setSpritePY(Sprite *, int);
	void setSpriteC(Sprite *, char);
	void setSpriteSIn(Sprite *, bool);
	void setSpriteSTe(Sprite *, bool);
	void setSpriteSTu(Sprite *, bool);
	void setSpriteSEb(Sprite *, bool);
	void setSpriteSS(Sprite *, int);
	void setSpriteT(Sprite *, int);
	void setSpriteToX(Sprite *, int);
	void setSpriteToY(Sprite *, int);
	void setSpriteSn(Sprite *, int);
	void setSpritePcX(Sprite *, int);
	void setSpritePcY(Sprite *, int);
	void setSpriteA(Sprite *, bool);
	/* array-based */
	void setSpriteAPX(Sprite *, int, int);
	void setSpriteAPY(Sprite *, int, int);
	void setSpriteAC(Sprite *, int, char c);
	void setSpriteASIn(Sprite *, int, bool);
	void setSpriteASTe(Sprite *, int, bool);
	void setSpriteASTu(Sprite *, int, bool);
	void setSpriteASEb(Sprite *, int, bool);
	void setSpriteASS(Sprite *, int, int);
	void setSpriteAT(Sprite *, int, int);
	void setSpriteAToX(Sprite *, int, int);
	void setSpriteAToY(Sprite *, int, int);
	void setSpriteASn(Sprite *, int, int);
	void setSpriteAPcX(Sprite *, int, int);
	void setSpriteAPcY(Sprite *, int, int);
	void setSpriteAA(Sprite *, int, bool);

	/***
	PC functions
	***/
	PC * initPC(Dungeon *);

	/***
	our happy little position pointer section that made us go from 204 to 272 errors
	***/
	int getPosX(Position * p);
	int getPosY(Position * p);
	void setPosX(Position * p, int n);
	void setPosY(Position * p, int n);
	Position * initPos(void);

	/***
	utility functions from cpp-space
	***/
	void updateMemory(Dungeon *);
	char getMem(Dungeon *, int, int);
	PC * getPC(Sprite * arr);
	PC * thisAPC(Sprite * arr, int i);

#endif
