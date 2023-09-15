#include <stdio.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    FILE* file_ptr = fopen("test.txt", "rb");
    fread(
    printf(
}
