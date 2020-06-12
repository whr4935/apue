#ifndef _UTILS_MAIN_H
#define _UTILS_MAIN_H

#include <string>
#include <functional>

bool registerRoutine(std::string name, std::function<int()> routine);

#define TO_STRING(x) TO_STRING2(x)
#define TO_STRING2(x) #x

#define AUTO_RUN(name) \
    AUTO_REGISTER_ROUTINE(TO_STRING(name), name)

#define AUTO_REGISTER_ROUTINE(name, routine) \
    namespace { \
        struct routine ## _{ \
            routine ## _() { \
                registerRoutine(name, routine); \
            } \
        } routine ##_; \
    } 

#endif
