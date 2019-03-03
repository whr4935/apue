#ifndef		_MISC_H
#define		_MISC_H

#include <algorithm.h>


//////////////////////////////////////////////////////////////////////////
BOOL isOperator(char c);
STATUS infix2postfix(char *in, char *output);

//////////////////////////////////////////////////////////////////////////
int TEST_gcd();
int TEST_subsequence();
int TEST_infix2postfix();

#endif
