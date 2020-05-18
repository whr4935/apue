#include "elementary_graph.h"
#include <malloc.h>
#include <string.h>
#include <ADT/queue.h>
#include <ctype.h>

////////////////////////////////////
struct node {
    int v;
    struct node *next;
};

#define MAX_V 20

struct node* adj[MAX_V], *z;
int V, E = 0;

static inline int index(int v)
{
    return v - 'A';
}

static inline int index2V(int idx)
{
    return idx + 'A';
}


////////////////////////////////
int** create_adjacency_matrix(char* graph, int len) 
{
    int i, j, x, y;
    char v1, v2;
    int pos = 0;

    pos = sscanf(graph, "%d %d", &V, &E);
    graph += pos*2;

    int** a = (int**)malloc(V*sizeof(int*));
    for (i = 0; i < V; ++i) {
        a[i] = (int*)malloc(V*sizeof(int));
    }

    for (int i = 0; i < V; ++i) {
        for (j = 0; j < V; ++j) {
            a[i][j] = 0;
        }
    }

    for (i = 0; i < V; ++i) {
        a[i][i] = 1;
    }

    for (i = 0; i < E; ++i) {
       pos = sscanf(graph, "%c%c", &v1, &v2); 
       graph += pos+1;
       x = index(v1), y = index(v2);
       a[x][y] = 1;
       a[y][x] =1;

    }

    return a;
}

void destroy_adjacency_matrix(int** a)
{
    for (int i = 0; i < V; ++i) {
        free(a[i]);
    }
    free(a);
}

void print_adjacency_matrix(int** a)
{
    int i, j;
    for (i = 0; i < V; ++i) {
        for (j = i+1; j < V; ++j) {
            if (a[i][j]) {
                printf("%c%c ", index2V(i), index2V(j));
            }
        }
    }

    printf("\n");
}

void test_adjacency_matrix()
{
    char* graph = "5 4 AB AC CD DB";
    int** a = create_adjacency_matrix(graph, strlen(graph));
    print_adjacency_matrix(a);
    destroy_adjacency_matrix(a);
}


////////////////////////////////
void create_adjacency_list(char *graph, int len)
{
    char *p = graph;
    int i;
    char v1, v2;
    int x, y;
    struct node *t;

    z = (struct node*)malloc(sizeof(*z));
    z->next = z;

    sscanf(p, "%d %d", &V, &E);
    while (!isalpha(*p))
        ++p;

    for (i = 0; i < V; ++i) {
        adj[i] = z;
    }

    for (int j = 0; j < E; ++j) {
        sscanf(p, "%c%c", &v1, &v2);
        p += 3;
        x = index(v1), y = index(v2);
        t = (struct node*)malloc(sizeof(*t));
        t->v = x;
        t->next = adj[y];
        adj[y] = t;

        t = (struct node*)malloc(sizeof(*t));
        t->v = y;
        t->next = adj[x];
        adj[x] = t;
    }
}

void destroy_adjacency_list()
{
    int i;
    struct node *t;

    for (i = 0; i < V; ++i) {
        while (adj[i] != z) {
            t = adj[i]->next;
            free(adj[i]);
            adj[i] = t;
        }
    }
}

void print_adjacency_list()
{
    int i;
    int v;
    struct node *t;

    for (i = 0; i < V; ++i) {
        v = index2V(i);
        printf("%c:", v);

        t = adj[i];
        while (t != z) {
            v = index2V(t->v);
            printf("%2c", v);
            t = t->next;
        }
        printf("\n");
    }
}

void test_adjacency_list()
{
    char *graph = "4 4 AB CD AC BD";

    create_adjacency_list(graph, strlen(graph));

    print_adjacency_list();

    destroy_adjacency_list();
}

///////////////////////////////
static int v_id = 0;

static int dfs_visit_list(int idx, int *val, int *begin)
{
    struct node *t;
    int m, min;
    val[idx] = ++v_id;
    min = v_id;

    if (*begin) {
        *begin = 0;
    } else {
        printf("->");
    }
    printf("%2c ", index2V(idx));
    for (t = adj[idx]; t != z; t = t->next) {
        if (val[t->v] == 0) {
            m = dfs_visit_list(t->v, val, begin);
            if (m < min)
                min = m;
            if (m >= val[idx])
                printf(" ##-%c ", index2V(idx));
        } else if (val[t->v] < min)
            min = val[t->v];
    }

    return min;
}

void dfs_adjacency_list()
{
    int i;
    int val[MAX_V];
    int id;
    int begin;


    for (i = 0; i < MAX_V; ++i)
        val[i] = 0;

    printf("DFS:\n\n");
    v_id = 0;
    for (i = 0; i < V; ++i) {
        if (val[i] == 0) {
            begin = 1;
            dfs_visit_list(i, val, &begin);
            printf("\n");
        }
    }

    v_id = 0;
    printf("\n");
}

void test_dfs_adjacency_list()
{
    char* graph = "13 13 AG AB AC LM JM JL JK ED FD HI FE AF GE";

    create_adjacency_list(graph, strlen(graph));

    print_adjacency_list();

    dfs_adjacency_list();

    destroy_adjacency_list();
}

///////////////////////////////////
void dfs_visit_matrix(int**a, int i, int* val)
{
    int j;

    printf("%2c ", index2V(i));
    val[i] = 1;

    for (j = 0; j < V; ++j) {
        if (val[j]==0 && a[i][j]) {
            dfs_visit_matrix(a, j, val);
        }
    }
}

void dfs_ajaency_matrix(int** a)
{
    int i, j;
    int val[MAX_V] = {0};

    printf("DFS matrix:\n");
    for (i = 0; i < V; ++i) {
        if (val[i] == 0)
            dfs_visit_matrix(a, i, val); 
    }

    printf("\n");
}

void test_dfs_ajacency_matrix()
{
    char* graph = "5 5 BE AB AC BA CD";
    int** a = create_adjacency_matrix(graph, strlen(graph));
    print_adjacency_matrix(a);
    dfs_ajaency_matrix(a);
    destroy_adjacency_matrix(a);
}

/////////////////////////////////////
void bfs_visit_list(int i, int* val)
{
    struct node* k;

    alg::put(i);
    while (alg::get((long*)&i)) {
        val[i] = 1;
        printf("%2c ", index2V(i));

        for (k = adj[i]; k != z; k = k->next) {
            if (val[k->v] == 0) {
                alg::put(k->v);
                val[k->v] = -1;
            }
        }
    }
}

void bfs_adjacency_list()
{
    int i;
    int val[MAX_V] = {0};

    alg::init_queue();
    for (i = 0; i < V; ++i) {
        if (val[i] == 0)
            bfs_visit_list(i, val);
    }

    printf("\n");
}

void test_bfs_adjacency_list()
{
    char* graph = "5 5 BE AB AC BA CD";

    create_adjacency_list(graph, strlen(graph));
    bfs_adjacency_list();
    destroy_adjacency_list();
}


/////////////////////////////////////
void test_elementary_graph()
{
    /* test_adjacency_matrix(); */
    /* test_adjacency_list(); */
    test_dfs_adjacency_list();
    /* test_dfs_ajacency_matrix(); */
    /* test_bfs_adjacency_list(); */
}








