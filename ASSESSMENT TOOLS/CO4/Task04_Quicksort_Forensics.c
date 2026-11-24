#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000

static unsigned long long comparisons;
static unsigned long long swaps;
static int current_depth, max_depth;

static void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
    swaps++;
}

/* Last-element pivot, matching the assignment's forensic example. */
static int partition_last(int a[], int low, int high) {
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        comparisons++;
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[high]);
    return i + 1;
}

static void quicksort(int a[], int low, int high, int depth) {
    if (depth > max_depth) max_depth = depth;
    if (low < high) {
        int p = partition_last(a, low, high);
        quicksort(a, low, p - 1, depth + 1);
        quicksort(a, p + 1, high, depth + 1);
    }
}

static void run_case(const char *label, int sorted_input) {
    int *a = malloc(N * sizeof(*a));
    if (!a) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < N; i++)
        a[i] = sorted_input ? i : rand();

    comparisons = swaps = 0;
    max_depth = 0;
    quicksort(a, 0, N - 1, 1);

    printf("%s input (%d items)\n", label, N);
    printf("Comparisons: %llu\n", comparisons);
    printf("Swap calls: %llu\n", swaps);
    printf("Maximum recursion depth: %d\n\n", max_depth);
    free(a);
}

int main(void) {
    srand(42);
    puts("LAST-PIVOT QUICKSORT FORENSICS");
    run_case("Random", 0);
    run_case("Already sorted", 1);
    puts("For sorted input, last-pivot quicksort has quadratic work and");
    puts("linear recursion depth. Run with smaller N if stack space is limited.");
    return 0;
}
