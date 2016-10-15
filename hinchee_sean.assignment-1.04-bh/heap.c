#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

static void swap(Heap *h, int i, int j)
{
	void *tmp = h->heap[i];
	h->heap[i] = h->heap[j];
	h->heap[j] = tmp;
}

static void percolateUp(Heap *h, int index)
{
	while (index>0)
	{
		int p = (index-1)/2;
		if (h->comp(h->heap[p], h->heap[index]) <= 0)
			break;
		else
		{
			swap(h, p, index);
			index = p;
		}
	}
}

static void percolateDown(Heap *h, int index)
{
	while (index<h->size/2)
	{
		int l = index*2+1;
		int r = index*2+2;
		int imin = index;
		if (l < h->size && h->comp(h->heap[l], h->heap[imin]) < 0)
			imin = l;
		if (r < h->size && h->comp(h->heap[r], h->heap[imin]) < 0)
			imin = r;
		if (h->comp(h->heap[index], h->heap[imin]) == 0)
			break;
		else
		{
			swap(h, index, imin);
			index = imin;
		}
	}
}

int heap_init(Heap *h, int (*compare)(const void*, const void*))
{
	h->comp = compare;
	h->heap = malloc(sizeof(void*) * HEAP_BUFFER_SIZE);
	h->size = 0;
	return 0;
}

int heap_delete(Heap *h)
{
	free(h->heap);
	return 0;
}

int heap_insert(Heap *h, void *e)
{
	if (h->size >= HEAP_BUFFER_SIZE)
	{
		fprintf(stderr, "Error: heap_insert: buffer overflow\n");
		return 1;
	}
	h->heap[h->size] = e;
	percolateUp(h, h->size);
	h->size++;
	return 0;
}

void *heap_extract(Heap *h)
{
	if (h->size <= 0)
	{
		return NULL;
	}
	void *tmp = h->heap[0];
	swap(h, --h->size, 0);
	percolateDown(h, 0);
	return tmp;
}

