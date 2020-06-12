#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <queue>
#include <stack>
#include <utils/utils_main/utils_main.h>

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

static inline char index2V(int idx)
{
    return idx + 'A';
}


////////////////////////////////
int** create_adjacency_matrix(const char* graph, int len) 
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
            a[i][j] = i != j ? 0 : 1;
        }
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

int test_adjacency_matrix()
{
    const char* graph = "4 4 AB AC CD DB";
    int** a = create_adjacency_matrix(graph, strlen(graph));
    print_adjacency_matrix(a);
    destroy_adjacency_matrix(a);

    return 0;
}

AUTO_RUN(test_adjacency_matrix)


////////////////////////////////
void create_adjacency_list(const char *graph, int len)
{
    const char *p = graph;
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

int test_adjacency_list()
{
    const char *graph = "4 4 AB CD AC BD";

    create_adjacency_list(graph, strlen(graph));

    print_adjacency_list();

    destroy_adjacency_list();

    return 0;
}

AUTO_RUN(test_adjacency_list)

///////////////////////////////////
static void dfs_visit_matrix(int**a, int i, int* val)
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

static void dfs_ajaency_matrix(int** a)
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

static void dfs_visit_matrix_non_recursive(int** a, int i, int val[], std::stack<int>& s)
{
    s.emplace(i);
    while (!s.empty()) {
        i = s.top();
        s.pop();

        printf("%2c ", index2V(i));
        val[i] = 1;

        for (int j = 0; j < V; ++j) {
            if (a[i][j] == 1 && val[j] == 0) {
                s.push(j);
                val[j] = -1;
            }
        }
    }

}

static void dfs_adjacency_matrix_non_recursive(int** a)
{
    int i = 0;
    int val[MAX_V] = {0};

    printf("DFS matrix non recursive:\n");
    std::stack<int> s;

    for (i = 0; i < V; ++i) {
        if (val[i] == 0) {
            dfs_visit_matrix_non_recursive(a, i, val, s);
        }
    }

    printf("\n");
}

int test_dfs_ajacency_matrix()
{
    const char* graph = "7 8 AG AB AC AF ED FD FE GE";
    int** a = create_adjacency_matrix(graph, strlen(graph));
    print_adjacency_matrix(a);
    dfs_ajaency_matrix(a);
    dfs_adjacency_matrix_non_recursive(a);
    destroy_adjacency_matrix(a);

    return 0;
}
AUTO_RUN(test_dfs_ajacency_matrix)

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

    printf("DFS adjacency list:\n");
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

static void visit_adjacency_list_non_recursive(int idx, int val[], std::stack<int>& s)
{
    struct node*t = adj[idx]; 
    s.push(idx);

    while (!s.empty()) {
        idx = s.top();
        s.pop();

        printf("%2c ", index2V(idx));
        val[idx] = 1;

        for (t = adj[idx]; t != z; t = t->next) {
            if (val[t->v] == 0) {
                s.push(t->v);
                val[t->v] = -1;
            }
        }
    }
}

static void dfs_adjancency_list_non_recursive()
{
    int i;
    int val[MAX_V];
    std::stack<int> s;

    for (i = 0; i < MAX_V; ++i)
        val[i] = 0;

    printf("DFS adjacency list non recursive:\n");
    for (i = 0; i < V; ++i) {
        if (val[i] == 0) {
            visit_adjacency_list_non_recursive(i, val, s);
        }
    }

    printf("\n");
}

int test_dfs_adjacency_list()
{
    const char* graph = "7 8 AG AB AC AF ED FD FE GE";

    create_adjacency_list(graph, strlen(graph));

    print_adjacency_list();

    dfs_adjacency_list();

    dfs_adjancency_list_non_recursive();

    destroy_adjacency_list();

    return 0;
}
AUTO_RUN(test_dfs_adjacency_list);

/////////////////////////////////////
void bfs_visit_list(int i, int* val, std::queue<int>& q)
{
    struct node* k;

    q.push(i);
    while (!q.empty()) {
        i = q.front();
        q.pop();
        val[i] = 1;
        printf("%2c ", index2V(i));

        for (k = adj[i]; k != z; k = k->next) {
            if (val[k->v] == 0) {
                q.push(k->v);
                val[k->v] = -1;
            }
        }
    }
}

void bfs_adjacency_list()
{
    int i;
    int val[MAX_V] = {0};
    std::queue<int> q;

    for (i = 0; i < V; ++i) {
        if (val[i] == 0)
            bfs_visit_list(i, val, q);
    }

    printf("\n");
}

void test_bfs_adjacency_list()
{
    const char* graph = "5 5 BE AB AC BA CD";

    create_adjacency_list(graph, strlen(graph));
    bfs_adjacency_list();
    destroy_adjacency_list();
}





