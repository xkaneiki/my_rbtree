#include "myrbtree.h"

void myrb_left_rotate(myrb_tree *T, struct myrb_node *node)
{
    struct myrb_node *x = node;
    struct myrb_node *y = node->rc;

    x->rc = y->lc;
    if (y->lc)
        y->lc->par = x;

    y->par = x->par;
    /*if x is the root then set the root as y*/
    if (x->par == NULL)
        (*T) = y;
    else if (x == x->par->lc) /* x is the lc of its parent*/
        x->par->lc = y;
    else if (x == x->par->rc) /* x is the rc of its parent*/
        x->par->rc = y;

    y->lc = x;
    x->par = y;

    myrb_print(*T);
    return;
}

void myrb_right_rotate(myrb_tree *T, struct myrb_node *node)
{
    struct myrb_node *x = node;
    struct myrb_node *y = node->lc;

    x->lc = y->rc;
    if (y->rc)
        y->rc->par = x;

    y->par = x->par;
    if (x->par == NULL)
        (*T) = y;
    else if (x == x->par->lc)
        x->par->lc = y;
    else if (x == x->par->rc)
        x->par->rc = y;

    y->rc = x;
    x->par = y;

    myrb_print(*T);
    return;
}

/*insert operation*/
void myrb_insert(myrb_tree *T, KElemType key)
{
    struct myrb_node *node = (struct myrb_node *)malloc(sizeof(struct myrb_node));
    node->key = key;
    node->lc = node->rc = node->par = NULL;

    myrb_node_insert(T, node);
}

void myrb_node_insert(myrb_tree *T, struct myrb_node *node)
{
    // case 1: node is root
    // if the rb tree is empty, then let this node to be the root node
    if ((*T) == NULL)
    {
        node->col = BLACK;
        (*T) = node;
        myrb_print(*T);
        return;
    }

    // find the pos of this node in the tree
    struct myrb_node *p, *par;
    p = *T;
    par = NULL;
    while (p != NULL)
    {
        par = p;
        if (node->key < p->key)
        {
            p = p->lc;
        }
        else if (node->key > p->key)
        {
            p = p->rc;
        }
        else // p->key==node->key,indicate the node has existed
        {
            free(node); //free the space of node
            return;
        }
    }
    // insert node into the tree
    node->par = par;
    if (node->key < par->key) //node is the lc of par
        par->lc = node;
    else //node is the rc if par
        par->rc = node;

    node->col = RED; //let node be red
    myrb_print(*T);

    // case 2: par of node is black
    if (par->col == BLACK)
    {
        myrb_print(*T);
        return; //T has been a rb tree
    }

    // case 3: par of node is red
    myrb_node_insert_fix_up(T, node);
}

void myrb_node_insert_fix_up(myrb_tree *T, struct myrb_node *node)
{
    struct myrb_node *par = node->par;
    // case 1
    if (par == NULL)
    {
        node->col = BLACK;
        myrb_print(*T);
        return;
    }

    // case 2:
    if (par->col == BLACK)
    {
        myrb_print(*T);
        return;
    }

    // case 3:
    struct myrb_node *gp, *uncle;
    gp = par->par;
    if (par == gp->lc)
        uncle = gp->rc;
    else
        uncle = gp->lc;
    // case 3.1
    // par node and uncle node are both red
    if (uncle && uncle->col == RED)
    {
        uncle->col = BLACK;
        par->col = BLACK;
        gp->col = RED;
        myrb_print(*T);
        myrb_node_insert_fix_up(T, gp);
        return;
    }

    // case 3.2
    if ((!uncle || uncle->col == BLACK) && (gp->lc == par))
    {
        if (par->rc == node)
        {
            myrb_left_rotate(T, par);
            node->col = BLACK;
        }
        else
            par->col = BLACK;
        gp->col = RED;
        myrb_right_rotate(T, gp);
        return;
    }

    //case 3.3
    if ((!uncle || uncle->col == BLACK) && (gp->rc == par))
    {
        if (node == par->lc)
        {
            myrb_right_rotate(T, par);
            node->col = BLACK;
        }
        else
            par->col = BLACK;
        gp->col = RED;
        myrb_left_rotate(T, gp);
        return;
    }

    return;
}

/*delete operation*/
void myrb_delete(myrb_tree *T, KElemType key)
{
    struct myrb_node *node;
    node = myrb_search(*T, key);

    if (node == NULL)
        return;

    myrb_node_delete(T, node);
}

void myrb_node_delete(myrb_tree *T, struct myrb_node *node)
{
    // delete node as T is a binary tree
    struct myrb_node *del = node, *p, *rp;
    // if del has both child, then search for the successor
    if (del->lc && del->rc)
    {
        p = del->rc;
        while (p->lc)
            p = p->lc;

        del->key = p->key;
        del->sval = p->sval;
        del = p;
    }

    rp = NULL;
    if (del->lc)
        rp = del->lc;
    else
        rp = del->rc;
    // del has only one child
    if (rp)
        rp->par = del->par;
    if (del->par == NULL) // del is the root then set rp as the root;
        (*T) = rp;
    else if (del == del->par->lc)
    {
        del->par->lc = rp;
    }
    else if (del == del->par->rc)
    {
        del->par->rc = rp;
    }

    myrb_print(*T);
    //del the node and fix up the rb tree
    struct myrb_node *par = del->par;
    if (del->col == BLACK)
        myrb_node_delete_fix_up(T, rp, par);

    // free the space of the del node
    free(del);
    myrb_print(*T);
}

void myrb_node_delete_fix_up(myrb_tree *T, struct myrb_node *node, struct myrb_node *par)
{
    // now node has 2 colors, back and node->col

    // case 1:node is the root,then let node be black
    if ((*T) == node)
    {
        if (node)
            node->col = BLACK;
        myrb_print(*T);
        return;
    }

    // case 2:node is not root
    // case 2.1:node is red with black
    if (node && node->col == RED)
    {
        node->col = BLACK;
        myrb_print(*T);
        return;
    }

    // case 2.2:node is black with black
    struct myrb_node *bro = NULL;

    if (node)
    {
        par = node->par;
    }

    if (node == par->lc) //node is lc
    {
        bro = par->rc;
        //case 2.2.1:bro is red then change it to black
        if (bro && bro->col == RED)
        {
            bro->col = BLACK;
            par->col = RED;
            myrb_left_rotate(T, par);
            //reset bro
            bro = par->rc;

            myrb_print(*T);
        }

        //bro is black
        //case 2.2.2:both childs of bro are black
        if ((!bro->lc || bro->lc == BLACK) && (!bro->rc || bro->rc == BLACK))
        {
            bro->col = RED;

            myrb_print(*T);

            myrb_node_delete_fix_up(T, par, NULL); //set par as the new node to fix up
            return;
        }
        //case 2.2.3:lc of bro are red,then change into case 2.2.4
        if (bro->lc && bro->lc->col == RED)
        {
            bro->col = RED;
            bro->lc->col = BLACK;
            myrb_right_rotate(T, bro);
            // reset bro
            bro = par->rc;

            myrb_print(*T);
        }
        // case 2.2.4:rc of bro are red
        if (bro->rc && bro->rc->col == RED)
        {
            bro->col = par->col;
            par->col = BLACK;
            bro->rc->col = BLACK;
            myrb_left_rotate(T, par);
            myrb_print(*T);
            // myrb_node_delete_fix_up(T, bro, NULL); //set the pra as the new node to fix up
            return;
        }
    }
    else //node is rc
    {
        bro = par->lc;
        //case 2.2.1:bro is red then change it to black
        if (bro && bro->col == RED)
        {
            bro->col = BLACK;
            par->col = RED;
            myrb_right_rotate(T, par);
            //reset bro
            bro = par->lc;

            myrb_print(*T);
        }

        //bro is black
        //case 2.2.2:both childs of bro are black
        if ((!bro->lc || bro->lc->col == BLACK) && (!bro->rc || bro->rc->col == BLACK))
        {
            bro->col = RED;
            myrb_print(*T);
            myrb_node_delete_fix_up(T, par, NULL); //set par as the new node to fix up
            return;
        }

        //case 2.2.3:rc of bro are red,then change into case 2.2.4
        if (bro->rc && bro->rc->col == RED)
        {
            bro->col = RED;
            bro->rc->col = BLACK;
            myrb_left_rotate(T, bro);
            // reset bro
            bro = par->lc;
            myrb_print(*T);
        }

        // case 2.2.4:lc of bro are red
        if (bro->lc && bro->lc->col == RED)
        {
            bro->col = par->col;
            par->col = BLACK;
            bro->lc->col = BLACK;
            myrb_right_rotate(T, par);
            myrb_print(*T);
            // myrb_node_delete_fix_up(T, bro, NULL); //set the pra as the new node to fix up
            return;
        }
    }
}

struct myrb_node *myrb_search(myrb_tree T, KElemType key)
{
    struct myrb_node *p = T;
    while (p)
    {
        if (key < p->key)
            p = p->lc;
        else if (key > p->key)
            p = p->rc;
        else
            return p;
    }
    return NULL;
}

void myrb_print(myrb_tree T)
{
    if (T == NULL)
        return;

    FILE *fp = fopen("tree.dot", "w");
    fprintf(fp, "digraph Tree{\n");
    ;
    // int depth=myrb_depth(T);
    //set a queue
    int qs = 0, qe = 0;
    struct myrb_node **que;
    que = (struct myrb_node **)malloc(10000 * sizeof(struct myrb_node *));

    que[qe++] = T;
    struct myrb_node *p;
    while (qe > qs) //que is not empty
    {
        p = que[qs++];
        if (p->col == RED)
        {
            fprintf(fp, "\t%u [style=filled color=red] ;\n", p->key);
        }
        if (p->lc)
        {
            fprintf(fp, "\t%u -> %u ;\n", p->key, p->lc->key);
            que[qe++] = p->lc;
        }
        if (p->rc)
        {
            fprintf(fp, "\t%u -> %u ;\n", p->key, p->rc->key);
            que[qe++] = p->rc;
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
    printf("draw finish!\n");
    // getchar();
}

int myrb_depth(myrb_tree T)
{
    if (T == NULL)
        return 0;
    int l = myrb_depth(T->lc), r = myrb_depth(T->rc);
    if (l > r)
        return l + 1;
    return r + 1;
}

void myrb_free(myrb_tree T)
{
    if (T)
    {
        myrb_free(T->lc);
        myrb_free(T->rc);
        free(T);
    }
    return;
}