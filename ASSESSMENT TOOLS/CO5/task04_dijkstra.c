#include <stdio.h>
#include <limits.h>

#define V 6
#define INF (LLONG_MAX / 4)

long long weight[V][V] = {
    {0, 7, 9, 0, 0, 14},
    {7, 0, 10, 15, 0, 0},
    {9, 10, 0, 11, 0, 2},
    {0, 15, 11, 0, 6, 0},
    {0, 0, 0, 6, 0, 9},
    {14, 0, 2, 0, 9, 0}
};

int main(void) {
    long long dist[V];
    int previous[V], done[V] = {0};

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        previous[i] = -1;
    }
    dist[0] = 0;

    for (int count = 0; count < V; count++) {
        int u = -1;
        for (int v = 0; v < V; v++)
            if (!done[v] && (u == -1 || dist[v] < dist[u]))
                u = v;

        if (u == -1 || dist[u] == INF)
            break;

        done[u] = 1;
        for (int v = 0; v < V; v++) {
            if (weight[u][v] > 0 && !done[v] &&
                dist[u] + weight[u][v] < dist[v]) {
                dist[v] = dist[u] + weight[u][v];
                previous[v] = u;
            }
        }
    }

    puts("Vertex\tDistance from 0");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF)
            printf("%d\tUnreachable\n", i);
        else
            printf("%d\t%lld\n", i, dist[i]);
    }

    printf("Path to vertex 4: ");
    int path[V], length = 0;
    for (int v = 4; v != -1; v = previous[v])
        path[length++] = v;
    for (int i = length - 1; i >= 0; i--)
        printf("%d%s", path[i], i ? " -> " : "\n");

    return 0;
}
