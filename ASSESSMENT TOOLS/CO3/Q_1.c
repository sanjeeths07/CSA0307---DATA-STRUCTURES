#include <stdio.h>
#include <stdlib.h>

/* =====================================================
   Q1 (1) – Array Binary Tree Traversals
   ===================================================== */

const char T[] = "ABCDE.F";
int N = 7;

void pre(int i)
{
    if (i >= N || T[i] == '.')
        return;

    printf("%c", T[i]);
    pre(2 * i + 1);
    pre(2 * i + 2);
}

void in(int i)
{
    if (i >= N || T[i] == '.')
        return;

    in(2 * i + 1);
    printf("%c", T[i]);
    in(2 * i + 2);
}

void post(int i)
{
    if (i >= N || T[i] == '.')
        return;

    post(2 * i + 1);
    post(2 * i + 2);
    printf("%c", T[i]);
}


/* =====================================================
   Q1 (2) – BST Insertion and Deletion
   ===================================================== */

typedef struct Node
{
    int k;
    struct Node *l, *r;
} Node;

Node *ins(Node *t, int k)
{
    if (!t)
    {
        t = calloc(1, sizeof *t);
        t->k = k;
        return t;
    }

    if (k < t->k)
        t->l = ins(t->l, k);
    else
        t->r = ins(t->r, k);

    return t;
}

Node *del(Node *t, int k)
{
    if (!t)
        return NULL;

    if (k < t->k)
        t->l = del(t->l, k);

    else if (k > t->k)
        t->r = del(t->r, k);

    else if (t->l && t->r)
    {
        Node *s = t->r;

        while (s->l)
            s = s->l;

        t->k = s->k;
        t->r = del(t->r, s->k);
    }

    else
    {
        Node *c = t->l ? t->l : t->r;

        free(t);
        return c;
    }

    return t;
}

void preBST(Node *t)
{
    if (t)
    {
        printf("%d ", t->k);
        preBST(t->l);
        preBST(t->r);
    }
}

void freeBST(Node *t)
{
    if (t)
    {
        freeBST(t->l);
        freeBST(t->r);
        free(t);
    }
}


/* =====================================================
   Q1 (3) – BST Insert, Height, Search
   ===================================================== */

typedef struct Book
{
    int acc;
    struct Book *left, *right;
} Book;

Book *insert(Book *root, int acc)
{
    if (!root)
    {
        root = malloc(sizeof *root);

        if (!root)
            return NULL;

        root->acc = acc;
        root->left = root->right = NULL;

        return root;
    }

    if (acc < root->acc)
        root->left = insert(root->left, acc);

    else if (acc > root->acc)
        root->right = insert(root->right, acc);

    return root;
}

int height(const Book *r)
{
    if (!r)
        return -1;

    int l = height(r->left);
    int rr = height(r->right);

    return 1 + (l > rr ? l : rr);
}

int search(const Book *r, int acc, int *cmp)
{
    *cmp = 0;

    while (r)
    {
        (*cmp)++;

        if (acc == r->acc)
            return 1;

        r = acc < r->acc ? r->left : r->right;
    }

    return 0;
}

void inorder(const Book *r)
{
    if (!r)
        return;

    inorder(r->left);
    printf("%d ", r->acc);
    inorder(r->right);
}

void destroy(Book *r)
{
    if (!r)
        return;

    destroy(r->left);
    destroy(r->right);
    free(r);
}


/* =====================================================
   MAIN
   ===================================================== */

int main(void)
{
    /* ---------------- Q1 (1) ---------------- */

    printf("Q1 (1) - Binary Tree Traversals\n");

    printf("Preorder   : ");
    pre(0);

    printf("\nInorder    : ");
    in(0);

    printf("\nPostorder  : ");
    post(0);

    printf("\nLevel-order: ");

    for (int i = 0; i < N; i++)
    {
        if (T[i] != '.')
            printf("%c", T[i]);
    }

    printf("\n\n");


    /* ---------------- Q1 (2) ---------------- */

    printf("Q1 (2) - BST Deletion\n");

    int a[] = {50, 30, 70, 20, 40, 60, 80, 65};

    Node *t = NULL;

    for (int i = 0; i < 8; i++)
        t = ins(t, a[i]);

    t = del(t, 20);
    printf("After deleting 20: ");
    preBST(t);

    printf("\n");

    t = del(t, 30);
    printf("After deleting 30: ");
    preBST(t);

    printf("\n");

    t = del(t, 50);
    printf("After deleting 50: ");
    preBST(t);

    printf("\n\n");

    freeBST(t);


    /* ---------------- Q1 (3) ---------------- */

    printf("Q1 (3) - BST Insert, Height and Search\n");

    int random[] = {500, 300, 700, 200, 400, 600, 800, 350};
    int sorted[] = {10, 20, 30, 40, 50, 60, 70, 80};

    Book *randomTree = NULL;
    Book *sortedTree = NULL;

    for (int i = 0; i < 8; i++)
        randomTree = insert(randomTree, random[i]);

    for (int i = 0; i < 8; i++)
        sortedTree = insert(sortedTree, sorted[i]);

    printf("Inorder: ");
    inorder(randomTree);

    printf("\n");

    printf("Height: random order %d, sorted order %d\n",
           height(randomTree), height(sortedTree));

    int c1, c2;

    search(randomTree, 350, &c1);
    search(sortedTree, 80, &c2);

    printf("Comparisons: %d and %d\n", c1, c2);

    destroy(randomTree);
    destroy(sortedTree);

    return 0;
}