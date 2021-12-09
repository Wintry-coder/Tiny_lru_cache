#include "simple_pair.h"

struct container
{
    int32_t data_size;
    unsigned char *data;
};

void free_container(struct container *c)
{
    free(c->data);
    free(c);
}

int main()
{
    int *key = (int *)malloc(sizeof(int));
    *key = 55;
    struct container *value = (struct container *)malloc(sizeof(struct container));
    value->data_size = 100;
    value->data = calloc(1, 100);
    struct simple_pair *pair = new_simple_pair(key, value, free, free_container);
    int *k = pair->first;
    struct container *c = pair->second;
    printf("The key is %d, the value data_size is %d\n", *k, c->data_size);
    free_simple_pair(pair);
    return 0;
}