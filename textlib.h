#ifndef TEXTLIB_H
#define TEXTLIB_H

#include <stdio.h>

#ifdef NDEBUG
    #define DEBUG(FORMAT, ...)
#else
    #define DEBUG(FORMAT, ...)\
    do\
    {\
        printf(FORMAT, ##__VA_ARGS__);\
    } while(0)
#endif

#define FREE_AND_NULL(ptr)\
do{\
    free(ptr);\
    ptr = NULL;\
} while(0)

#define HANDLE_ERROR(exp, message, ...)\
    do {\
        if (!(exp))\
        {\
            perror(__FILE__ ": " message);\
            return __VA_ARGS__;\
        }\
    } while(0)

enum COMPARE_TYPE
{
    COMPARE_FORWARD = 1,
    COMPARE_BACKWARD = -1
};

typedef struct line_struct
{
    char* start;
    size_t len;
} line;

ssize_t get_file_size(const char* file_name);

size_t get_char_amount(const char* const string, const char ch);

size_t get_lines_amount(const char* const string);

// needs free()
char* read_file(const char* file_name);

// need free()
line* parse_lines_to_arr(char* string, const size_t lines_amount);

void print_line(line* line_ptr, FILE* file_ptr);

// void write_lines_to_file(const char* const* str_ptr, FILE* file_ptr);
void write_lines_to_file(line* line_ptr, size_t lines_amount, FILE* file_ptr);

void write_in_dictionary_format(line* line_ptr, size_t lines_amount, FILE* file_ptr);

void print_seperator(FILE* file_ptr);

const char* move_to_alphabet_sym(const char* str, int direction);

int compare_lines_forward(const void* line_1_ptr, const void* line_2_ptr);

int compare_lines_backward(const void* line_1_ptr, const void* line_2_ptr);

void sort_lines(void* data, size_t size, size_t elem_size,
                int (*compare_func) (const void* a, const void* b));

void quick_sort(void* arr, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b));

#endif
