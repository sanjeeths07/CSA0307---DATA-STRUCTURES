#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ALPHABET 26
#define MAX_WORD 100

typedef struct TrieNode
{
    struct TrieNode *child[ALPHABET];
    int endOfWord;
} TrieNode;

/* Create a new Trie node */
TrieNode *createNode()
{
    TrieNode *newNode;
    int i;

    newNode = (TrieNode *)malloc(sizeof(TrieNode));

    newNode->endOfWord = 0;

    for (i = 0; i < ALPHABET; i++)
        newNode->child[i] = NULL;

    return newNode;
}

/* Insert a word */
void insert(TrieNode *root, char word[])
{
    TrieNode *current;
    int i;
    int index;

    current = root;

    for (i = 0; word[i] != '\0'; i++)
    {
        index = word[i] - 'a';

        if (current->child[index] == NULL)
            current->child[index] = createNode();

        current = current->child[index];
    }

    current->endOfWord = 1;
}

/* Exact word lookup */
int search(TrieNode *root, char word[])
{
    TrieNode *current;
    int i;
    int index;

    current = root;

    for (i = 0; word[i] != '\0'; i++)
    {
        index = word[i] - 'a';

        if (current->child[index] == NULL)
            return 0;

        current = current->child[index];
    }

    return current->endOfWord;
}

/* Display autocomplete suggestions */
void autocomplete(TrieNode *root, char word[], int level)
{
    int i;

    if (root->endOfWord)
    {
        word[level] = '\0';
        printf("%s\n", word);
    }

    for (i = 0; i < ALPHABET; i++)
    {
        if (root->child[i] != NULL)
        {
            word[level] = 'a' + i;
            autocomplete(root->child[i], word, level + 1);
        }
    }
}

/* Find prefix node */
TrieNode *findPrefixNode(TrieNode *root, char prefix[])
{
    TrieNode *current;
    int i;
    int index;

    current = root;

    for (i = 0; prefix[i] != '\0'; i++)
    {
        index = prefix[i] - 'a';

        if (current->child[index] == NULL)
            return NULL;

        current = current->child[index];
    }

    return current;
}

/* Display words in alphabetical order */
void displayWords(TrieNode *root, char word[], int level)
{
    int i;

    if (root->endOfWord)
    {
        word[level] = '\0';
        printf("%s\n", word);
    }

    for (i = 0; i < ALPHABET; i++)
    {
        if (root->child[i] != NULL)
        {
            word[level] = 'a' + i;
            displayWords(root->child[i], word, level + 1);
        }
    }
}

/* Display words between two given words */
void rangeListing(TrieNode *root, char word[], int level,
                  char start[], char end[])
{
    int i;

    if (root->endOfWord)
    {
        word[level] = '\0';

        if (strcmp(word, start) >= 0 && strcmp(word, end) <= 0)
            printf("%s\n", word);
    }

    for (i = 0; i < ALPHABET; i++)
    {
        if (root->child[i] != NULL)
        {
            word[level] = 'a' + i;
            rangeListing(root->child[i], word, level + 1, start, end);
        }
    }
}

/* Count Trie nodes */
int countNodes(TrieNode *root)
{
    int i;
    int count = 1;

    for (i = 0; i < ALPHABET; i++)
    {
        if (root->child[i] != NULL)
            count = count + countNodes(root->child[i]);
    }

    return count;
}

/* Free Trie */
void freeTrie(TrieNode *root)
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
    TrieNode *root;
    TrieNode *prefixNode;

    int n;
    int i;
    int nodes;

    char word[MAX_WORD];
    char prefix[MAX_WORD];
    char start[MAX_WORD];
    char end[MAX_WORD];

    clock_t startTime;
    clock_t endTime;

    root = createNode();

    printf("TRIE BASED DICTIONARY\n");
    printf("=====================\n\n");

    printf("Enter number of words: ");
    scanf("%d", &n);

    printf("\nEnter %d words in lowercase:\n", n);

    for (i = 0; i < n; i++)
    {
        scanf("%99s", word);
        insert(root, word);
    }

    nodes = countNodes(root);

    printf("\nDictionary created successfully.");
    printf("\nNumber of words: %d", n);
    printf("\nNumber of Trie nodes: %d", nodes);
    printf("\nApproximate memory used: %lu bytes\n",
           (unsigned long)(nodes * sizeof(TrieNode)));

    /* Exact lookup */
    printf("\nEnter word for exact lookup: ");
    scanf("%99s", word);

    startTime = clock();

    if (search(root, word))
        printf("Word found\n");
    else
        printf("Word not found\n");

    endTime = clock();

    printf("Lookup time: %.6f seconds\n",
           (double)(endTime - startTime) / CLOCKS_PER_SEC);

    /* Autocomplete */
    printf("\nEnter prefix for autocomplete: ");
    scanf("%99s", prefix);

    prefixNode = findPrefixNode(root, prefix);

    printf("\nAutocomplete suggestions:\n");

    if (prefixNode == NULL)
    {
        printf("No suggestions found\n");
    }
    else
    {
        strcpy(word, prefix);
        autocomplete(prefixNode, word, strlen(prefix));
    }

    /* Alphabetical range */
    printf("\nEnter starting word: ");
    scanf("%99s", start);

    printf("Enter ending word: ");
    scanf("%99s", end);

    printf("\nAlphabetical range listing:\n");

    rangeListing(root, word, 0, start, end);

    freeTrie(root);

    return 0;
}