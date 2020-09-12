#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct option longOptions[] = {
    {"help", no_argument, NULL, 'help'},
    {"demuxid", required_argument, NULL, 'dmid'},
    {0, 0, 0, 0},
};


int main(int argc, char *argv[])
{
    printf("hello getopt\n");    

    int optionChar = 0;
    int optionIndex = 0;
    while ((optionChar = getopt_long(argc, argv, "h", longOptions, &optionIndex)) != -1) {
        switch (optionChar) {
        case 'dmid':
            {
                printf("abc, optind:%d\n", optind);
                int demuxId = strtol(optarg, NULL, 0);
                printf("demuxId = %d\n", demuxId);
            }
            break;
        case 'help':
            printf("help -> usage:xxxxxxx!\n");
            break;
        case 'h':
            printf("h -> usage:xxxxxxx!\n");
            break;

        default:
            break;
        }
    }

    if (optionIndex < argc) {
        printf("last extra arg:%s\n", argv[optind]);
        printf("optind:%d, argc:%d\n", optind, argc);
    }

    return 0;
}

