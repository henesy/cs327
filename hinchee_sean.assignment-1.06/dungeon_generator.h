#ifndef dungeon_generator
#define dungeon_generator

	#ifdef __cplusplus

		/* c++ classes */
		class Position {
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
		};

		class Sprite {
		public:
			Position	p;	/* position of the sprite in the dungeon */
			char		c;	/* character to print for the sprite */
			Stats       s;	/* stats for a sprite */
			int			t;	/* turn count */
			Position	to;	/* to move to */
			int			sn;	/* sprite number */
			Position	pc;	/* last known location of the PC */
			bool		a;	/* alive T/F */
		};

		class Monster: public Sprite {
		public:

		};

		class PC: public Sprite {
			friend class Sprite;
		public:
			int		view;		/* visible viewing distance */
		};



	#else

		/* not C++ */
		typedef struct Position Position;
		typedef struct Stats Stats;
		typedef struct Sprite Sprite;
		typedef struct Monster Monster;
		typedef struct PC PC;

		/*** functions to make C++ and C play nicely together on their little hellish playground  ***/
		/***
		setter functions
		***/

		void initSprite(Sprite *);
		void initSprites(Sprite *);

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
		int setSpritePX(Sprite *, int n);
		int setSpritePY(Sprite *, int n);
		char setSpriteC(Sprite * s, char c);
		bool setSpriteSIn(Sprite *, bool b);
		bool setSpriteSTe(Sprite *, bool b);
		bool setSpriteSTu(Sprite *, bool b);
		bool setSpriteSEb(Sprite *, bool b);
		int setSpriteSS(Sprite *, int n);
		int setSpriteT(Sprite *, int n);
		int setSpriteToX(Sprite *, int n);
		int setSpriteToY(Sprite *, int n);
		int setSpriteSn(Sprite *, int n);
		int setSpritePcX(Sprite *, int n);
		int setSpritePcY(Sprite *, int n);
		bool setSpriteA(Sprite *, bool b);
		/* array-based */
		int setSpriteAPX(Sprite *, int i, int n);
		int setSpriteAPY(Sprite *, int i, int n);
		char setSpriteAC(Sprite *, int i, char c);
		bool setSpriteASIn(Sprite *, int i, bool b);
		bool setSpriteASTe(Sprite *, int i, bool b);
		bool setSpriteASTu(Sprite *, int i, bool b);
		bool setSpriteASEb(Sprite *, int i, bool b);
		int setSpriteASS(Sprite *, int i, int n);
		int setSpriteAT(Sprite *, int i, int n);
		int setSpriteAToX(Sprite *, int i, int n);
		int setSpriteAToY(Sprite *, int i, int n);
		int setSpriteASn(Sprite *, int i, int n);
		int setSpriteAPcX(Sprite *, int i, int n);
		int setSpriteAPcY(Sprite *, int i, int n);
		bool setSpriteAA(Sprite *, int i, bool b);

	#endif

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(__STDC__) || defined(__cplusplus)
		/* C stuff for C++ */
		#include "binheap.h"
		/* set up Booleans */
		#define	TRUE	1
		#define	FALSE	0
		typedef int Bool;

		/* custom structures */
		typedef struct {
			int		h;	/* hardness */
			char	c;	/* visual character */
			int		p;	/* mark 1 if path, 0 if not a path (corridors) */
		} Tile;

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
			Tile 	**	d;		/* dungeon buffer */
			Tile 	**	p;		/* print buffer */
			int			h;		/* height */
			int			w;		/* width */
			int			nr; 	/* number of rooms */
			int			mr;		/* max rooms */
			Room 	*	r;		/* rooms buffer */
			int			v;		/* file version */
			int			s;		/* file size */
			Sprite	*	ss;		/* sprite array */
			int			ns;		/* number of sprites */
			int			ms;		/* max number of sprites */
			int		**	csnt;
			int		**	cst;	/* costs for djikstra's map */
			int			pc;		/* location of PC in SpriteS array (.ss) */
		    int         t;      /* turn number */
			Bool		go;		/* game over T/F */
			Position	su;		/* location of the dungeon's up staircase */
			Position	sd;		/* location of the dungeon's down staircase */
		} Dungeon;

		typedef struct {
			int x;
			int y;
			int cost;
			int v;
		} Tile_Node;

		typedef struct {
			int			sn;	/* sprite number */
			int 		speed;	/* speed of the Sprite */
			int			turn;	/* turn counter */
			Position	to;		/* where we move to */
		} Event;

		/*** Function prototypes ***/
		/* monsters.c */
		void add_sprite(Dungeon * dungeon, Sprite s);
		Sprite gen_sprite(Dungeon * dungeon, char c, int x, int y, int r);
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
	#endif

	#ifdef __cplusplus
	}
	#endif
#endif
