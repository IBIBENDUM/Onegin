#ifndef SORTS_H
#define SORTS_H

void sort_lines(void* data, size_t size, size_t elem_size,
                int (*compare_func) (const void* a, const void* b));

void quick_sort(void* arr, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b));

#endif
