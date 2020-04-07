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
    // to replace the del node with a leaf node
    struct myrb_node *del = node, *p;
    while (del->rc || del->lc)
    {
        // if the successor exists,then search the successor of del node
        if (del->rc)
        {
            p = del->rc;
            while (p->lc)
                p = p->lc;
        }
        // search the predecessor of del node
        else
        {
            p = del->lc;
            while (p->rc)
                p = p->rc;
        }

        //replace del node by the successor or predecessor
        del->key = p->key;
        del->sval = p->sval;
        del = p;
    }

    //del the node and fix up the rb tree
    myrb_node_delete_fix_up(T, del);
}

void myrb_node_delete_fix_up(myrb_tree *T, struct myrb_node *del)
{

    //case 1:the del node is red,delete it directly
    if (del->col == RED)
    {
        if (del->par == NULL) // del node is the root
            (*T) = NULL;
        else if (del->par->lc == del)
            del->par->lc = NULL;
        else
            del->par->rc = NULL;
        free(del);
        return;
    }

    // case 2:the del node is black
    struct myrb_node *par, *bro;
    par = del->par;

    // case 2.1:del is the lc of its parent
    if (del == par->lc)
    {
        bro = par->rc;
        //case 2.1.1: change 2.1.1 into 2.1.2.3
        if (bro->col == RED)
        {
            bro->col = BLACK;
            par->col = RED;
            myrb_left_rotate(T, par);
            myrb_node_delete_fix_up(T, del);
            return;
        }
        //case 2.1.2: bro color is black

        //case 2.1.2.1
        if (bro->rc && bro->rc->col == RED)
        {
            bro->col = par->col;
            par->col = BLACK;
            bro->rc->col = BLACK;
            myrb_left_rotate(T, par);
            del->col = RED;
            myrb_node_delete_fix_up(T, del);
            return;
        }

        // case 2.1.2.2 :change into 2.1.2.1
        if ((bro->lc && bro->lc->col == RED) && (!bro->rc || bro->rc->col == BLACK))
        {
            bro->lc->col = BLACK;
            bro->col = RED;
            myrb_right_rotate(T, bro);
            myrb_node_delete_fix_up(T, del);
            return;
        }

        // case 2.1.2.3:
        bro->col=RED;

    }
    // case 2.2:del is the rc of its parent
    else
    {
        bro = par->lc;
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