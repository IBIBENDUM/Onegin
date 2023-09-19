#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <cwchar>

int main()
{
    // setlocale(LC_ALL, "");

    // FILE* file_ptr = fopen("test.txt", "rb");
    // wchar_t buffer[100] = {};
    // fread(buffer, 1, 100, file_ptr);
    FILE* out_ptr = fopen("test_out.txt", "w");
    char buffer[] = "АБОБА";
    size_t i =  0;
    fprintf(out_ptr, "%s\n", buffer);
    // fputws(L"АБОБА", out_ptr);
    // wprintf(L"%s\n", buffer);
    // while (buffer[i])
    // {
    //     wprintf(L"char = %c code = %d\n", buffer[i], buffer[i]);
    //     // putchar(buffer[i]);
    //     i++;
    // }
    // printf("привет!");
}
