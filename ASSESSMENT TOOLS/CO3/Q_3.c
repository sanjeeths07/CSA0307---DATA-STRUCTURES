#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct BTreeNode
{
    int keys[3];
    struct BTreeNode *child[4];
    int n;
    int leaf;
} BTreeNode;

BTreeNode *createNode(int leaf)
{
    BTreeNode *node;
    int i;

    node = (BTreeNode *)malloc(sizeof(BTreeNode));

    node->leaf = leaf;
    node->n = 0;

    for (i = 0; i < 4; i++)
        node->child[i] = NULL;

    return node;
}

void traverse(BTreeNode *root)
{
    int i;

    if (root == NULL)
        return;

    for (i = 0; i < root->n; i++)
    {
        if (!root->leaf)
            traverse(root->child[i]);

        printf("%d ", root->keys[i]);
    }

    if (!root->leaf)
        traverse(root->child[i]);
}

BTreeNode *search(BTreeNode *root, int key)
{
    int i;

    if (root == NULL)
        return NULL;

    i = 0;

    while (i < root->n && key > root->keys[i])
        i++;

    if (i < root->n && key == root->keys[i])
        return root;

    if (root->leaf)
        return NULL;

    return search(root->child[i], key);
}

void splitChild(BTreeNode *parent, int i, BTreeNode *fullChild)
{
    BTreeNode *newChild;
    int j;

    newChild = createNode(fullChild->leaf);

    newChild->n = 1;

    newChild->keys[0] = fullChild->keys[2];

    if (!fullChild->leaf)
    {
        newChild->child[0] = fullChild->child[2];
        newChild->child[1] = fullChild->child[3];
    }

    fullChild->n = 1;

    for (j = parent->n; j >= i + 1; j--)
        parent->child[j + 1] = parent->child[j];

    parent->child[i + 1] = newChild;

    for (j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = fullChild->keys[1];

    parent->n++;
}

void insertNonFull(BTreeNode *node, int key)
{
    int i;

    i = node->n - 1;

    if (node->leaf)
    {
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->n++;
    }
    else
    {
        while (i >= 0 && key < node->keys[i])
            i--;

        i++;

        if (node->child[i]->n == 3)
        {
            splitChild(node, i, node->child[i]);

            if (key > node->keys[i])
                i++;
        }

        insertNonFull(node->child[i], key);
    }
}

void insert(BTreeNode **root, int key)
{
    BTreeNode *newRoot;
    int i;

    if (*root == NULL)
    {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->n = 1;
        return;
    }

    if ((*root)->n == 3)
    {
        newRoot = createNode(0);

        newRoot->child[0] = *root;

        splitChild(newRoot, 0, *root);

        i = 0;

        if (key > newRoot->keys[0])
            i++;

        insertNonFull(newRoot->child[i], key);

        *root = newRoot;
    }
    else
    {
        insertNonFull(*root, key);
    }
}

void printTree(BTreeNode *root, int level)
{
    int i;

    if (root == NULL)
        return;

    printf("Level %d: [", level);

    for (i = 0; i < root->n; i++)
    {
        printf("%d", root->keys[i]);

        if (i < root->n - 1)
            printf(", ");
    }

    printf("]\n");

    if (!root->leaf)
    {
        for (i = 0; i <= root->n; i++)
            printTree(root->child[i], level + 1);
    }
}

void freeTree(BTreeNode *root)
{
    int i;

    if (root == NULL)
        return;

    if (!root->leaf)
    {
        for (i = 0; i <= root->n; i++)
            freeTree(root->child[i]);
    }

    free(root);
}

int main(void)
{
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = 8;
    int i;
    int key;
    BTreeNode *root = NULL;

    printf("B-Tree Insertion\n");
    printf("================\n\n");

    for (i = 0; i < n; i++)
    {
        insert(&root, keys[i]);

        printf("After inserting %d:\n", keys[i]);
        printTree(root, 0);
        printf("\n");
    }

    printf("Final B-Tree:\n");
    printTree(root, 0);

    printf("\nInorder Traversal: ");
    traverse(root);

    printf("\n\nEnter key to search: ");
    scanf("%d", &key);

    if (search(root, key) != NULL)
        printf("Key %d found\n", key);
    else
        printf("Key %d not found\n", key);

    freeTree(root);

    return 0;
}