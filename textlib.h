#ifndef TEXTLIB_H
#define TEXTLIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifdef NDEBUG
    #define DEBUG(FORMAT, ...)
#else
    #define DEBUG(FORMAT, ...)\
    do {\
        printf(FORMAT, ##__VA_ARGS__);\
    } while(0)
#endif

enum COMPARE_TYPE
{
    COMPARE_FORWARD = 1,
    COMPARE_BACKWARD = -1
};

ssize_t get_file_size(FILE* file_ptr)
{
    assert(file_ptr);

    // fseek + ftell realization
    size_t prev_pos = ftell(file_ptr);
    if (prev_pos != -1)
    {
        if (!fseek(file_ptr, 0, SEEK_END));
        {
            size_t size = ftell(file_ptr);
            if (size != -1)
            {
                if (!fseek(file_ptr, prev_pos, SEEK_SET));
                    return size;
            }
        }
    }
    printf("Error at getting file size");
    return -1;

    // BAH: Make fstat realization

}

size_t get_char_amount(const char* const string, const char ch)
{
    assert(string);

    size_t amount = 0;
    for (const char* ch_ptr = string; ch_ptr = strchr(ch_ptr, ch); amount++, ch_ptr++);
    return amount;
}

size_t get_lines_amount(const char* const string)
{
    assert(string);

    return get_char_amount(string, '\n') + 1;
}

void parse_lines_to_arr(char** arr, char* string) // Add const
{
    assert(arr);
    assert(string);

    *arr = string;
    char* ch_ptr = string;
    while (ch_ptr = strchr(ch_ptr, '\n'))
        *++arr = ++ch_ptr;
}

void print_line(const char* str, FILE* file_ptr)
{
    assert(str);
    assert(file_ptr);

    size_t len = strcspn(str, "\n") + 1; // +1 for \n itself
    fwrite(str, sizeof(char), len, file_ptr);
}

void write_lines_to_file(const char* const* str_ptr, FILE* file_ptr) // Add const
{
    assert(str_ptr);
    assert(file_ptr);

    while (*str_ptr)
    {
        print_line(*str_ptr, file_ptr);
        str_ptr++;
    }
}

const char* move_to_alphabet_sym(const char* str, int direction)
{
    assert(str);

    while (!isalpha(*(str)) && *(str) != '\n')
    {
        // DEBUG("%c", *(str));
        str += direction;
    }
    return str;
}

int compare_lines(const char* line_1, const char* line_2, int direction)
{
    // Skip leading non alphabet symbols
    line_1 = move_to_alphabet_sym(line_1, direction);
    line_2 = move_to_alphabet_sym(line_2, direction);

    DEBUG("--------------------------\n");
    // DEBUG("Compare this strings:\n");
    // print_line(line_1, stdout);
    // print_line(line_2, stdout);

    while (*line_1 == *line_2)
    {
        if (*line_1 == '\n')
        {
            DEBUG("return 0\n");
            DEBUG("--------------------------\n");
            return 0;
        }
        DEBUG("ch1 = %c ch2 = %c\n", *line_1, *line_2);
        line_1 += direction;
        line_2 += direction;
    }
    DEBUG("ch1 = %c ch2 = %c\n", *line_1, *line_2);
    DEBUG("return %d\n", *line_1 - *line_2);
    DEBUG("--------------------------\n");
    return *line_1 - *line_2;
}

int compare_lines_forward(const void* line_1_ptr, const void* line_2_ptr)
{
    assert(line_1_ptr);
    assert(line_2_ptr);

    return compare_lines(*(const char**) line_1_ptr, *(const char**) line_2_ptr, COMPARE_FORWARD);
}

int compare_lines_backward(const void* line_1_ptr, const void* line_2_ptr)
{
    assert(line_1_ptr);
    assert(line_2_ptr);

    const char* str_1 = *((const char**) line_1_ptr);
    const char* str_2 = *((const char**) line_2_ptr);

    return compare_lines(str_1 + strcspn(str_1, "\n") - 1, str_2 + strcspn(str_2, "\n") - 1, COMPARE_BACKWARD);
}


void swap_values(void* a, void* b, size_t size)
{
    assert(a);
    assert(b);

    static uint8_t* temp[10] = {};
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}


size_t get_max_idx(size_t step, void* data, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    assert(data);

    size_t max_idx = step;
    for (size_t i = step + 1; i < size; i++)
    {
        if (compare_func((char*) data + max_idx * elem_size, (char*)data + i * elem_size) > 0)
        {
            max_idx = i;
        }
    }
    return max_idx;
}

void sort_lines(void* data, size_t size, size_t elem_size,
                int (*compare_func) (const void* a, const void* b))
{
    assert(data);
    assert(compare_func);


//  First sort (selection sort)
    DEBUG("------------------------------------\n");
    for (size_t step = 0; step < size; step++)
    {

        size_t max_idx = get_max_idx(step, data, size, elem_size, compare_func);

        swap_values((void*)((size_t)data + step * elem_size),
                    (void*)((size_t)data + max_idx * elem_size), elem_size);

        DEBUG("------------------------------------\n");
    }
}
#endif
