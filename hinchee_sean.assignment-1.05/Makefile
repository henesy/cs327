all: main

main: main.c binheap.c monsters.c world.c printing.c
	gcc -Wall -Werror -ggdb main.c binheap.c monsters.c world.c printing.c -lm -lncurses -o dungeon_generator

clean: dungeon_generator
	rm dungeon_generator
