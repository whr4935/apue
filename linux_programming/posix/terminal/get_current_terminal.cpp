#include <termio.h>
#include <stdio.h>
#include <unistd.h>

void hexdump(char buf[], size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    struct termios term, old_term;

    if (tcgetattr(STDIN_FILENO, &term) < 0) {
        printf("tcgetattr failed!\n");
        return 0;
    }

    old_term = term;


    printf("c_cflag:%#x\n", term.c_cflag);

    printf("%s\n", ctermid(nullptr));
    printf("isatty:%d\n", isatty(STDIN_FILENO));
    printf("isatty:%d\n", isatty(STDOUT_FILENO));
    printf("isatty:%d\n", isatty(STDERR_FILENO));


    //只要改成noncanonical mode，并且把回显关闭，就可以手动控制打印某些用户输入的字符
    //可以特殊处理tab key，实现自动补全
    term.c_lflag &= ~(ECHO | ICANON );
    int ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
    if (ret < 0) {
        printf("tcsetattr failed with %d\n", ret);
    }



    char buf[100]{};
    char* ptr = buf;
    char c;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (ptr < buf + sizeof(buf) -1) {
            if (c == '\t') {
                //printf("complete abc\n");
            } else {
                *ptr++ = c;
                printf("%c", c);
            }
        }
    }
    *ptr = '\0';

    printf("size:%d, result:%s\n", ptr - buf, buf);
    hexdump(buf, ptr-buf);

    term.c_lflag |= (ECHO | ICANON );
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

    return 0;
}
