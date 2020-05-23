#include <string>

#include "quicksort.h"
#include "date.h"

using namespace std;

int partition(std::string arr[], int low, int high)
{
    std::string pivot = arr[high]; // pivot
    int i = (low - 1);             // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        date d1(arr[j]);
        date d2(pivot);
        // If current element is smaller than or equal to pivot
        if (isLater(d1, d2) > -1)
        {
            i++; // increment index of smaller element
            arr[i].swap(arr[j]);
        }
    }
    arr[i + 1].swap(arr[high]);
    return (i + 1);
}

void quickSort(std::string arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high); //partitioning index, to pi einai sti swsti thesi tou tr
        //kanw quisort gia ta prin kai gia ta meta xwria
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}