#include "simple_pair.h"
struct simple_pair *new_simple_pair(void *first, void *second, void (*first_free_fun)(void *), void (*second_free_fun)(void *))
{
    struct simple_pair *pair = (struct simple_pair *)malloc(sizeof(struct simple_pair));
    pair->first = first;
    pair->second = second;
    pair->first_free_fun = first_free_fun;
    pair->second_free_fun = second_free_fun;
    return pair;
}

void free_simple_pair(struct simple_pair *pair)
{
    if (pair->first_free_fun)
    {
        pair->first_free_fun(pair->first);
        // printf("Free key!\n");
    }

    if (pair->second_free_fun)
    {
        pair->second_free_fun(pair->second);
        // printf("Free value!\n");
    }

    free(pair);
    // printf("Free pair successfully!\n");
}