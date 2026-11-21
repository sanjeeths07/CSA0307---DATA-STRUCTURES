#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct Node
{
    int key;
    int color;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

Node *root = NULL;

Node *createNode(int key)
{
    Node *newNode;

    newNode = (Node *)malloc(sizeof(Node));

    newNode->key = key;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;
}

void leftRotate(Node *x)
{
    Node *y;

    y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(Node *y)
{
    Node *x;

    x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void fixInsert(Node *z)
{
    Node *y;

    while (z != root && z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;

            if (y != NULL && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else
        {
            y = z->parent->parent->left;

            if (y != NULL && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }

    root->color = BLACK;
}

void insert(int key)
{
    Node *newNode;
    Node *current;
    Node *parent;

    newNode = createNode(key);

    if (root == NULL)
    {
        newNode->color = BLACK;
        root = newNode;
        return;
    }

    current = root;
    parent = NULL;

    while (current != NULL)
    {
        parent = current;

        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    fixInsert(newNode);
}

Node *findMinimum(Node *node)
{
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
        node = node->left;

    return node;
}

void inorder(Node *node)
{
    if (node == NULL)
        return;

    inorder(node->left);

    if (node->color == RED)
        printf("%d(R) ", node->key);
    else
        printf("%d(B) ", node->key);

    inorder(node->right);
}

void printTree(Node *node, int level)
{
    int i;

    if (node == NULL)
        return;

    for (i = 0; i < level; i++)
        printf("  ");

    if (node->color == RED)
        printf("%d(R)\n", node->key);
    else
        printf("%d(B)\n", node->key);

    printTree(node->left, level + 1);
    printTree(node->right, level + 1);
}

int main()
{
    int keys[] = {10, 20, 30, 15, 25, 5};
    int i;
    Node *minimum;

    printf("Red-Black Tree\n");
    printf("==============\n\n");

    for (i = 0; i < 6; i++)
    {
        printf("Insert %d\n", keys[i]);
        insert(keys[i]);
    }

    printf("\nFinal Red-Black Tree:\n");
    printTree(root, 0);

    printf("\nInorder Traversal:\n");
    inorder(root);

    minimum = findMinimum(root);

    if (minimum != NULL)
        printf("\n\nSmallest process time: %d\n", minimum->key);

    return 0;
}