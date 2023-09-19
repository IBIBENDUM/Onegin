#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <fcntl.h>

#include "textlib.h"

// const char* file_in_name = "input.txt";
const char* file_in_name = "test.txt";
const char* file_out_name = "output.txt";

int main()
{
    setlocale(LC_ALL, "");
    // _setmode(_fileno(stdout), 0x00040000);
    // _setmode(_fileno(stdin), 0x00040000);
    // FILE* out_ptr = fopen("test_out.txt", "wb");
    // wchar_t buffer[] = L"АБОБА";
    // wchar_t buffer[] = L"А а Ә ә Б б В в Г г Д д Е е Ё ё Ж ж Җ җ З з И и Й й К к Л л М м Н н Ң ң О о Ө ө П п Р р С с Т т У у Ү ү Ф ф Х х Һ һ Ц ц Ч ч Ш ш Щ щ Ъ ъ Ы ы Ь ь Э э Ю ю Я я";
    // fwrite(buffer, sizeof(buffer[0]), 10, out_ptr);
    // wprintf(L"%s\n", buffer);

    wchar_t* buffer = read_file(file_in_name);
    HANDLE_ERROR(buffer, "Error at buffering file", 1);

    size_t lines_amount = get_lines_amount(buffer);
    DEBUG("lines_amount = %zu\n", lines_amount);

    line* lines_ptrs = parse_lines_to_arr(buffer, lines_amount);
    HANDLE_ERROR(lines_ptrs, "Error at lines parsing", 1);
    FILE* file_out = fopen(file_out_name, "wb");
    _setmode(_fileno(file_out), 0x00040000);
    HANDLE_ERROR(file_out, "Error at file open", 1);

// Make function for this
    DEBUG("START OUTPUT\n");
    fwprintf(file_out, L"ENCYCLOPEDIA\n");
    print_seperator(file_out);
    qsort(lines_ptrs, lines_amount - 1, sizeof(lines_ptrs[0]), compare_lines_forward);
    write_in_dictionary_format(lines_ptrs, lines_amount, file_out);

    fwprintf(file_out, L"\nМЦ ПУШКИН\n");
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
