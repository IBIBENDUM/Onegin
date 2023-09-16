#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

#include "textlib.h"

ssize_t get_file_size(const int file_descriptor)
{
    assert(file_descriptor > -1);
    struct stat file_info;
    if (fstat(file_descriptor, &file_info) != -1)
        return (ssize_t) file_info.st_size;

    return -1;
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
    return get_char_amount(string, '\n') + 1;
}

// Needs free()
char* read_file(const char* file_name)
{
    const int file_descriptor = open(file_name, O_RDONLY);
    if (file_descriptor == -1)
    {
        perror("textlib.cpp: Couldn't open file");
        return NULL;
    }

    const ssize_t size = get_file_size(file_descriptor);
    if (size == -1)
    {
        perror("textlib.cpp: Couldn't get file size");
        return NULL;
    }

    char* buffer = (char*) calloc(size + 1, sizeof(char));
    if (!buffer)
    {
        perror("textlib.cpp: Error at memory allocation");
        return NULL;
    }

    if (read(file_descriptor, buffer, size) == -1)
    {
        perror("textlib.cpp: Error at reading file");
        return NULL;
    }

    if (close(file_descriptor))
    {
        perror("textlib.cpp: Error at file closing");
        return NULL;
    }

    for (size_t i = 0; i < size + 1; i++)
    {
        DEBUG("buffer[%zu] = <%d>\n", i, buffer[i]);
    }

    return buffer;
}

// Needs free()
char** parse_lines_to_arr(char* string, const size_t lines_amount)
{
    assert(string);

    char** lines_ptrs = (char**) calloc(lines_amount, sizeof(char**));
    if (!lines_ptrs)
    {
        perror("textlib.cpp: Error at memory allocation");
        return NULL;
    }

    char** arr = lines_ptrs;
    *arr = string;
    char* ch_ptr = string;
    while (ch_ptr = strchr(ch_ptr, '\n'))
        *++arr = ++ch_ptr;

    return lines_ptrs;
}

void print_line(const char* str, FILE* file_ptr)
{
    assert(str);
    assert(file_ptr);

    // BAH: Make line struct
    size_t len = strcspn(str, "\n") + 1; // +1 for \n itself
    if (len > 2) // Dont print empty lines
        fwrite(str, sizeof(char), len, file_ptr);
}

void write_lines_to_file(const char* const* str_ptr, FILE* file_ptr)
{
    assert(str_ptr);
    assert(file_ptr);

    while (*str_ptr)
    {
        print_line(*str_ptr, file_ptr);
        str_ptr++;
    }
}

void write_dictionaty_separator(const char symbol, FILE* file_ptr)
{
    assert(file_ptr);

    fprintf(file_ptr, "\n----------------------\n%c\n", toupper(symbol));
}

void print_seperator(FILE* file_ptr)
{
    assert(file_ptr);

    fprintf(file_ptr, "--------------------------------------\n");
}

void write_in_dictionary_format(const char* const* str_ptr, FILE* file_ptr)
{
    assert(str_ptr);
    assert(file_ptr);

    char prev_symbol = *move_to_alphabet_sym(*str_ptr, COMPARE_FORWARD);
    write_dictionaty_separator(prev_symbol, file_ptr);
    while (*str_ptr)
    {
        // BAH: Make line struct
        size_t len = strcspn(*str_ptr, "\n") + 1; // +1 for \n itself
        if (len > 2) // Dont print empty lines
        {
            char symbol = *move_to_alphabet_sym(*str_ptr, COMPARE_FORWARD);
            DEBUG("<%c> <%c>\n", prev_symbol, symbol);
            if (symbol != prev_symbol)
            {
                write_dictionaty_separator(symbol, file_ptr);
                prev_symbol = symbol;
            }
            fwrite(*str_ptr, sizeof(char), len, file_ptr);
        }
        str_ptr++;
    }
}

const char* move_to_alphabet_sym(const char* str, const int direction)
{
    assert(str);

    while (!isalpha(*(str)) && *(str) != '\n')
    {
        str += direction;
    }
    return str;
}

int compare_lines(const char* line_1, const char* line_2, const int direction)
{
    // Skip leading non alphabet symbols
    line_1 = move_to_alphabet_sym(line_1, direction);
    line_2 = move_to_alphabet_sym(line_2, direction);

    // Move empty lines to end??
    if (*line_1 == '\n')
        return 1;
    if (*line_2 == '\n')
        return -1;

    // What if "ABC\n" and "ABC\n" reverse comparison?
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

    // Make line struct or realisation by pointer arithmetic
    return compare_lines(str_1 + strcspn(str_1, "\n") - 1, str_2 + strcspn(str_2, "\n") - 1, COMPARE_BACKWARD);
}

void swap_values(void* a, void* b, const size_t size)
{
    assert(a);
    assert(b);

    static uint8_t* temp[10] = {};
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

size_t get_max_idx(const size_t pass_number, void* data, const size_t size, const size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    assert(data);

    size_t max_idx = pass_number;
    for (size_t i = pass_number + 1; i < size; i++)
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
    for (size_t pass_number = 0; pass_number < size; pass_number++)
    {

        size_t max_idx = get_max_idx(pass_number, data, size, elem_size, compare_func);

        swap_values((void*)((size_t)data + pass_number * elem_size),
                    (void*)((size_t)data + max_idx * elem_size), elem_size);

    }
}
