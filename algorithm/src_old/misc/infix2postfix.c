#include <algorithm.h>
#include <ADT/stack.h>
#include <ADT/queue.h>
#include <ADT/tree.h>


STATUS checkBalance(char *pstr)
{
    struct Stack *S = NULL;
    char *p = NULL;
    element_type data;

    p = pstr;
    while (*p) {
        if (*p == '(' || *p == '[' || *p == '{') {
            push(&S, *p);
        } else if (*p == ')' || *p == ']' || *p == '}') {
            pop(&S, &data);
            if ((*p == ')' && data == '(') ||
                (*p == ']' && data == '[') ||
                (*p == '}' && data == '{')) {

            } else {
                printf("error: unmatch!\n");
                return FAILURE;
            }
        }

        ++p;
    }

    if (S != NULL) {
        printf("error: unmatch!\n");
        return FAILURE;
    }

    return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
char op_table[][4] = {
    { '+', '-' },
    { '*', '/', '%' },
};

int getPriority(char c)
{
    int i;
    int priority = 0;

    if (c == 0)
        return c;

    for (priority = 0; priority < sizeof(op_table) / sizeof(*op_table); ++priority) {
        for (i = 0; i < sizeof(op_table[0]) / sizeof(*op_table[0]); ++i) {
            if (c == op_table[priority][i])
                return priority + 1;
        }
    }

    return 0;
}

int cmpPriority(char p1, char p2)
{
    return 0;
}

BOOL isOperator(char c)
{
    int op_count;
    int i;

    op_count = sizeof(op_table) / sizeof(char);

    for (i = 0; i < op_count; ++i) {
        if (c == ((char*)op_table)[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static inline void out_c(char c, char **out)
{
    *(*out)++ = ' ';
    *(*out)++ = c;
    *(*out)++ = ' ';
}

STATUS infix2postfix(char *in, char *output)
{
    struct Stack *S = NULL;
    char *p = in;
    element_type data;

    while (*p) {
        if (isdigit(*p)) {
            //	out_c(*p, &output);
            *output++ = *p;
        } else if (*p == '(') {
            push(&S, *p);
        } else if (*p == ')') {
            *output = ' ';
            while (1) {
                pop(&S, &data);
                if (data && data != '(') {
                    out_c(data, &output);
                } else {
                    break;
                }
            }
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            while (1) {
                if ((top(S, &data) == SUCCESS) && (getPriority(*p) <= getPriority(data))) {
                    pop(&S, &data);

                    if (*p != '(' && *p != ')')
                        out_c(data, &output);
                } else {
                    *output++ = ' ';
                    push(&S, *p);
                    break;
                }
            }
        } else if (*p == ' ' || *p == '\t') {
            out_c(*p, &output);
        }

        ++p;
    }

    for (;;) {
        if (top(S, &data) != SUCCESS)
            break;

        pop(&S, &data);
        if (data != '(' && data != ')')
            out_c(data, &output);
    }

    return SUCCESS;
}

int calcPostfix(char *str)
{
    struct Stack *S = NULL;
    char *p = str;
    element_type data = 0;
    element_type op1, op2;

    while (*p) {
        if (isdigit(*p)) {
            data = strtol(p, &p, 10);
            push(&S, data);
            continue;

        } else if (getPriority(*p)) {
            pop(&S, &op2);
            pop(&S, &op1);

            switch (*p) {
            case '+':
                data = op1 + op2;
                break;

            case '-':
                data = op1 - op2;
                break;

            case '*':
                data = op1 * op2;
                break;

            case '/':
                data = op1 / op2;
                break;

            case '%':
                data = op1 % op2;
                break;

            default:
                printf("unknow operator!\n");
                break;
            }

            push(&S, data);
        }

        ++p;
    }

    return data;
}

//////////////////////////////////////////////////////////////////////////
int TEST_infix2postfix()
{
    element_type data;

    char *string = "{fdffldfsdfds()}fdfssd[ff...]";
    if (checkBalance(string) == SUCCESS) {
        printf("success, balance!\n");
    }

    char *infix_string = "12 + 2 * 3 + (4*5+6)*1";
    char out_string[100] = "";

    infix2postfix(infix_string, out_string);
    printf("infix string:   %s\n", infix_string);
    printf("postfix string: %s\n", out_string);

    data = calcPostfix(out_string);
    printf("postfix result: %d\n", data);

    return 0;
}
