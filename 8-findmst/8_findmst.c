#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MAX_VERTICES 100
#define INF 1000

int parent[MAX_VERTICES]; // �θ� ���

// �ʱ�ȭ
void set_init(int n) {
    for (int i = 0; i < n; i++)
        parent[i] = -1;
}

// curr�� ���ϴ� ������ ��ȯ�Ѵ�.
int set_find(int curr) {
    if (parent[curr] == -1)
        return curr; // ��Ʈ
    while (parent[curr] != -1) curr = parent[curr];
    return curr;
}

// �� ���� ���Ұ� ���� ������ ��ģ��.
void set_union(int a, int b) {
    int root1 = set_find(a); // ��� a�� ��Ʈ�� ã�´�.
    int root2 = set_find(b); // ��� b�� ��Ʈ�� ã�´�.
    if (root1 != root2) // ���Ѵ�.
        parent[root1] = root2;
}

struct Edge { // ������ ��Ÿ���� ����ü
    int start, end, weight;
};

typedef struct GraphType {
    int n; // ������ ����
    struct Edge edges[2 * MAX_VERTICES];
} GraphType;

// �׷��� �ʱ�ȭ
void graph_init(GraphType* g) {
    g->n = 0;
}

// ���� ���� ����
void insert_edge(GraphType* g, int start, int end, int w) {
    g->edges[g->n].start = start;
    g->edges[g->n].end = end;
    g->edges[g->n].weight = w;
    g->n++;
}

// �׷��� ���� �Լ�
void GenerateGraph(GraphType* g) {
    insert_edge(g, 1, 7, 12);
    insert_edge(g, 7, 8, 13);
    insert_edge(g, 8, 10, 15);
    insert_edge(g, 10, 9, 10);
    insert_edge(g, 9, 5, 18);
    insert_edge(g, 5, 6, 9);
    insert_edge(g, 6, 1, 11);
    insert_edge(g, 1, 2, 3);
    insert_edge(g, 6, 2, 7);
    insert_edge(g, 2, 5, 1);
    insert_edge(g, 2, 7, 8);
    insert_edge(g, 2, 3, 5);
    insert_edge(g, 2, 4, 4);
    insert_edge(g, 3, 7, 6);
    insert_edge(g, 3, 8, 5);
    insert_edge(g, 3, 4, 2);
    insert_edge(g, 4, 8, 4);
    insert_edge(g, 4, 10, 16);
    insert_edge(g, 4, 5, 13);
    insert_edge(g, 5, 10, 17);
}

// qsort()�� ���Ǵ� �Լ�
int compare(const void* a, const void* b) {
    struct Edge* x = (struct Edge*)a;
    struct Edge* y = (struct Edge*)b;
    return (x->weight - y->weight);
}

// Quick ��� Kruskal �˰���
void QuickKruskal(GraphType* g) {
    set_init(g->n); // ���� �ʱ�ȭ
    qsort(g->edges, g->n, sizeof(struct Edge), compare);

    printf("Quick Based Kruskal\n");
    int edge_accepted = 0; // ������� ���õ� ������ ��

    for (int i = 0; i < g->n; i++) {
        int uset = set_find(g->edges[i].start); // ���� u�� ���� ��ȣ
        int vset = set_find(g->edges[i].end);   // ���� v�� ���� ��ȣ

        if (uset != vset) { // ���� ���� ������ �ٸ���
            printf("Edge (%d, %d) select %d \n", g->edges[i].start, g->edges[i].end, g->edges[i].weight);
            edge_accepted++;
            set_union(uset, vset); // �� ���� ������ ��ģ��.
        }

        if (edge_accepted == g->n - 1)
            break;
    }
}

// MinHeap�� ���� ����ü
typedef struct MinHeap {
    struct Edge heap[MAX_VERTICES];
    int size;
} MinHeap;

// MinHeap �ʱ�ȭ
void min_heap_init(MinHeap* h) {
    h->size = 0;
}

// MinHeap ���� �Լ�
void min_heap_insert(MinHeap* h, struct Edge e) {
    int i = h->size;
    while (i > 0 && h->heap[(i - 1) / 2].weight > e.weight) {
        h->heap[i] = h->heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h->heap[i] = e;
    h->size++;
}

// MinHeap���� �ּҰ� ���� �� ��ȯ
struct Edge min_heap_delete(MinHeap* h) {
    struct Edge min = h->heap[0];
    struct Edge last = h->heap[--h->size];
    int parent = 0, child = 1;
    while (child < h->size) {
        if (child + 1 < h->size && h->heap[child].weight > h->heap[child + 1].weight)
            child++;
        if (last.weight <= h->heap[child].weight) break;
        h->heap[parent] = h->heap[child];
        parent = child;
        child = 2 * parent + 1;
    }
    h->heap[parent] = last;
    return min;
}

// MinHeap�� ����� Kruskal �˰���
void MinHeapKruskal(GraphType* g) {
    MinHeap heap;
    min_heap_init(&heap);

    // ��� ������ MinHeap�� ����
    for (int i = 0; i < g->n; i++)
        min_heap_insert(&heap, g->edges[i]);

    set_init(g->n); // ���� �ʱ�ȭ
    int edge_accepted = 0; // ������� ���õ� ������ ��

    printf("MinHeap Based Kruskal\n");

    while (edge_accepted < (g->n - 1) && heap.size > 0) {
        struct Edge e = min_heap_delete(&heap);
        int uset = set_find(e.start); // ���� u�� ���� ��ȣ
        int vset = set_find(e.end);   // ���� v�� ���� ��ȣ
        if (uset != vset) {           // ���� ���� ������ �ٸ���
            printf("Edge (%d, %d) select %d \n", e.start, e.end, e.weight);
            edge_accepted++;
            set_union(uset, vset); // �� ���� ������ ��ģ��.
        }
    }
}

int main(void) {
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));
    graph_init(g);

    // �׷����� �����ϴ� �Լ�
    GenerateGraph(g);

    QuickKruskal(g); // Quick ��� Kruskal
    printf("\n");
    MinHeapKruskal(g); // MinHeap ��� Kruskal

    free(g);
    return 0;
}
