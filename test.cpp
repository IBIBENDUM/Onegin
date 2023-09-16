#include <stdio.h>
#include <assert.h>

int compare_int(const void* a_void_ptr, const void* b_void_ptr)
{
    int a = *((int*) a_void_ptr);
    int b = *((int*) b_void_ptr);

    return a - b;
}

void swap_values(void* a_void_ptr, void* b_void_ptr, const size_t size)
{
    char* a_ptr = (char*) a_void_ptr;
    char* b_ptr = (char*) b_void_ptr;

    for (size_t i = 0; i < size; i++)
    {
        char temp = *(a_ptr + i);
        *(a_ptr + i) = *(b_ptr + i);
        *(b_ptr + i) = temp;
    }
}

int partition(void* arr, size_t left, size_t right, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    size_t pivot = (left + right) / 2;

    while (left <= right)
    {
        while (compare_func((void*)((size_t) arr + left * elem_size), (void*)((size_t) arr + pivot * elem_size)) < 0)
            left++;
        while (compare_func((void*)((size_t) arr + right * elem_size), (void*)((size_t) arr + pivot * elem_size)) > 0)
            right--;

        if (left <= right)
        {
            void* left_ptr = (void*)((size_t) arr + left * elem_size);
            void* right_ptr = (void*)((size_t) arr + right * elem_size);
            void* pivot_ptr = (void*)((size_t) arr + pivot * elem_size);

            if (compare_func(left_ptr, pivot_ptr) == 0)
                pivot = left;
            else if (compare_func(left_ptr, pivot_ptr) == 0)
                pivot = right;
            swap_values(left_ptr, right_ptr, elem_size);
            left++;
            right--;
        }
    }
    return left;
}

void quick_sort_recursion(void* arr, size_t start, size_t end, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    if (start < end)
    {
        size_t right_start = partition(arr, start, end, elem_size, compare_func);
        quick_sort_recursion(arr, start, right_start - 1, elem_size, compare_func);
        quick_sort_recursion(arr, right_start, end, elem_size, compare_func);
    }
}

void quick_sort(void* arr, size_t size, size_t elem_size, int (*compare_func) (const void* a, const void* b))
{
    quick_sort_recursion(arr, 0, size, elem_size, compare_func);
}
int main()
{
    int data[] = {4, 2, 3, 32, 6, 5, 35, 7, 3};
    size_t size = sizeof(data) / sizeof(data[0]);
    quick_sort(data, size - 1, sizeof(int), compare_int);
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", data[i]);
    }
}


