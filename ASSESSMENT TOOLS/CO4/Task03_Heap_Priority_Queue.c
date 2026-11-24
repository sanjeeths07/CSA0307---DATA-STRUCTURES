#include <stdio.h>

#define MAX 100

typedef struct {
    int severity;
    int patient;
    unsigned long arrival;
} Case;

static Case heap[MAX];
static int size = 0;
static unsigned long next_arrival = 0;

static void swap(Case *a, Case *b) {
    Case t = *a; *a = *b; *b = t;
}

/* Higher severity first; earlier arrival wins ties. */
static int higher_priority(Case a, Case b) {
    if (a.severity != b.severity) return a.severity > b.severity;
    return a.arrival < b.arrival;
}

void push(int severity, int patient) {
    if (size == MAX) {
        puts("Priority queue is full.");
        return;
    }
    heap[size] = (Case){severity, patient, next_arrival++};
    int i = size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!higher_priority(heap[i], heap[parent])) break;
        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

Case pop(void) {
    Case empty = {-1, -1, 0};
    if (size == 0) return empty;

    Case top = heap[0];
    heap[0] = heap[--size];
    int i = 0;

    while (1) {
        int left = 2 * i + 1, right = left + 1, best = i;
        if (left < size && higher_priority(heap[left], heap[best]))
            best = left;
        if (right < size && higher_priority(heap[right], heap[best]))
            best = right;
        if (best == i) break;
        swap(&heap[i], &heap[best]);
        i = best;
    }
    return top;
}

void display_heap(void) {
    printf("Heap array: ");
    for (int i = 0; i < size; i++)
        printf("P%d(%d) ", heap[i].patient, heap[i].severity);
    puts("");
}

int main(void) {
    push(3, 101);
    push(9, 102);
    push(5, 103);
    push(7, 104);
    push(1, 105);
    push(8, 106);

    display_heap();
    puts("\nPatient removal order (highest severity first):");
    while (size > 0) {
        Case c = pop();
        printf("P%d (severity %d)\n", c.patient, c.severity);
    }
    return 0;
}
