#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "h:li")) != -1) {
        switch (opt) {
        case 'h':
            printf("h %s\n", optarg);
            break;

        case ':':
            printf(": %s\n", optarg);
            break;


        case '?':
            printf("? %s\n", optarg);
            break;
        }
    }

    printf("after optind:%d\n", optind);
    for (int i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }


    return 0;
}
