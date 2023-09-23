#include <stdlib.h>
#include <assert.h>

#include "sorts.h"
#include "textlib.h"

static void swap_values(void* a_void_ptr, void* b_void_ptr, const size_t size)
{
    assert(a_void_ptr);
    assert(b_void_ptr);

    char* a_ptr = (char*) a_void_ptr;
    char* b_ptr = (char*) b_void_ptr;

    for (size_t i = 0; i < size; i++)  // Or memcpy?
    {
        char temp = *(a_ptr + i);
        *(a_ptr + i) = *(b_ptr + i);
        *(b_ptr + i) = temp;
    }
}

static size_t get_max_idx(const size_t pass_number, void* data, const size_t size, const size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    assert(data);

    size_t max_idx = pass_number;
    for (size_t i = pass_number + 1; i < size; i++)
    {
        if (compare_func((wchar_t*) data + max_idx * elem_size, (wchar_t*)data + i * elem_size) > 0)
        {
            max_idx = i;
        }
    }
    return max_idx;
}

void sort_lines(void* data, size_t size, size_t elem_size,
                int (*compare_func) (const void* a, const void* b))
{
    assert(data);
    assert(compare_func);


//  First sort (selection sort)
    for (size_t pass_number = 0; pass_number < size; pass_number++)
    {

        size_t max_idx = get_max_idx(pass_number, data, size, elem_size, compare_func);

        swap_values((void*)((size_t)data + pass_number * elem_size),
                    (void*)((size_t)data + max_idx * elem_size), elem_size);

    }
}


static int partition(void* data, size_t left, size_t right, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    size_t pivot = (left + right) / 2;

    while (left <= right)
    {
        while (compare_func((void*)((size_t) data + left * elem_size), (void*)((size_t) data + pivot * elem_size)) < 0)
            left++;

        while (compare_func((void*)((size_t) data + right * elem_size), (void*)((size_t) data + pivot * elem_size)) > 0)
            right--;

        if (left <= right)
        {
            void* left_ptr  = (void*)((size_t) data + left * elem_size);
            void* right_ptr = (void*)((size_t) data + right * elem_size);
            void* pivot_ptr = (void*)((size_t) data + pivot * elem_size);

            DEBUG("left = %d\n", left);
            DEBUG("right = %d\n", right);
            if (compare_func(left_ptr, pivot_ptr) == 0)
                pivot = right;
            else if (compare_func(right_ptr, pivot_ptr) == 0)
                pivot = left;

            swap_values(left_ptr, right_ptr, elem_size);
            left++;
            right--;
        }
    }
    return left;
}
static void quick_sort_recursion(void* data, size_t start, size_t end, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    DEBUG("end = %d, start = %d\n", end, start);
    if (start < end)
    {
        void* start_ptr = (void*)((size_t) data + start * elem_size);
        void* end_ptr   = (void*)((size_t) data + end   * elem_size);
//
//         size_t size = end - start + 1;
//         if (size == 3)
//         {
//             sort_lines((void*)((size_t) data + start * elem_size), size, elem_size, compare_func);
//         }
//
//         if (size == 2)
//         {
//             if (compare_func(start_ptr, end_ptr) > 0)
//                 {
//                     swap_values(start_ptr, end_ptr, elem_size);
//                 }
//         }
//
//         else
//         {
            DEBUG("NEW RECURSION\n");
            size_t right_start = partition(data, start, end, elem_size, compare_func);
            quick_sort_recursion(data, start, right_start - 1, elem_size, compare_func);
            quick_sort_recursion(data, right_start, end, elem_size, compare_func);
        // }
    }
}

void quick_sort(void* data, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    quick_sort_recursion(data, 0, size - 1, elem_size, compare_func);
}
