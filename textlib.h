#ifndef TEXTLIB_H
#define TEXTLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

size_t get_file_size(FILE* file_ptr);

size_t get_file_size(FILE* file_ptr)
{
    // fseek + ftell realization
    size_t prev_pos = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_END);
    size_t size = ftell(file_ptr);
    fseek(file_ptr, prev_pos, SEEK_SET);

    // BAH: Make fstat realization

    return size;
}

size_t get_char_amount(const char* const string, const char ch)
{
    size_t amount = 1;
    for (const char* ch_ptr = string; ch_ptr = strchr(ch_ptr, ch); amount++, ch_ptr++);
    return amount;
}

void copy_lines_ptrs_to_arr(const char** arr, const char* string) // расставить const
{
    *arr = string;
    const char* ch_ptr = string;
    while (ch_ptr = strchr(ch_ptr, '\n'))
        *++arr = ++ch_ptr;
}
#endif
