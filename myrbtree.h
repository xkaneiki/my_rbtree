#ifndef MYRBTREE_H
#define MYRBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RED 1
#define BLACK 0
#define COLOR int
#define KElemType unsigned int
#define VElemType int

struct myrb_node
{
    KElemType key;             //the value
    VElemType sval;            //the elem that the node contained
    COLOR col;                 //the color of the node
    struct myrb_node *par;     //the parent of the node
    struct myrb_node *lc, *rc; //the left child and the right child
};

typedef struct myrb_node *myrb_tree;

/*baisc operations*/
void myrb_left_rotate(myrb_tree *T, struct myrb_node *node);

void myrb_right_rotate(myrb_tree *T, struct myrb_node *node);

int myrb_depth(myrb_tree T);

/*insert functions*/
void myrb_insert(myrb_tree *T, KElemType key);

void myrb_node_insert(myrb_tree *T, struct myrb_node *node);

void myrb_node_insert_fix_up(myrb_tree *T, struct myrb_node *node);

/*delete functions*/
void myrb_delete(myrb_tree *T, KElemType key);

void myrb_node_delete(myrb_tree *T, struct myrb_node *node);

void myrb_node_delete_fix_up(myrb_tree *T, struct myrb_node *node, struct myrb_node *par);

/*search function*/
struct myrb_node *myrb_search(myrb_tree T, KElemType key);

/*print function*/
void myrb_print(myrb_tree T);

/*free space function*/
void myrb_free(myrb_tree T);

#endif // !MYRBTREE_H