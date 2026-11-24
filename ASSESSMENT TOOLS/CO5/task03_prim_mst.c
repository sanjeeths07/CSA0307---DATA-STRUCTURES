#include <stdio.h>
#include <limits.h>

#define V 5

int cost[V][V] = {
    {0, 2, 0, 6, 0},
    {2, 0, 3, 8, 5},
    {0, 3, 0, 0, 7},
    {6, 8, 0, 0, 9},
    {0, 5, 7, 9, 0}
};

int main(void) {
    int key[V], parent[V], included[V] = {0};
    int total = 0;

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    key[0] = 0;

    for (int count = 0; count < V; count++) {
        int u = -1;
        for (int v = 0; v < V; v++)
            if (!included[v] && (u == -1 || key[v] < key[u]))
                u = v;

        if (u == -1 || key[u] == INT_MAX) {
            puts("Graph is disconnected; MST does not exist.");
            return 0;
        }

        included[u] = 1;
        if (parent[u] != -1) {
            printf("%d - %d : %d\n", parent[u], u, cost[u][parent[u]]);
            total += cost[u][parent[u]];
        }

        for (int v = 0; v < V; v++) {
            if (cost[u][v] != 0 && !included[v] && cost[u][v] < key[v]) {
                key[v] = cost[u][v];
                parent[v] = u;
            }
        }
    }

    printf("Total MST cost: %d\n", total);
    return 0;
}
