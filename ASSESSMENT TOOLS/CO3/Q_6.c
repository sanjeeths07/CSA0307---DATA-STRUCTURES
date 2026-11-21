#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

typedef struct Trie
{
    struct Trie *child[ALPHABET];
    int end;
    int words;
} Trie;

Trie *node()
{
    Trie *newNode;
    int i;

    newNode = (Trie *)malloc(sizeof(Trie));

    if (newNode == NULL)
        return NULL;

    newNode->end = 0;
    newNode->words = 0;

    for (i = 0; i < ALPHABET; i++)
        newNode->child[i] = NULL;

    return newNode;
}

/* Insert a word */
void insert(Trie *t, const char *w)
{
    Trie *cur = t;
    int i;

    for (; *w; w++)
    {
        i = *w - 'a';

        if (cur->child[i] == NULL)
            cur->child[i] = node();

        cur = cur->child[i];
        cur->words++;
    }

    cur->end = 1;
}

/* Search a complete word */
int search(Trie *t, const char *w)
{
    Trie *cur = t;
    int i;

    for (; *w; w++)
    {
        i = *w - 'a';

        if (cur->child[i] == NULL)
            return 0;

        cur = cur->child[i];
    }

    return cur->end;
}

/* Count words with a given prefix */
int count_prefix(const Trie *t, const char *p)
{
    const Trie *cur = t;
    int i;

    if (*p == '\0')
        return cur->words;

    for (; *p; p++)
    {
        i = *p - 'a';

        if (cur->child[i] == NULL)
            return 0;

        cur = cur->child[i];
    }

    return cur->words;
}

/* Print all words with a prefix */
void autocomplete(Trie *root, char *prefix, int level)
{
    int i;

    if (root->end)
    {
        prefix[level] = '\0';
        printf("%s\n", prefix);
    }

    for (i = 0; i < ALPHABET; i++)
    {
        if (root->child[i] != NULL)
        {
            prefix[level] = 'a' + i;
            autocomplete(root->child[i], prefix, level + 1);
        }
    }
}

/* Delete a word */
int deleteWord(Trie *root, const char *word, int depth)
{
    int i;

    if (word[depth] == '\0')
    {
        if (!root->end)
            return 0;

        root->end = 0;
        return 1;
    }

    i = word[depth] - 'a';

    if (root->child[i] == NULL)
        return 0;

    if (deleteWord(root->child[i], word, depth + 1))
    {
        if (root->child[i]->words > 0)
            root->child[i]->words--;

        return 1;
    }

    return 0;
}

/* Free the trie */
void freeTrie(Trie *root)
{
    int i;

    if (root == NULL)
        return;

    for (i = 0; i < ALPHABET; i++)
        freeTrie(root->child[i]);

    free(root);
}

int main()
{
    Trie *root;
    char *names[] = {
        "anand",
        "anitha",
        "anu",
        "anusuya",
        "arun",
        "bala"
    };

    char prefix[50];
    int i;

    root = node();

    printf("TRIE - CONTACT SEARCH\n");
    printf("=====================\n\n");

    /* Insert contacts */
    printf("Contacts inserted:\n");

    for (i = 0; i < 6; i++)
    {
        insert(root, names[i]);
        printf("%s\n", names[i]);
    }

    /* Search */
    printf("\nEnter contact to search: ");
    scanf("%49s", prefix);

    if (search(root, prefix))
        printf("Contact found\n");
    else
        printf("Contact not found\n");

    /* Prefix count */
    printf("\nEnter prefix: ");
    scanf("%49s", prefix);

    printf("Number of contacts with prefix \"%s\": %d\n",
           prefix, count_prefix(root, prefix));

    /* Autocomplete */
    printf("\nAutocomplete suggestions:\n");

    {
        Trie *cur = root;
        int valid = 1;

        for (i = 0; prefix[i] != '\0'; i++)
        {
            int index = prefix[i] - 'a';

            if (cur->child[index] == NULL)
            {
                valid = 0;
                break;
            }

            cur = cur->child[index];
        }

        if (valid)
            autocomplete(cur, prefix, i);
        else
            printf("No suggestions\n");
    }

    /* Delete */
    printf("\nEnter contact to delete: ");
    scanf("%49s", prefix);

    if (deleteWord(root, prefix, 0))
        printf("Contact deleted\n");
    else
        printf("Contact not found\n");

    freeTrie(root);

    return 0;
}