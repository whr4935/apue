#include <stdio.h>
#include <utils/utils.h>

#define N 100

static int stack[N];
static int sp;

int init_stack()
{
    int i;

    sp = 0;
    for (i = 0; i < N; ++i) {
        stack[i] = 0;
    }
}

void push(int v)
{
    stack[sp++] = v;
}

int pop()
{
    return stack[--sp];
}

bool stack_empty()
{
    return sp <= 0;
}

#define SPACE '.'

static char preference_table[4][3] = {
    {0},
    {'+', '-'},
    {'*', '/'}
};

static int get_preference(int c)
{
    int i;


    for (i = 3; i < 4 * 3; ++i) {
        if (c == *((char*)preference_table +i)) {
            break;
        }
    }

    if (i != 3 *4 ) {
        return i/3;
    } else {
        return 0;
    }
}


static int check_preference(int c)
{
    int t;

    if (stack_empty())
        return false;

    t = stack[sp-1];

#if 0
    if (c == '+' || c == '-') {
        if (t == '+' || t == '-' || t == '*')
            return true;
    }

    if (c == '*') {
        if (t == '*')
            return true;
    }
#else

    if (t == '(')
        return false;

    int l = get_preference(t);
    int r = get_preference(c);
    if (l >= r)
        return true;
#endif

    return false;
}

/*
 * 注意两点：
 * 1. 遇到‘)’要出栈，直到遇到'('字符。
 * 2. 遇到运算符时，要跟堆栈里的运算符比较优先级，直到堆栈里的运算符优先级比当前运算符优先级低。
 */
void infix2postfix(const char* text, char* post_text)
{
    char c;
    const char* p = text;

    init_stack();

    while (c = *p++) {
        if (c >= '0' && c <= '9') {
            *post_text++ = c;
            continue;
        }

        *post_text++ = SPACE;

        if (c == ')') {
            while ((c = pop()) != '(') {
                *post_text++ = c;
                *post_text++ = SPACE;
            }
            continue;
        }

        if (c == '(') {
            push(c);
        }

        if (c == '+' || c == '-' || c == '*' || c == '/' ) {
            while (check_preference(c)) {
                *post_text++ = pop();
                *post_text++ = SPACE;
            }
            push(c);
        }
    }

    *post_text++ = SPACE;
    while (!stack_empty()) {
        *post_text++ = pop();
        *post_text++ = SPACE;
    }
    *post_text = '\0';
}


int test_infix2postfix()
{
    static char* text = "11*5-21+(2-(4+3-9))";
    char post_text[100];

    infix2postfix(text, post_text);

    printf("infix: %s\n", text);
    printf("postfix: %s\n", post_text);

    return 0;
}

static int calc_postfix(const char *post_text)
{
    char c;
    int num = 0, num1, num2;
    char bNum = false;

    init_stack();

    while (c = *post_text++) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + c - '0';

            bNum = true;
            continue;
        }

        if (bNum) {
            bNum = false;
            push(num);
            num = 0;
        }

        if (c == '+' || c == '*' || c == '-' || c == '/') {
            num2 = pop();
            num1 = pop();

            switch (c) {
            case '+':
                push(num1 + num2);
                break;

            case '-':
                push(num1 - num2);
                break;

            case '*':
                push(num1 * num2);
                break;

            case '/':
                push(num1 / num2);
                break;

            default:
                printf("we should not reach here: %c\n", c);
                break;
            }
        }
    }

    return pop();
}

static void test_calc_postfix()
{
    static char* text = "(1+2)*50/(4-8)";
    char post_text[100];
    int result = -1;

    infix2postfix(text, post_text);

    printf("postfix:%s\n", post_text);
    result = calc_postfix(post_text);
    printf("result: %d\n", result);
}




int test_stack()
{
  //  test_infix2postfix();

    test_calc_postfix();

    return 0;
}












