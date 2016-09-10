#ifndef BINHEAP_H
# define BINHEAP_H

# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>

struct binheap_hode;
typedef struct binheap_node binheap_node_t;

typedef struct binheap {
  binheap_node_t **array;
  uint32_t size;
  uint32_t array_size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} binheap_t;

void binheap_init(binheap_t *h,
                  int32_t (*compare)(const void *key, const void *with),
                  void (*datum_delete)(void *));
void binheap_init_from_array(binheap_t *h,
                             void *array,
                             uint32_t size,
                             uint32_t nmemb,
                             int32_t (*compare)(const void *key,
                                                const void *with),
                             void (*datum_delete)(void *));
void binheap_delete(binheap_t *h);
binheap_node_t *binheap_insert(binheap_t *h, void *v);
void *binheap_peek_min(binheap_t *h);
void *binheap_remove_min(binheap_t *h);
void binheap_decrease_key(binheap_t *h, binheap_node_t *n);
uint32_t binheap_is_empty(binheap_t *h);

# ifdef __cplusplus
}
# endif

#endif
