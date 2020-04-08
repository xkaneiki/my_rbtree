#include <stdio.h>
#include "myrbtree.h"

int main()
{
    myrb_tree T = NULL;

    // insert
    myrb_insert(&T, 10);
    myrb_insert(&T, 8);
    myrb_insert(&T, 11);
    myrb_insert(&T, 6);
    myrb_insert(&T, 9);
    myrb_insert(&T, 5);
    myrb_insert(&T, 7);
    myrb_insert(&T, 4);

    // delete
    myrb_delete(&T, 7);
    myrb_delete(&T, 4);
    myrb_delete(&T, 8);
    myrb_delete(&T, 11);
    myrb_delete(&T, 5);
    myrb_delete(&T, 9);
    myrb_delete(&T, 10);
    myrb_delete(&T, 6);
    // unsigned int x;
    // while (~scanf("%u", &x))
    // {
    //     myrb_insert(&T, x);
    // }
    // printf("finish!\n");
    return 0;
}