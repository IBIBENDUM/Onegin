#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>

#include "textlib.h"

int main()
{
    // Get file size
    FILE* file_ptr = fopen("test.txt", "rt");    // fread remove \r
    // int file_ptr1 = open("test.txt", 0);    // fread remove \r
    // FILE* file_ptr = fopen("test.txt", "rb");
    if (!file_ptr)
    {
        printf("Error at file open");
        return 1;
    }

    size_t size = get_file_size(file_ptr);

    // BAH: Make debug info only for debug compilation
    printf("file_size = <%zu>\n", size); // size counts \n as 2 symbols: \r\n

    char* buffer = (char*) calloc(size + 1, sizeof(char));
    assert(buffer);

    fread(buffer, sizeof(char), size, file_ptr); // Try read()
    // read(file_ptr1, buffer, size); // Try read()
    if (fclose(file_ptr))
    {
        printf("Error at file closing");
        return 1;
    }

    for (size_t i = 0; i < size; i++)
    {
        printf("buffer[%zu] = <%d>\n", i, buffer[i]);
    }
    printf("%s\n", buffer);

    size_t lines_amount = get_char_amount(buffer, '\n');
    printf("lines_amount = <%zu>\n", lines_amount);

    char** lines_ptrs = (char**) calloc(lines_amount, sizeof(char**));
    assert(lines_ptrs);

    // Without sorting yet
    parse_lines_to_arr(lines_ptrs, buffer);
    write_lines_to_file(lines_ptrs, stdout);

//     for (size_t i = 0; i < lines_amount; i++)
//     {
//         printf("i = <%zu>\n", i);
//         printf("%s\n", lines_ptrs[i]);
//     }

    free(buffer);
    free(lines_ptrs);
    return 0;
}
