#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 11
#define ID_LEN 24
#define NAME_LEN 48

typedef struct Member {
    char id[ID_LEN];
    char name[NAME_LEN];
    struct Member *next;
} Member;

static Member *table[SIZE];

static unsigned hash_id(const char *s) {
    unsigned h = 0;
    while (*s) h = h * 31u + (unsigned char)*s++;
    return h % SIZE;
}

void put(const char *id, const char *name) {
    unsigned b = hash_id(id);
    for (Member *p = table[b]; p; p = p->next) {
        if (strcmp(p->id, id) == 0) {
            snprintf(p->name, sizeof p->name, "%s", name);
            return;
        }
    }

    Member *m = malloc(sizeof *m);
    if (!m) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(m->id, sizeof m->id, "%s", id);
    snprintf(m->name, sizeof m->name, "%s", name);
    m->next = table[b];
    table[b] = m;
}

const char *get(const char *id, int *probes) {
    *probes = 0;
    for (Member *m = table[hash_id(id)]; m; m = m->next) {
        (*probes)++;
        if (strcmp(m->id, id) == 0) return m->name;
    }
    return NULL;
}

int remove_member(const char *id) {
    unsigned b = hash_id(id);
    Member **link = &table[b];
    while (*link) {
        if (strcmp((*link)->id, id) == 0) {
            Member *victim = *link;
            *link = victim->next;
            free(victim);
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

void display(void) {
    puts("\nSeparate-chaining hash table:");
    for (unsigned i = 0; i < SIZE; i++) {
        printf("[%u]", i);
        for (Member *p = table[i]; p; p = p->next)
            printf(" -> %s(%s)", p->id, p->name);
        puts(" -> NULL");
    }
}

int main(void) {
    put("CSE1044", "Arun");
    put("CSE2041", "Bala");
    put("CSE3052", "Charan");
    put("CSE9999", "Divya");
    put("CSE1077", "Esha");

    display();

    int probes;
    const char *name = get("CSE1044", &probes);
    printf("\nSearch CSE1044: %s (node visits: %d)\n",
           name ? name : "Not found", probes);

    name = get("CSE0000", &probes);
    printf("Search CSE0000: %s (node visits: %d)\n",
           name ? name : "Not found", probes);

    printf("Delete CSE2041: %s\n",
           remove_member("CSE2041") ? "successful" : "not found");
    display();

    for (unsigned i = 0; i < SIZE; i++) {
        Member *p = table[i];
        while (p) {
            Member *next = p->next;
            free(p);
            p = next;
        }
    }
    return 0;
}
