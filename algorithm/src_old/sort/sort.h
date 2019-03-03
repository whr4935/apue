#ifndef _SORT_H
#define _SORT_H

#include <algorithm.h>

//////////////////////////////////////////////////////////////////////////
int insert_sort(element_type *e, int len);
int select_sort(element_type *e, int size);
int bubble_sort(element_type *e, int size);
int merge_sort(element_type *e, int size);
int shell_sort(element_type *e, int size);
int quick_sort(element_type *e, int size);

#endif