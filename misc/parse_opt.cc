#include <stdio.h>

void sort_opts(char** argv, int parsed, int len, int& processed)
{
    char* cur = nullptr;

    for (size_t i = 0; i < len; ++i, ++parsed) {
        cur = argv[parsed];  

        for (int j = parsed; j > processed; --j) {
            argv[j] = argv[j-1];
        }
        argv[processed++] = cur;
    }
}

int parse_opts(int argc, char** argv)
{
    int processed = 0;
    int parsed = 0;

    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'c':
                if (i < argc-1) {
                    printf("parsed opt \'c\', value=%s\n", argv[++i]);

                    sort_opts(argv, i-1, 2, processed);
                }
                break;

            case 'h':
                sort_opts(argv, i, 1, processed);
                break;

            default:
                break;
            }
        }
    }

    return processed;
}


int main(int argc, char *argv[])
{
    char* opts[] = {"-a", "a1", "-b", "b2", "-h", "file.txt", "-c", "c3"};

    int count = sizeof(opts)/sizeof(*opts);

    int processed = parse_opts(count, opts);

    printf("processed:%d, dump opts:\n", processed);
    for (size_t i = 0; i < count; ++i) {
        printf("opt[%d] = %s\n", i, opts[i]);
    }

    return 0;
}
