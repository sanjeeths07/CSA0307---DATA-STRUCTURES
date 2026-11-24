#include <stdio.h>

#define V 6

const char *task[V] = {
    "Lease", "Power", "Racks", "Network", "Staff", "Launch"
};

int graph[V][V] = {
    {0, 1, 1, 0, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0}
};

int main(void) {
    int indegree[V] = {0};
    int queue[V], head = 0, tail = 0, processed = 0;

    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (graph[u][v])
                indegree[v]++;

    for (int v = 0; v < V; v++)
        if (indegree[v] == 0)
            queue[tail++] = v;

    printf("Topological order: ");
    while (head < tail) {
        int u = queue[head++];
        printf("%s ", task[u]);
        processed++;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && --indegree[v] == 0)
                queue[tail++] = v;
        }
    }

    if (processed != V)
        puts("\nCycle detected: topological ordering is not possible.");
    else
        puts("\nNo cycle detected.");

    return 0;
}
