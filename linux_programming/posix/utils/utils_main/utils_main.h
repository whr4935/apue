#ifndef _UTILS_MAIN_H
#define _UTILS_MAIN_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

int registerRoutine(const char* name, int (*routine)());
int registerRoutine2(const char* name, int (*routine)(int argc, char* argv[]));

#ifndef TO_STRING
#define TO_STRING(x) TO_STRING2(x)
#define TO_STRING2(x) #x
#endif

#define AUTO_RUN(routine) \
    AUTO_REGISTER_ROUTINE(TO_STRING(routine), routine)

#define AUTO_REGISTER_ROUTINE(name, routine) \
    __attribute__((constructor)) \
    static int routine##_constructor() { \
        return registerRoutine(name, routine); \
    }

#define AUTO_RUN2(routine) \
    AUTO_REGISTER_ROUTINE2(TO_STRING(routine), routine)

#define AUTO_REGISTER_ROUTINE2(name, routine) \
    __attribute__((constructor)) \
    static int routine##_constructor2() { \
        return registerRoutine2(name, routine); \
    }














#ifdef __cplusplus
}
#endif












#endif
