#include "misc.h"
#include <stddef.h>
#include <ADT/list.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

///////////////////////////////////////
int test_josephus_loop();

///////////////////////////////////////

static int test_misc()
{
    int d = 5;
    void *p_d = &d;
    void **pp_str = &p_d;
    void ***ppp_str = &pp_str;

    int d1 = 3;

    *pp_str = &d1;
    *(int*)**ppp_str = 8;

    printf("d1 = %d\n", d1);
    printf("pp_str: %d\n", *(int*)*pp_str);


#if 0
#pragma  pack(push, 4)
    struct aa {
        short a;
        char c;
        double d;
        char c2;
        float f;
        //
    };

#pragma pack(pop)

#pragma  pack(8)
    struct bb {
        char m;
        struct aa aa;
        //      char c;
        //      int d;
        //      char c2;
    };

    printf("sizeof double = %d\n", sizeof(double));
    printf("sizeof float = %d\n", sizeof(float));
    int size_a = sizeof(struct aa);
    struct bb _bb;
    int so_aa = offsetof(struct bb, aa);

    int size_b = sizeof(struct bb);

    //int so_d = offsetof(struct bb, d);
#endif
    int max_int = (int)(((unsigned int)~0) >> 1);

    max_int++;

    char *p[] = { "a", "b", "c" };
    int ia[] = { 1, 2, 3 };

    char *(*pp)[3] = &p;
    //	char **pp2 = &p;
    int i;
    int array1[3][2] = {
        {
            1, 2
        },
        {
            3, 4
        },
        {
            5, 6
        }
    };

    //	int *pArray1 = array1;
    int v;

    int(*pia)[3] = &ia;
    char pt_str[] = "abc:def:ghi:jkl:mn:opqrst:uvwxyz";

    char *my_str = strtok(pt_str, ":");
    char c;
    int c_count = 0;

    pp++;
    //	pp2++;



    for (i = 0; i < 100; ++i) {
        printf("hello %d\r", i);
    }
    printf("\n");



    v = *(array1[0] + 1);


    printf(my_str);
    putchar('\n');

    while (my_str) {
        my_str = strtok(NULL, ":");
        if (my_str == NULL)
            break;
        printf(my_str);
        putchar('\n');
    }




    //  while ((c = getchar()) != EOF) {
    //      c_count++;
    //      putchar(c);
    //  }

}


int test_reverseLinkList()
{
    struct List *L;
    struct List *node;
    struct List **ppNode;
    struct List *prev;
    struct List *next;
    int i;

    initList(&L);
    for (i = 0; i < 10; ++i) {
        node = mallocListNode();
        initListNode(node, i, i + 1);
        listAddNodeTail(&L, node);
    }

    node = L;
    while (node) {
        printf("%d ", node->pdata);
        node = node->next;
    }
    printf("\n");

    prev = NULL;
    node = L;
    while (node) {
        next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }
    L = prev;

    node = L;
    while (node) {
        printf("%d ", node->pdata);
        node = node->next;
    }
    printf("\n");

    while (L) {
        ListDeleteNode(&L, L);
    }
    assert(L == NULL);
}


static int test_number_convert()
{
    short x = -12345;
    unsigned short ux = x;
    int ix = ux;
    int ix2 = x;
    unsigned uix = x;
    unsigned uix2 = ux;

    printf("ix = %d, uix = %u\n", ix, uix);

    unsigned char i = 1, j = 255;
    unsigned char num = i - j;

    return 0;
}

static int test_roundup_power_two()
{
    int val = rand();
    int val2;

    if ((val & val-1) == 0) {
        printf("val is two's power!\n", val);
        return 0;
    }

    unsigned max_int = (unsigned)(~0);
    unsigned mask = max_int & max_int>>1;
    mask = ~mask;
    while ((mask & val) == 0) {
        mask >>= 1;
    }
    val2 = mask << 1;
    printf("val: %d, round up two's power is:%d", val, val2);

    return 0;
}

static test_read_write()
{

    int fd;
    int ret;

    fd = open("a.txt", O_RDWR|O_CREAT, 0666);
    if (fd < 0) {
        printf("open a.txt failed!\n" );
        return -1;
    }

    ret = write(fd, "12345", 5);
    if (ret < 0) {
        printf("write failed:%s\n", strerror(errno));
        return -1;
    }
    ret = lseek(fd, 1, SEEK_SET);
    if (ret < 0) {
        printf("lseek failed:%s\n", strerror(errno));
        return -1;
    }
    ret = write(fd, "89", 2);
    if (ret < 0) {
        printf("write failed:%s\n", strerror(errno));
        return -1;
    }
    ret = close(fd);
    if (ret < 0) {
        printf("close failed:%s\n", strerror(errno));
        return -1;
    }

    return 0;
}

int TEST_misc()
{
    /* test_misc(); */

    /* TEST_gcd(); */
    /* TEST_subsequence(); */
    /* TEST_infix2postfix(); */

    /* test_reverseLinkList(); */

    /* test_number_convert(); */
    /* test_roundup_power_two(); */
    /* test_josephus_loop(); */

    return 0;
}
