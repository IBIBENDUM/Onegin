#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>

#include "textlib.h"

#ifdef NDEBUG
    #define DEBUG(FORMAT, ...)
#else
    #define DEBUG(FORMAT, ...)\
    do {\
        printf(FORMAT, __VA_ARGS__);\
    } while(0)
#endif
int main()
{
    // Get file size
    // FILE* file_ptr = fopen("test.txt", "rt");    // fread remove \r
    // int file_ptr1 = open("test.txt", 0);
    FILE* file_ptr = fopen("test.txt", "rb");
    if (!file_ptr)
    {
        printf("Error at file open");
        return 1;
    }

    size_t size = get_file_size(file_ptr);

    DEBUG("file_size = <%zu>\n", size); // size counts \n as 2 symbols: \r\n

    char* buffer = (char*) calloc(size + 1, sizeof(char));
    assert(buffer);

    fread(buffer, sizeof(char), size, file_ptr); // Try read()
    // read(file_ptr1, buffer, size); // Try read()
    if (fclose(file_ptr))
    {
        printf("Error at file closing");
        return 1;
    }

    for (size_t i = 0; i < size + 1; i++)
    {
        DEBUG("buffer[%zu] = <%d>\n", i, buffer[i]);
    }
    DEBUG("%s\n", buffer);

    size_t lines_amount = get_char_amount(buffer, '\n');
    DEBUG("lines_amount = <%zu>\n", lines_amount);

    char** lines_ptrs = (char**) calloc(lines_amount, sizeof(char**));
    assert(lines_ptrs);

    parse_lines_to_arr(lines_ptrs, buffer);
    write_lines_to_file(lines_ptrs, stdout);
    printf("------------------\n");
    sort_lines(lines_ptrs, lines_amount, sizeof(lines_ptrs[0]), strcmp_without_punctuation);
    write_lines_to_file(lines_ptrs, stdout);

    free(buffer);
    free(lines_ptrs);
    return 0;
}
