#include <stdio.h>
#include <unwind.h>
#include <stdint.h>
#include <execinfo.h>
#include <stdlib.h>

static _Unwind_Reason_Code unwind_backtrace_callback(struct _Unwind_Context* context, void *arg)
{
    uintptr_t pc = _Unwind_GetIP(context);

    if (pc) {
        printf("unwind got pc ... %#x\n", pc);
    }

    return _URC_NO_REASON;
}

ssize_t unwind_backtrace()
{
    _Unwind_Reason_Code rc = _Unwind_Backtrace(unwind_backtrace_callback, 0);

    return rc == _URC_END_OF_STACK ? 0 : -1;
}


int main(int argc, char *argv[])
{

    unwind_backtrace();

    void* array[10];
    int size = backtrace(array, 10);

    printf("Obtained %d stack frames.\n", size);

    for (size_t i = 0; i < size; ++i) {
        printf("%#lx\n", array[i]);
    }

    char** strings;

    strings = backtrace_symbols(array, size);
    for (size_t i = 0; i < size; ++i) {
        printf("%s\n", strings[i]);
    }

    //only the return value need be freed, not the individual strings
    free((void*)strings);

    return 0;
}
