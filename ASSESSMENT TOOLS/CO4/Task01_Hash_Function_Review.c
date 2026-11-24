#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define WEAK_BUCKETS 1000u
#define GOOD_BUCKETS 131071u
#define RECORDS 100000u

typedef struct Node {
    char key[32];
    struct Node *next;
} Node;

static unsigned weak_hash(const char *s) {
    return ((unsigned char)s[0] + (unsigned char)s[1]) % WEAK_BUCKETS;
}

static unsigned polynomial_hash(const char *s, unsigned m) {
    uint32_t h = 0;
    while (*s)
        h = h * 31u + (unsigned char)*s++;
    return h % m;
}

static void make_registration(char *out, unsigned i) {
    /* Deterministic sample registration-like identifiers. */
    snprintf(out, 32, "TN%02u%c%c%04u",
             (i % 99u) + 1u,
             (char)('A' + (i / 26u) % 26u),
             (char)('A' + i % 26u),
             i % 10000u);
}

static void measure(const char *label, unsigned buckets, int improved) {
    Node **table = calloc(buckets, sizeof(*table));
    unsigned *lengths = calloc(buckets, sizeof(*lengths));
    if (!table || !lengths) {
        fprintf(stderr, "Memory allocation failed\n");
        free(table); free(lengths);
        return;
    }

    for (unsigned i = 0; i < RECORDS; i++) {
        char key[32];
        make_registration(key, i);
        unsigned b = improved ? polynomial_hash(key, buckets)
                              : weak_hash(key);
        Node *node = malloc(sizeof(*node));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            break;
        }
        strcpy(node->key, key);
        node->next = table[b];
        table[b] = node;
        lengths[b]++;
    }

    unsigned max_chain = 0, nonempty = 0;
    unsigned long long total_nonempty = 0;
    for (unsigned i = 0; i < buckets; i++) {
        if (lengths[i]) {
            nonempty++;
            total_nonempty += lengths[i];
            if (lengths[i] > max_chain) max_chain = lengths[i];
        }
        Node *p = table[i];
        while (p) {
            Node *next = p->next;
            free(p);
            p = next;
        }
    }

    printf("%s\n", label);
    printf("Buckets: %u | Records: %u\n", buckets, RECORDS);
    printf("Load factor: %.3f\n", (double)RECORDS / buckets);
    printf("Non-empty buckets: %u\n", nonempty);
    printf("Average entries per non-empty bucket: %.3f\n",
           nonempty ? (double)total_nonempty / nonempty : 0.0);
    printf("Longest chain: %u\n\n", max_chain);

    free(table);
    free(lengths);
}

int main(void) {
    puts("HASH FUNCTION DISTRIBUTION EXPERIMENT");
    measure("Weak hash (first two characters)", WEAK_BUCKETS, 0);
    measure("Polynomial hash (entire registration)", GOOD_BUCKETS, 1);
    return 0;
}
