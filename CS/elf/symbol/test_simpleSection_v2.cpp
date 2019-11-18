#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char* out_dir = getenv("OUT_DIR");
    const char* so_name = "/lib/libsymbol_v2.so";
    char path[100];
    strcpy(path, out_dir);
    strcat(path, so_name);
    void* handle = dlopen(path, RTLD_LAZY);
    if (handle == nullptr) {
        printf("dlopen %s failed, %s\n", path, dlerror());
        exit(-1);
    }

    const char* symbols[] = {
    "whr_func1",
    "whr_func2"
    };


    for (size_t i = 0; i < sizeof(symbols)/sizeof(*symbols); ++i) {
        void* p = dlsym(handle, symbols[i]);
        if (p == nullptr) {
            printf("dlsym %s failed, %s\n", symbols[i], dlerror());
            continue;
        }

        printf("dlsym %s success, %p\n", symbols[i], p);
    }


    dlclose(handle);

    return 0;
}
