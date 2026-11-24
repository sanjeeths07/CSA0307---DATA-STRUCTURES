#include <stdio.h>

#define N 8
#define INF 999999

int weight[N][N] = {
    {0, 4, 0, 0, 0, 0, 0, 10},
    {4, 0, 3, 0, 0, 0, 0, 0},
    {0, 3, 0, 2, 0, 0, 0, 0},
    {0, 0, 2, 0, 5, 0, 0, 0},
    {0, 0, 0, 5, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 2, 0},
    {0, 0, 0, 0, 0, 2, 0, 3},
    {10, 0, 0, 0, 0, 0, 3, 0}
};

void dijkstra(int source, int dist[]) {
    int used[N] = {0};

    for (int i = 0; i < N; i++) dist[i] = INF;
    dist[source] = 0;

    for (int count = 0; count < N; count++) {
        int u = -1;
        for (int i = 0; i < N; i++)
            if (!used[i] && (u == -1 || dist[i] < dist[u]))
                u = i;

        if (u == -1 || dist[u] == INF) break;
        used[u] = 1;

        for (int v = 0; v < N; v++)
            if (weight[u][v] > 0 && !used[v] &&
                dist[u] + weight[u][v] < dist[v])
                dist[v] = dist[u] + weight[u][v];
    }
}

int main(void) {
    int dist[N];
    dijkstra(0, dist);

    puts("Dijkstra distances from vertex 0:");
    for (int i = 0; i < N; i++)
        printf("Vertex %d: %d\n", i, dist[i]);

    puts("\nFor comparison, run Floyd-Warshall on the same weight matrix.");
    puts("The distances from source 0 should match the first row of");
    puts("the Floyd-Warshall result matrix.");

    return 0;
}
