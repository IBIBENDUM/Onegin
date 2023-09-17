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

ssize_t get_file_size(const char* file_name)
{
    struct stat file_info;
    if (stat(file_name, &file_info) != -1)
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

size_t get_line_len(const char* string)
{
    // DEBUG("get_line_len:\n");
    return strcspn(string, "\n") + 1;
}

char* read_file(const char* file_name)
{
    DEBUG("read_file()\n");
    FILE* file_ptr = fopen(file_name, "rb");
    HANDLE_ERROR(file_ptr, "Couldn't open file", NULL);
    DEBUG("File opened\n");

    const ssize_t size = get_file_size(file_name);
    HANDLE_ERROR(size != -1, "Couldn't get file size", NULL);
    DEBUG("Got size\n");

    char* buffer = (char*) calloc(size + 1, sizeof(char));
    HANDLE_ERROR(buffer, "Error at memory allocation", NULL);
    DEBUG("Buffer allocated\n");

    size_t fread_ret_val = fread(buffer, sizeof(char), size, file_ptr);
    //??
    HANDLE_ERROR(fread_ret_val, "Error at file reading", NULL);
    DEBUG("File readed\n");


    size_t fclose_ret_val = fclose(file_ptr);
    file_ptr = NULL;
    HANDLE_ERROR(!fclose_ret_val, "Error at closing file", NULL);
    DEBUG("File closed\n");

    // for (size_t i = 0; i < size + 1; i++)
    // {
    //     DEBUG("buffer[%zu] = <%d>\n", i, buffer[i]);
    // }
    return buffer;
}

// static void initialize_line(line* line_ptr, char* string)
// {
//     // DEBUG("initialize_line():\n");
//     line_ptr->start = string;
//     line_ptr->len = get_line_len(string);
//     // DEBUG("line_ptr->start = string\n");
//     // DEBUG("Line initialized\n");
// }

// Needs free()
// add const
line* parse_lines_to_arr(char* string, const size_t lines_amount)
{
    assert(string);
    DEBUG("parse_lines_to_arr():\n");

    line* lines_ptrs = (line*) calloc(lines_amount + 1, sizeof(line));
    HANDLE_ERROR(lines_ptrs, "Error at memory allocation", NULL);


    line* line_ptr = lines_ptrs;
    char* str_ptr = string;

    size_t i = 0;
    do
    {

        size_t line_length = get_line_len(str_ptr);

        line_ptr[i].start = str_ptr;
        line_ptr[i].len = line_length;
        // DEBUG("line_length = %d\n", line_length);
        // DEBUG("line_ptr->len = %zu\n", line_length);

        str_ptr += line_length; // move to symbol after \n
        i++;
    }
    while (i < lines_amount);

    return lines_ptrs;
}

void print_line(line* line_ptr, FILE* file_ptr)
{
    assert(line_ptr);
    assert(file_ptr);

    size_t len = line_ptr->len;
    if (len > 2) // Dont print empty lines
        fwrite(line_ptr->start, sizeof(char), len, file_ptr);
}
//
// void check_empty_line(line* line_ptr)
// {
//     char* str = line_ptr->start;
//     size_t len = line_ptr->len;
//     if ();
//     while (!isalpha(*(str)) && *(str) != '\n')
//         str++;
// }

void write_lines_to_file(line* line_ptr, size_t lines_amount, FILE* file_ptr)
{
    assert(line_ptr);
    assert(file_ptr);
    DEBUG("write_lines_to_file():\n");
    for (size_t i = 0; i < lines_amount; i++)
        print_line(line_ptr + i, file_ptr);
}

static void write_dictionary_separator(const char symbol, FILE* file_ptr)
{
    assert(file_ptr);

    fprintf(file_ptr, "\n----------------------\n%c\n", toupper(symbol));
}

void print_seperator(FILE* file_ptr)
{
    assert(file_ptr);

    fprintf(file_ptr, "--------------------------------------\n");
}

void write_in_dictionary_format(line* line_ptr, const size_t lines_amount, FILE* file_ptr)
{
    assert(line_ptr);
    assert(file_ptr);

    DEBUG("write_in_dictionary_format():");

    char prev_symbol = *move_to_alphabet_sym(line_ptr[0].start, COMPARE_FORWARD);
    write_dictionary_separator(prev_symbol, file_ptr);
    for (size_t i = 0; i < lines_amount; i++)
    {
        size_t len = line_ptr[i].len;
        if (len > 2)
        {
            char symbol = *move_to_alphabet_sym(line_ptr[i].start, COMPARE_FORWARD);
            if (symbol != prev_symbol)
            {
                write_dictionary_separator(symbol, file_ptr);
                prev_symbol = symbol;
            }
            fwrite(line_ptr[i].start, sizeof(char), len, file_ptr);
        }
    }
}

const char* move_to_alphabet_sym(const char* str, const int direction)
{
    assert(str);

    while (!isalpha(*(str)) && *(str) != '\n')
        str += direction;

    return str;
}

static int compare_lines(const char* line_1_ptr, const char* line_2_ptr, const int direction)
{
    assert(line_1_ptr);
    assert(line_2_ptr);

    // Skip leading non alphabet symbols
    const char* line_1 = move_to_alphabet_sym(line_1_ptr, direction);
    const char* line_2 = move_to_alphabet_sym(line_2_ptr, direction);

    while (*line_1 == *line_2)
    {
        if (*line_1 == '\n')
        {
            DEBUG("return 0\n");
            DEBUG("--------------------------\n");
            return 0;
        }
        // DEBUG("ch1 = %c ch2 = %c\n", *line_1, *line_2);
        line_1 += direction;
        line_2 += direction;
    }
    // DEBUG("ch1 = %c ch2 = %c\n", *line_1, *line_2);
    DEBUG("return %d\n", *line_1 - *line_2);
    DEBUG("--------------------------\n");
    return *line_1 - *line_2;
}

int compare_lines_forward(const void* line_1_ptr_void, const void* line_2_ptr_void)
{
    assert(line_1_ptr_void);
    assert(line_2_ptr_void);

    const line* line_1_ptr = (const line*) line_1_ptr_void;
    const line* line_2_ptr = (const line*) line_2_ptr_void;

    return compare_lines(line_1_ptr->start, line_2_ptr->start, COMPARE_FORWARD);
}

int compare_lines_backward(const void* line_1_ptr_void, const void* line_2_ptr_void)
{
    assert(line_1_ptr_void);
    assert(line_2_ptr_void);

    const line* line_1_ptr = (const line*) line_1_ptr_void;
    const line* line_2_ptr = (const line*) line_2_ptr_void;

    return compare_lines(line_1_ptr->start + line_1_ptr->len - 2, line_2_ptr->start + line_2_ptr->len - 2, COMPARE_BACKWARD);
}

void swap_values(void* a_void_ptr, void* b_void_ptr, const size_t size)
{
    assert(a_void_ptr);
    assert(b_void_ptr);

    char* a_ptr = (char*) a_void_ptr;
    char* b_ptr = (char*) b_void_ptr;

    for (size_t i = 0; i < size; i++)
    {
        char temp = *(a_ptr + i);
        *(a_ptr + i) = *(b_ptr + i);
        *(b_ptr + i) = temp;
    }
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

int partition(void* data, size_t left, size_t right, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    size_t pivot = (left + right) / 2;

    while (left <= right)
    {
        while (compare_func((void*)((size_t) data + left * elem_size), (void*)((size_t) data + pivot * elem_size)) < 0)
            left++;

        while (compare_func((void*)((size_t) data + right * elem_size), (void*)((size_t) data + pivot * elem_size)) > 0)
            right--;

        if (left <= right)
        {
            void* left_ptr = (void*)((size_t) data + left * elem_size);
            void* right_ptr = (void*)((size_t) data + right * elem_size);
            void* pivot_ptr = (void*)((size_t) data + pivot * elem_size);

            DEBUG("left = %d\n", left);
            DEBUG("right = %d\n", right);
            if (compare_func(left_ptr, pivot_ptr) == 0)
                pivot = right;
            else if (compare_func(right_ptr, pivot_ptr) == 0)
                pivot = left;

            swap_values(left_ptr, right_ptr, elem_size);
            left++;
            right--;
        }
    }
    return left;
}
void quick_sort_recursion(void* data, size_t start, size_t end, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    DEBUG("end = %d, start = %d\n", end, start);
    if (start < end)
    {
        void* start_ptr = (void*)((size_t) data + start * elem_size);
        void* end_ptr = (void*)((size_t) data + end * elem_size);
//
//         size_t size = end - start + 1;
//         if (size == 3)
//         {
//             sort_lines((void*)((size_t) data + start * elem_size), size, elem_size, compare_func);
//         }
//
//         if (size == 2)
//         {
//             if (compare_func(start_ptr, end_ptr) > 0)
//                 {
//                     swap_values(start_ptr, end_ptr, elem_size);
//                 }
//         }
//
//         else
//         {
            DEBUG("NEW RECURSION\n");
            size_t right_start = partition(data, start, end, elem_size, compare_func);
            quick_sort_recursion(data, start, right_start - 1, elem_size, compare_func);
            quick_sort_recursion(data, right_start, end, elem_size, compare_func);
        // }
    }
}

void quick_sort(void* data, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    quick_sort_recursion(data, 0, size - 1, elem_size, compare_func);
}
