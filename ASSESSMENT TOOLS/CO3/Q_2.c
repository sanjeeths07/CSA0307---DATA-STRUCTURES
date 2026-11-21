#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int key, h;
    struct Node *l, *r;
} Node;

int ht(Node *n)
{
    return n ? n->h : 0;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void fix(Node *n)
{
    n->h = 1 + max(ht(n->l), ht(n->r));
}

/* LL Rotation - Right Rotation */
Node *rotate_right(Node *y)
{
    Node *x = y->l;

    y->l = x->r;
    x->r = y;

    fix(y);
    fix(x);

    return x;
}

/* RR Rotation - Left Rotation */
Node *rotate_left(Node *x)
{
    Node *y = x->r;

    x->r = y->l;
    y->l = x;

    fix(x);
    fix(y);

    return y;
}

Node *insert(Node *n, int key)
{
    if (!n)
    {
        n = malloc(sizeof *n);

        if (!n)
            return NULL;

        n->key = key;
        n->h = 1;
        n->l = n->r = NULL;

        return n;
    }

    if (key < n->key)
        n->l = insert(n->l, key);

    else if (key > n->key)
        n->r = insert(n->r, key);

    else
        return n;

    fix(n);

    int bal = ht(n->l) - ht(n->r);

    /* LL Case */
    if (bal > 1 && key < n->l->key)
        return rotate_right(n);

    /* RR Case */
    if (bal < -1 && key > n->r->key)
        return rotate_left(n);

    /* LR Case */
    if (bal > 1)
    {
        n->l = rotate_left(n->l);
        return rotate_right(n);
    }

    /* RL Case */
    if (bal < -1)
    {
        n->r = rotate_right(n->r);
        return rotate_left(n);
    }

    return n;
}

void preorder(Node *n)
{
    if (!n)
        return;

    printf("%d ", n->key);

    preorder(n->l);
    preorder(n->r);
}

void destroy(Node *n)
{
    if (n)
    {
        destroy(n->l);
        destroy(n->r);
        free(n);
    }
}

int main(void)
{
    int prices[] = {10, 20, 30, 40, 50, 25};

    Node *root = NULL;

    for (int i = 0; i < 6; i++)
        root = insert(root, prices[i]);

    printf("Preorder: ");
    preorder(root);

    printf("\n");

    printf("Height = %d\n", ht(root));

    destroy(root);

    return 0;
}