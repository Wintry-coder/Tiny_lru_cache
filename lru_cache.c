#include "lru_cache.h"
struct lruCache *new_lru_cache(int size, void (*free_elem)(void *),
                               void (*key_free_fun)(void *),
                               void (*value_free_fun)(void *),
                               guint (*hashfun)(void *key),
                               gboolean (*equalfun)(void *key1, void *key2))
{
    struct lruCache *c = (struct lruCache *)malloc(sizeof(struct lruCache));

    c->elem_queue = NULL;
    c->map = g_hash_table_new_full(hashfun, equalfun, NULL, NULL);

    c->max_size = size;
    c->size = 0;
    c->hit_count = 0;
    c->miss_count = 0;

    c->free_elem = free_elem; //free_simple_pair
    c->key_free_elem = key_free_fun;
    c->value_free_elem = value_free_fun;

    return c;
}

void free_lru_cache(struct lruCache *c)
{
    g_list_free_full(c->elem_queue, c->free_elem);
    g_hash_table_destroy(c->map);
    free(c);
}

void lru_cache_insert(struct lruCache *c, void *key, void *value)
{
    if (g_hash_table_lookup(c->map, key) == NULL)
    {
        if (c->max_size > 0 && c->size == c->max_size)
        {
            GList *last = g_list_last(c->elem_queue);
            struct simple_pair *victim_pair = last->data;
            g_hash_table_remove(c->map, victim_pair->first);
            c->elem_queue = g_list_remove_link(c->elem_queue, last);
            g_list_free_1(last);
            c->size--;
            if (c->free_elem)
                c->free_elem(victim_pair);
        }
    }
    else
    {
        GList *victim = g_hash_table_lookup(c->map, key);
        struct simple_pair *victim_pair = victim->data;
        c->elem_queue = g_list_remove_link(c->elem_queue, victim);
        g_list_free_1(victim);
        c->size--;

        if (c->free_elem)
            c->free_elem(victim_pair);
    }
    struct simple_pair *pair = new_simple_pair(key, value, c->key_free_elem, c->value_free_elem);

    c->elem_queue = g_list_prepend(c->elem_queue, pair);
    g_hash_table_replace(c->map, pair->first, g_list_first(c->elem_queue));
    c->size++;
}

void *lru_cache_lookup(struct lruCache *c, void *key)
{
    if (g_hash_table_lookup(c->map, key) == NULL)
    {
        c->miss_count++;
        return NULL;
    }

    GList *elem = g_hash_table_lookup(c->map, key);
    struct simple_pair *pair = elem->data;
    c->elem_queue = g_list_remove_link(c->elem_queue, elem);
    c->elem_queue = g_list_concat(elem, c->elem_queue);
    g_hash_table_replace(c->map, pair->first, g_list_first(c->elem_queue));
    c->hit_count++;
    return pair->second;
}