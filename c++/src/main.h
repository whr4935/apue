#ifndef		_MAIN_H
#define		_MAIN_H

//////////////////////////////////////////////////////////////////////////
//misc
#define		TEST_MISC_EN				1
#define		TEST_EXPRESSION_EN			1
#define		TEST_FUNC_EN				0
#define		TEST_CLASS_EN				1
#define		TEST_TEMPLATE_EN			0
#define		TEST_STDLIB_EN				1

//////////////////////////////////////////////////////////////////////////
#include "misc/misc.h"
#include "expression/expression.h"
#include "func/func.h"
#include "class/class.h"
#include "template/template.h"
#include "std_lib/std_lib.h"

//////////////////////////////////////////////////////////////////////////
int(*g_funcArray[])(int argc, char **argv) = {
#if TEST_MISC_EN
    TEST_misc,
#endif

#if TEST_CLASS_EN
    TEST_class,
#endif

#if TEST_EXPRESSION_EN
    TEST_expression,
#endif

#if TEST_FUNC_EN
    TEST_func,
#endif

#if TEST_TEMPLATE_EN
    TEST_template,
#endif

#if TEST_STDLIB_EN
    TEST_stdlib,
#endif

    nullptr,
};

#endif
