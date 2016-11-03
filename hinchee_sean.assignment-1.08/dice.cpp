#include "dungeon_generator.h"
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <sstream>

/* 
 * found online @ http://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c#5591169
 * as a fix for gcc wanting c++11 for std::to_string() and such, dirty hack is dirty
 */
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


int	Dice::roll(void) {
	if(d == 0)
		return b;
	else
		return b + n * ((rand() % d) + 1);
}

std::string Dice::string(void) {
	return SSTR(b) + "+" + SSTR(n) + "d" + SSTR(d);
}

Dice::Dice(int base, int num, int die) {
	/* format to resemble: rolling 2+1d6 (2 + 1*[1→6]) ∴ (b + n*[1→d]) */
	b = base;
	n = num;
	d = die;
}


