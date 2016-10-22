#ifndef STUFF_H
#define STUFF_H

#ifdef __cplusplus
/* c++ */

class Thing {
	public:
	int x;
};

extern "C" {
#else
typedef struct Thing Thing;
#endif

#if defined(__STDC__) || defined(__cplusplus)
/* both */
int getX(Thing *);
void setX(Thing *, int);
Thing * initThing();
#endif

#ifdef __cplusplus
/* c++ */
}
#endif

#endif
