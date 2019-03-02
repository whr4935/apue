#include "main.h"
#include <c_plus_plus_primer.h>

int main(int argc, char **argv)
{
    int i;
    int ret;

    initUtil();

    for (i = 0; i < sizeof(g_funcArray) / sizeof(*g_funcArray); ++i) {
        if (g_funcArray[i])
            (*g_funcArray[i])(argc, argv);
    }

    uninitUtil();

#ifdef NDEBUG
    getchar();
#endif

    return ret;
}
