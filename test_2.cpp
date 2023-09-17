#include <stdio.h>
#include <stdlib.h>

typedef struct line_struct
{
    char* start;
    size_t len;
} line;

int main()
{

    const size_t lines_amount = 6000;
    line* line_ptr = (line*) calloc(lines_amount, sizeof(line));

    size_t i = 5990;
    (line_ptr + i)->len = 5;
    // line_ptr[i].len = 5;
    // line_ptr[
    printf("End\n");
    return 0;
}
