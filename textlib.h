#ifndef TEXTLIB_H
#define TEXTLIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

size_t get_file_size(FILE* file_ptr)
{
    assert(file_ptr);
    // fseek + ftell realization
    size_t prev_pos = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_END);
    size_t size = ftell(file_ptr);
    fseek(file_ptr, prev_pos, SEEK_SET);
    // asserts?

    // BAH: Make fstat realization

    return size;
}

size_t get_char_amount(const char* const string, const char ch)
{
    assert(string);
    size_t amount = 1;
    for (const char* ch_ptr = string; ch_ptr = strchr(ch_ptr, ch); amount++, ch_ptr++);
    return amount;
}

void parse_lines_to_arr(char** arr, char* string) // Add const
{
    assert(arr);
    assert(string);

    *arr = string;
    char* ch_ptr = string;
    while (ch_ptr = strchr(ch_ptr, '\n'))
        *++arr = ++ch_ptr;
    *++arr = 0;

}

void write_lines_to_file(char** str, FILE* file_ptr) // Add const
{
    assert(str);
    assert(file_ptr);

    while (*str)
    {
        size_t len = strcspn(*str, "\n\0") + 1;
        fwrite(*str, sizeof(char), len, file_ptr);
        str++;
    }
}

int strcmp_without_punctuation(const void* s_1, const void* s_2)
{
    const char* str_1 = (const char*) s_1;
    const char* str_2 = (const char*) s_2;
    while (!*(const char*)str_1)
    {
        if (ispunct((int) str_1))
            str_1++;
        if (ispunct((int) str_1))
            str_2++;
        if (*str_1 != *str_2)
            break;
        str_1++;
        str_2++;
    }
    if (*str_1 == *str_2)
        return 0;

    return ((*str_1 < *str_2) ? -1 : 1);
}

int compare_strings(const void* str_1, const void* str_2)
{
    return strcmp((char*)str_1, (char*)str_2);
}

void swap_values(void* a, void* b, size_t size)
{
    static uint8_t* temp[10] = {};
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void sort_lines(void* data, size_t size, size_t elem_size,
                int (*compare_func) (const void* a, const void* b))
{
    assert(data);
    assert(compare_func);

//  First sort (selection sort)
    for (size_t step = 0; step < size; step++)
    {
        size_t min_idx = step;
        for (size_t i = step + 1; i < size; i++)
        {
            if (compare_func((void*)((size_t) data + min_idx * elem_size), (void*)((size_t) data + i * elem_size)) < 0)
            {
                min_idx = i;
            }
        }
        swap_values((void*)((size_t)data + step * elem_size),
                    (void*)((size_t)data + min_idx * elem_size), elem_size);
    }
}
#endif
