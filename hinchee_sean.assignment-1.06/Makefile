all: main

main: main.c binheap.c monsters.c world.c printing.c
	gcc -c -Wall -Werror -ggdb main.c binheap.c monsters.c world.c printing.c
	g++ -c -Wall -Werror -ggdb sprite.cpp monster.cpp pc.cpp
	g++ *.o -lm -lncurses -o dungeon_generator 

gpp: sprite.cpp monster.cpp pc.cpp
	g++ -c -Wall -Werror -ggdb sprite.cpp monster.cpp pc.cpp

clean:
	rm dungeon_generator *.o
