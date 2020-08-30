#ifndef LL_CYCLE_H
#define LL_CYCLE_H
typedef struct node {
    int value;
    struct node *next;
} node;

int ll_has_cycle(node *);
#endif