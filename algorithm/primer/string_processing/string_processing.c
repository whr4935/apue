#include <includes.h>

#include "string_searching.h"
#include "huffman_code.h"

int test_string_processing(int argc, char **argv)
{
    argc = argc, argv = argv;

    BEGIN_MODULE;

//    test_string_searching();

    test_huffman_code();

    END_MODULE;
    return 0;
}
