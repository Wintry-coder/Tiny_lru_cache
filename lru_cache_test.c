#include "lru_cache.h"
#include <assert.h>
#define CACHE_SIZE 3
#define FP_SIZE 5

typedef unsigned char fingerprint[20];

struct chunk
{
    char *fp;
    int content;
};

struct chunk *new_chunk(char *fp, int content)
{
    struct chunk *ck = (struct chunk *)malloc(sizeof(struct chunk));
    ck->fp = calloc(1, FP_SIZE);
    memcpy(ck->fp, fp, FP_SIZE);
    ck->content = content;
}

gboolean g_superfeature_equal(char *fp1, char *fp2)
{
    return memcmp(fp1, fp2, FP_SIZE) == 0 ? 1 : 0;
}

int main()
{

    struct lruCache *lru_cache = new_lru_cache(CACHE_SIZE, free_simple_pair,
                                               free, free, g_int_hash, g_superfeature_equal);

    char fp[5][FP_SIZE] = {"0000", "1111", "2222", "3333", "4444"};
    struct chunk *c = new_chunk(fp[0], 12536);
    lru_cache_insert(lru_cache, c->fp, c); // 0

    c = new_chunk(fp[1], 242455);
    lru_cache_insert(lru_cache, c->fp, c); // 1 0

    c = new_chunk(fp[2], 94467);
    lru_cache_insert(lru_cache, c->fp, c); // 2 1 0

    c = lru_cache_lookup(lru_cache, fp[0]); // 0 2 1
    printf("fp is %s and content is %d\n", c->fp, c->content);

    c = new_chunk(fp[3], 32454);
    lru_cache_insert(lru_cache, c->fp, c); // 3 0 2

    c = lru_cache_lookup(lru_cache, fp[1]);
    if (c)
    {
        printf("fp is %s and content is %d\n", c->fp, c->content);
    }
    assert(c == NULL);

    c = lru_cache_lookup(lru_cache, fp[2]); // 2 3 0
    printf("fp is %s and content is %d\n", c->fp, c->content);

    c = new_chunk(fp[4], 78454);
    lru_cache_insert(lru_cache, c->fp, c); // 4 2 3

    c = lru_cache_lookup(lru_cache, fp[0]);
    assert(c == NULL);

    free_lru_cache(lru_cache);
    return 0;
}