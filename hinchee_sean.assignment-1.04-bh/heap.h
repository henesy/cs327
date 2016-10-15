#ifndef HEAP_H
#define HEAP_H

#define HEAP_BUFFER_SIZE (1<<10)

typedef struct
{
	void **heap;
	int (*comp)(const void*, const void*);
	int size;
} Heap;

/* init heap with a compare method */
int heap_init(Heap*, int (*compare)(const void*, const void*));

/* delete heap */
int heap_delete(Heap*);

/* insert into heap */
int heap_insert(Heap*, void*);

/* extract min element, NULL on failure */
void *heap_extract(Heap*);

#endif

