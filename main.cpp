#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "textlib.h"

const char* file_in_name = "test.txt";
const char* file_out_name = "output.txt";
int main()
{
    // FILE* file_ptr = fopen(file_in_name, "rt");
    // int file_ptr1 = open("test.txt", 0);
    FILE* file_ptr = fopen("test.txt", "rb");
    if (!file_ptr)
    {
        perror("Error at file open");
        return 1;
    }

    ssize_t size = get_file_size(file_ptr);
    if (size == -1)
    {
        return -1;
    }
    DEBUG("file_size = <%zu>\n", size);

    char* buffer = (char*) calloc(size + 1, sizeof(char));
    if (!buffer)
    {
        perror("Error at memory allocation");
    }

    fread(buffer, sizeof(char), size, file_ptr); // Try read()
    // read(file_ptr1, buffer, size);

    if (fclose(file_ptr))
    {
        perror("Error at file closing");
        return 1;
    }

    for (size_t i = 0; i < size + 1; i++)
    {
        DEBUG("buffer[%zu] = <%d>\n", i, buffer[i]);
    }
    DEBUG("%s\n", buffer);

    size_t lines_amount = get_lines_amount(buffer);
    DEBUG("lines_amount = <%zu>\n", lines_amount);

    char** lines_ptrs = (char**) calloc(lines_amount, sizeof(char**));
    assert(lines_ptrs);

    parse_lines_to_arr(lines_ptrs, buffer);



    FILE* file_out = fopen(file_out_name, "w");
    if (!file_out)
    {
        perror("Error at file open");
        return 1;
    }

    fprintf(stdout, "-----------------------------------\n");
    sort_lines(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    write_lines_to_file(lines_ptrs, stdout);
    fprintf(stdout, "-----------------------------------\n");
    sort_lines(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_backward);
    write_lines_to_file(lines_ptrs, stdout);
    fprintf(stdout, "-----------------------------------\n");
    fprintf(stdout, "%s\n", buffer);

    if (fclose(file_out))
    {
        perror("Error at file closing");
        return 1;
    }


    free(buffer);
    free(lines_ptrs);
    return 0;
}
