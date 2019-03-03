/*************************************************************************
 > File Name: main.c
 > Created Time: 2017年11月10日 星期五 13时30分56秒
 ************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "config_utils.h"

void *load_file(const char *fn, unsigned *_sz)
{
    char *data;
    int sz;
    int fd;

    data = 0;
    fd = open(fn, O_RDONLY);
    if(fd < 0) return 0;

    sz = lseek(fd, 0, SEEK_END);
    if(sz < 0) goto oops;

    if(lseek(fd, 0, SEEK_SET) != 0) goto oops;

    data = (char*) malloc(sz + 1);
    if(data == 0) goto oops;

    if(read(fd, data, sz) != sz) goto oops;
    close(fd);
    data[sz] = 0;

    if(_sz) *_sz = sz;
    return data;

oops:
    close(fd);
    if(data != 0) free(data);
    return 0;
}


int level = 1;
void dump(cnode* root)
{
    printf("%*c%s  %s\n", level*2,' ', root->name, root->value);

    ++level;
    cnode*p = root->first_child;
    if (p != NULL) {
        dump(p);
    }

    --level;
    if (root->next) {
        dump(root->next);
    }
}

int main()
{
    const char* path = "audio_policy.conf";

    cnode *root;
    char *data;

    data = (char *)load_file(path, NULL);
    if (data == NULL) {
        printf("load file failed!\n");
        return -1;
    }
    root = config_node("", "");
    config_load(root, data);

    dump(root->first_child);


    return 0;
}

