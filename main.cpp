#include <stdio.h>
#include <stdlib.h>

#include "textlib.h"

const char* file_in_name = "input.txt";
// const char* file_in_name = "test.txt";
const char* file_out_name = "output.txt";

int main()
{
    char* buffer = read_file(file_in_name);
    if (!buffer)
        return 1;

    size_t lines_amount = get_lines_amount(buffer);
    DEBUG("lines_amount = <%zu>\n", lines_amount);

    char** lines_ptrs = parse_lines_to_arr(buffer, lines_amount);

    if (!lines_ptrs)
    {
        perror("Error at lines parsing");
        return 1;
    }

    FILE* file_out = fopen(file_out_name, "wb");
    if (!file_out)
    {
        perror("Error at file open");
        return 1;
    }

// Make function for this??
    fprintf(file_out, "\nSorted forward\n");
    print_seperator(file_out);
    sort_lines(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    write_in_dictionary_format(lines_ptrs, file_out);

    fprintf(file_out, "\nSorted backward\n");
    print_seperator(file_out);
    sort_lines(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_backward);
    write_lines_to_file(lines_ptrs, file_out);

    fprintf(file_out, "\nOriginal\n");
    print_seperator(file_out);
    fprintf(file_out, "%s\n", buffer);

    printf("Sorting ended succesfully!");

    if (fclose(file_out))
    {
        perror("Error at file closing");
        return 1;
    }

    free(buffer);
    free(lines_ptrs);
    return 0;
}
