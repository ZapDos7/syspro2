#include <string>

#include "quicksort.h"
#include "date.h"

using namespace std;

int partition(std::string arr[], int start, int finish)
{
    std::string pivot = arr[finish]; // pivot
    int i = (start - 1);             // pou einai to smallest

    for (int j = start; j < finish; j++)
    {
        date d1(arr[j]);
        date d2(pivot);
        if (isLater(d1, d2) > -1) // If current element is smaller than or equal to pivot
        {
            i++; // thesi smallest ++
            arr[i].swap(arr[j]);
        }
    }
    arr[i + 1].swap(arr[finish]);
    return (i + 1);
}

void quickSort(std::string arr[], int start, int finish)
{
    if (start < finish)
    {
        int ok = partition(arr, start, finish); // to ok einai stin thesi tou
        //kanw quisort gia ta prin kai gia ta meta xwria
        quickSort(arr, start, ok - 1);
        quickSort(arr, ok + 1, finish);
    }
}