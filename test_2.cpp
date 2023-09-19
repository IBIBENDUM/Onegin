#include <stdio.h>
#include <locale.h>
#include <fcntl.h>
#include <cwchar>


int main()
{
    setlocale(LC_ALL, "");
    FILE* out_ptr = fopen("test_out.txt", "wb, ccs=UTF-16LE");
    // _setmode(_fileno(out_ptr), 0x00040000);
    // _setmode(_fileno(stdout), 0x00040000);
    // _setmode(_fileno(stdin), 0x00040000);
    // fwide(out_ptr, );
    // wchar_t buffer[] = L"АБОБА";
    wchar_t buffer[11] = {};
    FILE* input_ptr = fopen("input.txt", "rb");
    _setmode(_fileno(input_ptr), 0x00040000);
    fread(buffer, sizeof(wchar_t), 10, input_ptr);
    // wchar_t buffer[] = L"А а Ә ә Б б В в Г г Д д Е е Ё ё Ж ж Җ җ З з И и Й й К к Л л М м Н н Ң ң О о Ө ө П п Р р С с Т т У у Ү ү Ф ф Х х Һ һ Ц ц Ч ч Ш ш Щ щ Ъ ъ Ы ы Ь ь Э э Ю ю Я я";
    fwrite(buffer, sizeof(buffer[0]), 10, out_ptr);
    wprintf(L"%s\n", buffer);
}
