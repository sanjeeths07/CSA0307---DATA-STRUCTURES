#include <stdio.h>
#include <stdlib.h>

#define N 7

typedef struct Node {
    int v;
    struct Node *next;
} Node;

Node *adj[N] = {NULL};

void edge(int u, int v) {
    Node *a = malloc(sizeof *a);
    Node *b = malloc(sizeof *b);
    if (!a || !b) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    a->v = v; a->next = adj[u]; adj[u] = a;
    b->v = u; b->next = adj[v]; adj[v] = b;
}

void bfs(int start) {
    int seen[N] = {0}, q[N], head = 0, tail = 0;
    seen[start] = 1;
    q[tail++] = start;

    printf("BFS:");
    while (head < tail) {
        int u = q[head++];
        printf(" %d", u);
        for (Node *p = adj[u]; p; p = p->next) {
            if (!seen[p->v]) {
                seen[p->v] = 1;
                q[tail++] = p->v;
            }
        }
    }
    puts("");
}

void dfs_recursive(int u, int seen[]) {
    seen[u] = 1;
    printf(" %d", u);
    for (Node *p = adj[u]; p; p = p->next)
        if (!seen[p->v])
            dfs_recursive(p->v, seen);
}

int has_cycle(int u, int parent, int seen[]) {
    seen[u] = 1;
    for (Node *p = adj[u]; p; p = p->next) {
        int v = p->v;
        if (!seen[v]) {
            if (has_cycle(v, u, seen)) return 1;
        } else if (v != parent) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    edge(0, 1); edge(1, 2); edge(1, 5);
    edge(2, 3); edge(3, 4); edge(4, 6); edge(5, 6);

    bfs(0);

    int seen[N] = {0};
    printf("DFS:");
    dfs_recursive(0, seen);
    puts("");

    int components = 0;
    for (int i = 0; i < N; i++) {
        if (!seen[i]) {
            components++;
            dfs_recursive(i, seen);
        }
    }
    printf("Connected components: %d\n", components);

    int cycle_seen[N] = {0}, cycle = 0;
    for (int i = 0; i < N; i++)
        if (!cycle_seen[i] && has_cycle(i, -1, cycle_seen))
            cycle = 1;
    printf("Undirected cycle: %s\n", cycle ? "Yes" : "No");

    return 0;
}
