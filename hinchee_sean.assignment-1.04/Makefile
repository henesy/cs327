all: main

main: main.c monsters.c
	gcc -Wall -Werror -ggdb main.c binheap.c monsters.c -lm -o dungeon_generator

clean: dungeon_generator
	rm dungeon_generator

