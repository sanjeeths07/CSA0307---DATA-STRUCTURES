#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int key)
{
    Node *newNode;

    newNode = (Node *)malloc(sizeof(Node));

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* Right Rotation */
Node *rightRotate(Node *x)
{
    Node *y;

    y = x->left;
    x->left = y->right;
    y->right = x;

    return y;
}

/* Left Rotation */
Node *leftRotate(Node *x)
{
    Node *y;

    y = x->right;
    x->right = y->left;
    y->left = x;

    return y;
}

/* Splay operation */
Node *splay(Node *root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    /* Key lies in left subtree */
    if (key < root->key)
    {
        if (root->left == NULL)
            return root;

        /* Zig-Zig */
        if (key < root->left->key)
        {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }

        /* Zig-Zag */
        else if (key > root->left->key)
        {
            root->left->right = splay(root->left->right, key);

            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        if (root->left == NULL)
            return root;

        return rightRotate(root);
    }

    /* Key lies in right subtree */
    else
    {
        if (root->right == NULL)
            return root;

        /* Zag-Zag */
        if (key > root->right->key)
        {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        /* Zag-Zig */
        else if (key < root->right->key)
        {
            root->right->left = splay(root->right->left, key);

            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }

        if (root->right == NULL)
            return root;

        return leftRotate(root);
    }
}

/* Insert into Splay Tree */
Node *insert(Node *root, int key)
{
    Node *newNode;

    if (root == NULL)
        return createNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    newNode = createNode(key);

    if (key < root->key)
    {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
    else
    {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

/* Search in Splay Tree */
Node *search(Node *root, int key)
{
    if (root == NULL)
        return NULL;

    root = splay(root, key);

    return root;
}

/* Delete from Splay Tree */
Node *deleteNode(Node *root, int key)
{
    Node *temp;

    if (root == NULL)
        return NULL;

    root = splay(root, key);

    if (root->key != key)
        return root;

    if (root->left == NULL)
    {
        temp = root;
        root = root->right;
        free(temp);
    }
    else
    {
        temp = root;

        root = splay(root->left, key);

        root->right = temp->right;

        free(temp);
    }

    return root;
}

/* Inorder Traversal */
void inorder(Node *root)
{
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/* Preorder Traversal */
void preorder(Node *root)
{
    if (root == NULL)
        return;

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

/* Display Tree */
void display(Node *root, int level)
{
    int i;

    if (root == NULL)
        return;

    for (i = 0; i < level; i++)
        printf("  ");

    printf("%d\n", root->key);

    display(root->left, level + 1);
    display(root->right, level + 1);
}

/* Free memory */
void freeTree(Node *root)
{
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

int main()
{
    Node *root;
    Node *result;
    int keys[] = {10, 20, 30, 40, 50};
    int i;
    int key;

    root = NULL;

    printf("Splay Tree\n");
    printf("==========\n\n");

    /* Insertion */
    printf("Inserting: ");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", keys[i]);
        root = insert(root, keys[i]);
    }

    printf("\n\n");

    printf("Tree after insertion:\n");
    display(root, 0);

    printf("\nInorder Traversal: ");
    inorder(root);

    printf("\nPreorder Traversal: ");
    preorder(root);

    /* Search */
    printf("\n\nEnter key to search: ");
    scanf("%d", &key);

    result = search(root, key);

    if (result != NULL && result->key == key)
    {
        root = result;
        printf("Key %d found\n", key);
        printf("Key %d is now at the root\n", key);
    }
    else
    {
        printf("Key %d not found\n", key);
    }

    printf("\nTree after search:\n");
    display(root, 0);

    /* Delete */
    printf("\nEnter key to delete: ");
    scanf("%d", &key);

    root = deleteNode(root, key);

    printf("Tree after deletion:\n");
    display(root, 0);

    printf("\nInorder Traversal after deletion: ");
    inorder(root);

    printf("\n");

    freeTree(root);

    return 0;
}