#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1009
#define MAX_ORDERS 200

typedef struct Order {
    char id[24];
    int restaurant_id;
    int amount;
    int promised_time;
    int arrival;
    int status;       /* 0=active, 1=assigned, 2=cancelled, 3=completed */
    int heap_index;
    struct Order *next;
} Order;

static Order *table[TABLE_SIZE];
static Order *heap[MAX_ORDERS];
static int heap_size;
static int arrival_counter;

static unsigned hash_id(const char *s) {
    unsigned h = 0;
    while (*s) h = h * 31u + (unsigned char)*s++;
    return h % TABLE_SIZE;
}

static void hash_insert(Order *o) {
    unsigned b = hash_id(o->id);
    o->next = table[b];
    table[b] = o;
}

static Order *hash_find(const char *id) {
    for (Order *p = table[hash_id(id)]; p; p = p->next)
        if (strcmp(p->id, id) == 0) return p;
    return NULL;
}

static int overdue(const Order *o, int now) {
    int value = now - o->promised_time;
    return value > 0 ? value : 0;
}

static int higher(Order *a, Order *b, int now) {
    int oa = overdue(a, now), ob = overdue(b, now);
    if (oa != ob) return oa > ob;
    return a->arrival < b->arrival;
}

static void swap_heap(int a, int b) {
    Order *t = heap[a]; heap[a] = heap[b]; heap[b] = t;
    heap[a]->heap_index = a;
    heap[b]->heap_index = b;
}

static void sift_up(int i, int now) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!higher(heap[i], heap[p], now)) break;
        swap_heap(i, p);
        i = p;
    }
}

static void sift_down(int i, int now) {
    for (;;) {
        int l = 2 * i + 1, r = l + 1, best = i;
        if (l < heap_size && higher(heap[l], heap[best], now)) best = l;
        if (r < heap_size && higher(heap[r], heap[best], now)) best = r;
        if (best == i) break;
        swap_heap(i, best);
        i = best;
    }
}

static void heap_add(Order *o, int now) {
    if (heap_size == MAX_ORDERS) return;
    o->heap_index = heap_size;
    heap[heap_size++] = o;
    sift_up(o->heap_index, now);
}

static void heap_remove_at(int index, int now) {
    if (index < 0 || index >= heap_size) return;
    Order *removed = heap[index];
    heap[index] = heap[--heap_size];
    if (index < heap_size) {
        heap[index]->heap_index = index;
        sift_up(index, now);
        sift_down(index, now);
    }
    removed->heap_index = -1;
}

static int place_order(const char *id, int restaurant, int amount,
                       int promised, int now) {
    if (hash_find(id) || heap_size == MAX_ORDERS) return 0;
    Order *o = calloc(1, sizeof(*o));
    if (!o) return 0;

    snprintf(o->id, sizeof o->id, "%s", id);
    o->restaurant_id = restaurant;
    o->amount = amount;
    o->promised_time = promised;
    o->arrival = arrival_counter++;
    o->status = 0;
    o->heap_index = -1;

    hash_insert(o);
    heap_add(o, now);
    return 1;
}

static Order *assign_rider(int now) {
    while (heap_size > 0) {
        Order *o = heap[0];
        heap_remove_at(0, now);
        if (o->status != 0) continue;
        o->status = 1;
        return o;
    }
    return NULL;
}

static int cancel_order(const char *id, int now) {
    Order *o = hash_find(id);
    if (!o || o->status != 0) return 0;
    o->status = 2;
    if (o->heap_index >= 0) heap_remove_at(o->heap_index, now);
    return 1;
}

static int compare_report(const void *x, const void *y) {
    const Order *a = *(const Order * const *)x;
    const Order *b = *(const Order * const *)y;
    if (a->restaurant_id != b->restaurant_id)
        return a->restaurant_id - b->restaurant_id;
    if (a->amount != b->amount) return a->amount - b->amount;
    return a->arrival - b->arrival;
}

static void hourly_report(void) {
    Order *items[MAX_ORDERS];
    int n = 0;
    for (int b = 0; b < TABLE_SIZE; b++)
        for (Order *p = table[b]; p; p = p->next)
            if (p->status != 2) items[n++] = p;

    qsort(items, (size_t)n, sizeof(items[0]), compare_report);
    puts("\nHourly report (restaurant, amount, order):");
    for (int i = 0; i < n; i++)
        printf("%d, %d, %s\n", items[i]->restaurant_id,
               items[i]->amount, items[i]->id);
}

int main(void) {
    const int now = 100;

    place_order("ORD1001", 12, 450, 95, now);
    place_order("ORD1002", 10, 300, 80, now);
    place_order("ORD1003", 12, 120, 90, now);
    place_order("ORD1004", 10, 990, 98, now);
    place_order("ORD1005", 12, 120, 90, now);

    Order *found = hash_find("ORD1003");
    printf("Lookup ORD1003: %s\n", found ? "found" : "not found");

    printf("Cancel ORD1004: %s\n",
           cancel_order("ORD1004", now) ? "successful" : "failed");

    puts("\nRider assignment order:");
    Order *o;
    while ((o = assign_rider(now)) != NULL)
        printf("%s (overdue %d minutes)\n",
               o->id, overdue(o, now));

    hourly_report();

    for (int b = 0; b < TABLE_SIZE; b++) {
        Order *p = table[b];
        while (p) {
            Order *next = p->next;
            free(p);
            p = next;
        }
    }
    return 0;
}
