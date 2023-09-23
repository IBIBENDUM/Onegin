#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "textlib.h"
#include "sorts.h"

const char* file_in_name = "onegin_eng.txt";
const char* file_out_name = "output.txt";


static bool handle_cmd_args(const int argc, char* const* argv);
static void print_help();


int main(const int argc, char* const* argv)
{
    if (handle_cmd_args(argc, argv))
        return 1;

    setlocale(LC_ALL, "");
    wchar_t* buffer = read_file(file_in_name);
    HANDLE_ERROR(buffer, "Error at buffering file", 1);

    size_t lines_amount = get_lines_amount(buffer);
    DEBUG("lines_amount = %zu\n", lines_amount);

    line* lines_ptrs = parse_lines_to_arr(buffer, lines_amount);
    HANDLE_ERROR(lines_ptrs, "Error at lines parsing", 1);
    FILE* file_out = fopen(file_out_name, "wb");
    _setmode(_fileno(file_out), 0x00040000); // Define mb?
    HANDLE_ERROR(file_out, "Error at file open", 1);

// Make function for this
    DEBUG("START OUTPUT\n");
    fwprintf(file_out, L"ENCYCLOPEDIJJJAAAA\n");
    print_seperator(file_out);
    quick_sort(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    write_in_dictionary_format(lines_ptrs, lines_amount, file_out);

    fwprintf(file_out, L"\nЧУВАК ЭТО РЕПЧИК\n");
    print_seperator(file_out);
    qsort(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_backward);
    write_lines_to_file(lines_ptrs, lines_amount, file_out);

    fwprintf(file_out, L"\nОРИГИНАЛ\n");
    print_seperator(file_out);
    fwprintf(file_out, L"%s\n", buffer);

    print_tatarstan_symbolism(file_out);

    printf("Sorting ended succesfully!");

    size_t fclose_ret_val = fclose(file_out);
    file_out = NULL;
    HANDLE_ERROR(!fclose_ret_val, "Error at file closing", 1);

    empty_lines(lines_ptrs);
    FREE_AND_NULL(buffer);
    FREE_AND_NULL(lines_ptrs);

    return 0;
}

static bool handle_cmd_args(const int argc, char* const* argv)
{

    assert(argv);
    assert(argc);

    int arg = 0;

    while ((arg = getopt(argc, argv, "i:o:h")) != -1)
    {
        switch (arg)
        {
            case 'i': {
                file_in_name = optarg;
                break;
            }

            case 'o': {
                file_out_name = optarg;
                break;
            }

            case 'h': {
                print_help();
                return 1;
            }

            default: {
                printf("Wrong option found\n");
                print_help();

                return 1;
            }
        }
    }

    return 0;
}

static void print_help()
{
    printf("OPTIONS:\n");
    printf("-h             Display help message\n");
    printf("-i             Choose input file name\n");
    printf("-o             Choose output file name\n");
}
