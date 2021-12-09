#ifndef SIMPLE_PAIR_H_
#define SIMPLE_PAIR_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct simple_pair
{
    void *first;
    void *second;

    void (*first_free_fun)(void *);
    void (*second_free_fun)(void *);
};

struct simple_pair *new_simple_pair(void *first, void *second, void (*first_free_fun)(void *), void (*second_free_fun)(void *));
void free_simple_pair(struct simple_pair *pair);

#endif