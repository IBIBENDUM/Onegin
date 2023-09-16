#include <stdio.h>
#include <stdlib.h>

#include "textlib.h"

const char* file_in_name = "input.txt";
// const char* file_in_name = "test.txt";
const char* file_out_name = "output.txt";

int main()
{
    char* buffer = read_file(file_in_name);
    HANDLE_ERROR(buffer, "Error at buffering file", 1);

    size_t lines_amount = get_lines_amount(buffer);
    DEBUG("lines_amount = <%zu>\n", lines_amount);

    line* lines_ptrs = parse_lines_to_arr(buffer, lines_amount);
    HANDLE_ERROR(lines_ptrs, "Error at lines parsing", 1);

    FILE* file_out = fopen(file_out_name, "wb");
    HANDLE_ERROR(file_out, "Error at file open", 1);

// Make function for this
    DEBUG("START OUTPUT\n");
    // fprintf(file_out, "\nENCYCLOPEDIA\n");
    // print_seperator(file_out);
    // quick_sort(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    // sort_lines(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    // write_in_dictionary_format(lines_ptrs, lines_amount, file_out);

    // fprintf(file_out, "\nMC PUSHKIN\n");
    // print_seperator(file_out);
    // qsort(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_backward);
    // write_lines_to_file(lines_ptrs, lines_amount, file_out);

    fprintf(file_out, "\nORIGINAL\n");
    print_seperator(file_out);
    fprintf(file_out, "%s\n", buffer);

    printf("Sorting ended succesfully!");

    size_t fclose_ret_val = fclose(file_out);
    HANDLE_ERROR(!fclose_ret_val, "Error at file closing", 1);

    // FREE_AND_NULL(buffer);
    // FREE_AND_NULL(lines_ptrs);

    return 0;
}
