#ifndef SAFE_LRU_CACHE_H_
#define SAFE_LRU_CACHE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "simple_pair.h"
struct lruCache
{
    GList *elem_queue;
    GHashTable *map;
    int max_size; // less then zero means infinite cache
    int size;

    double hit_count;
    double miss_count;

    void (*free_elem)(void *);
    void (*key_free_elem)(void *);
    void (*value_free_elem)(void *);

    int (*hit_elem)(void *elem, void *user_data);
};

struct lruCache *new_lru_cache(int size, void (*free_elem)(void *),
                               void (*key_free_fun)(void *),
                               void (*value_free_fun)(void *),
                               guint (*hashfun)(void *key),
                               gboolean (*equalfun)(void *key1, void *key2));

void free_lru_cache(struct lruCache *);
void *lru_cache_lookup(struct lruCache *, void *key);
void lru_cache_insert(struct lruCache *c, void *key, void *value);

#endif