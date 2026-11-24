#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 10000

typedef struct {
    int amount;
    int order_id;
} Order;

static unsigned long long comparisons;

static void merge(Order a[], Order tmp[], int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        comparisons++;
        if (a[i].amount <= a[j].amount)
            tmp[k++] = a[i++]; /* <= preserves stability */
        else
            tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi) tmp[k++] = a[j++];
    for (int x = lo; x <= hi; x++) a[x] = tmp[x];
}

static void merge_sort(Order a[], Order tmp[], int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(a, tmp, lo, mid);
    merge_sort(a, tmp, mid + 1, hi);
    merge(a, tmp, lo, mid, hi);
}

static void parallel_merge_sort(Order a[], Order tmp[], int lo, int hi, int depth) {
    if (lo >= hi) return;
    if (depth <= 3 && hi - lo > 1000) {
        int mid = lo + (hi - lo) / 2;
        #pragma omp task shared(a, tmp) firstprivate(lo, mid, depth)
        parallel_merge_sort(a, tmp, lo, mid, depth + 1);
        #pragma omp task shared(a, tmp) firstprivate(mid, hi, depth)
        parallel_merge_sort(a, tmp, mid + 1, hi, depth + 1);
        #pragma omp taskwait
        merge(a, tmp, lo, mid, hi);
    } else {
        merge_sort(a, tmp, lo, hi);
    }
}

static int is_sorted(const Order a[], int n) {
    for (int i = 1; i < n; i++)
        if (a[i - 1].amount > a[i].amount) return 0;
    return 1;
}

int main(void) {
    Order *a = malloc(N * sizeof(*a));
    Order *tmp = malloc(N * sizeof(*tmp));
    if (!a || !tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        free(a); free(tmp);
        return 1;
    }

    srand(42);
    for (int i = 0; i < N; i++) {
        a[i].amount = rand() % 1000;
        a[i].order_id = i + 1;
    }

    double start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallel_merge_sort(a, tmp, 0, N - 1, 0);
    }
    double elapsed = omp_get_wtime() - start;

    printf("Parallel merge sort of %d records\n", N);
    printf("Sorted correctly: %s\n", is_sorted(a, N) ? "Yes" : "No");
    printf("Comparisons: %llu\n", comparisons);
    printf("Elapsed time: %.6f seconds\n", elapsed);
    printf("OpenMP threads available: %d\n", omp_get_max_threads());

    puts("\nFirst 10 sorted records (amount : order ID):");
    for (int i = 0; i < 10 && i < N; i++)
        printf("%d : #%d\n", a[i].amount, a[i].order_id);

    free(a);
    free(tmp);
    return 0;
}
