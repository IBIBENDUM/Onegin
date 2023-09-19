#include <stdio.h>
#include <locale.h>
#include <fcntl.h>

int main()
{
    setlocale(LC_ALL, "");
    FILE* out_ptr = fopen("test_out.txt", "wb");
    _setmode(_fileno(out_ptr), 0x00040000);
    _setmode(_fileno(stdout), 0x00040000);
    _setmode(_fileno(stdin), 0x00040000);
    // wchar_t buffer[] = L"АБОБА";
    wchar_t buffer[] = L"А а Ә ә Б б В в Г г Д д Е е Ё ё Ж ж Җ җ З з И и Й й К к Л л М м Н н Ң ң О о Ө ө П п Р р С с Т т У у Ү ү Ф ф Х х Һ һ Ц ц Ч ч Ш ш Щ щ Ъ ъ Ы ы Ь ь Э э Ю ю Я я";
    fwrite(buffer, sizeof(buffer[0]), 10, out_ptr);
    wprintf(L"%s\n", buffer);
}
