#include <stdio.h>
#include "myrbtree.h"

int main()
{
    myrb_tree T = NULL;

    myrb_insert(&T, 10);
    myrb_insert(&T, 8);
    myrb_insert(&T, 11);
    myrb_insert(&T, 6);
    myrb_insert(&T, 9);
    myrb_insert(&T, 5);
    myrb_insert(&T, 7);
    myrb_insert(&T, 4);

    // unsigned int x;
    // while (~scanf("%u", &x))
    // {
    //     myrb_insert(&T, x);
    // }
    // printf("finish!\n");
    return 0;
}