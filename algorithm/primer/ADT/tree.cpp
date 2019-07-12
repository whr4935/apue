#include <stdio.h>
#include <utils/utils.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "queue.h"
#include "stack.h"
#include "tree.h"

//////////////////////////////////////////////////////////////////////////
int64_t factorial(int N)
{
    int i;
    int64_t sum = 1;

    for (i = 2; i <= N; ++i) {
        sum *= i;
    }

    return sum;
}

int test_factorial()
{
    int64_t sum;
    int i;

    for (i = 0; i< 20; ++i) {
        sum = factorial(i);
        printf("N = %d, factorial = %ld\n", i, sum);
    }

    return 0;
}

int fibonacci(int N)
{
    int cache[2] = {0, 1};
    int i;

    for (i = 2; i <= N; ++i) {
        cache[i&1] = cache[0] + cache[1];
    }

    return cache[N&1];
}

void test_fibonacci()
{
    int result[20];
    int i;

    for (i = 0; i< 20; ++i) {
        result[i] = fibonacci(i);
    }

    printf("fibonacci:\n");
    for (i = 0; i < 20; ++i) {
        printf("%3d ", result[i]);
        if (i & (i+1)%10 == 0)
            printf("\n");
    }
    printf("\n");

}

#define RULE_LEN 8
static char rule_buf[RULE_LEN];

void mark(int m, int h)
{
    rule_buf[m] = '0' + h;
}

void rule(int l, int r, int h)
{
    int m = (l + r) / 2;
    if (h > 0) {
        mark(m, h);
        rule(l, m, h-1);
        rule(m, r, h-1);
    }
}

void test_rule()
{
    memset(rule_buf, '.', RULE_LEN);
    rule(0, RULE_LEN-1, 3);
    printf("%s\n", rule_buf);
}

//////////////////////////////////////////////////////////////////////////
void construct_tree(struct tree_node **root, const char* post_text)
{
    init_stack();
    struct tree_node *node;

    const char* tok = ".";
    char *p;
    p = strtok((char*)post_text, tok);
    while (p) {
//        printf("%s\n", p);
        node = (struct tree_node *)malloc(sizeof(struct tree_node));
        node->key = *p; node->l = NULL; node->r = NULL;
        if (*p=='+' || *p=='-' || *p=='*' || *p=='/') {
            node->r = (struct tree_node*) pop();
            node->l = (struct tree_node*) pop();
        }
        push((long)node);
        p = strtok(NULL, tok);
    }

    *root = (struct tree_node*) pop();
}

struct tree_node* test_contruct_tree()
{
    struct tree_node *root = NULL;
    static char* text = "1*5-2+(2-(4+3-9))";
    char post_text[100];

    infix2postfix(text, post_text);
    printf("post_text:%s\n", post_text);
    construct_tree(&root, post_text);
    return root;
}

void  pre_traverse_routine(struct tree_node *node)
{
    if (node) {
        printf("%2c ", node->key);
        pre_traverse_routine(node->l);
        pre_traverse_routine(node->r);
    }
}

void pre_traverse(struct tree_node *node)
{
    printf("%s:", __FUNCTION__);
    pre_traverse_routine(node);
    printf("\n");
}

void in_traverse_routine(struct tree_node *node)
{
    if (node) {
        in_traverse_routine(node->l);
        printf("%2c ", node->key);
        in_traverse_routine(node->r);
    }
}

void in_traverse(struct tree_node *node)
{
    printf("%s:", __FUNCTION__);
    in_traverse_routine(node);
    printf("\n");
}

void post_traverse_routine(struct tree_node *node)
{
    if (node) {
        post_traverse_routine(node->l);
        post_traverse_routine(node->r);
        printf("%2c ", node->key);
    }
}

void post_traverse(struct tree_node *node)
{
    printf("%s:", __FUNCTION__);
    post_traverse_routine(node);
    printf("\n");
}


void print_tree(struct tree_node *node)
{
    int mid_pos = 50;
    int step = 1;
    int depth = 0;
    char buf[2][100];

    memset(buf, ' ', 200);
    init_queue();

    node->pos = mid_pos;
    node->depth = 0;
    put((long)node);

    while (!queue_empty()) {
        get(&node);

        if (node->depth != depth) {
            buf[0][99] = 0; buf[1][99] = 0;
            puts(buf[0]);
            puts(buf[1]);
            memset(buf, ' ', 200);
        }
        depth = node->depth;

        mid_pos = node->pos;
        buf[0][mid_pos] = node->key;

        if (node->l) {
            buf[1][mid_pos - step] = '/';
            node->l->pos = mid_pos -  2 * step;
            node->l->depth = node->depth+1;
            put((long)node->l);
        }

        if (node->r) {
            buf[1][mid_pos + step] = '\\';
            node->r->pos = mid_pos + 2 * step;
            node->r->depth = node->depth+1;
            put((long)node->r);

        }
    }

    if (depth > 0) {
        puts(buf[0]);
        puts(buf[1]);
    }
}


int test_tree()
{
//    test_factorial();
//    test_fibonacci();
//    test_rule();

    //////////////////////////////
    struct tree_node *root = NULL;
    root = test_contruct_tree();
    assert(root != NULL);
    post_traverse(root);
    print_tree(root);

//    test_print_tree();

    return 0;
}
