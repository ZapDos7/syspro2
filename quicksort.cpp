#include <string>

#include "quicksort.h"
#include "date.h"

using namespace std;

int partition(std::string arr[], int low, int high)
{
    std::string pivot = arr[low];
    int left = low;

    for (int i = low + 1; i < high; i++)
    {
        date d1(arr[i]); //cout << d1.get_date_as_string();
        date d2(pivot);  //cout << d2.get_date_as_string();
        if (isLater(d1, d2) > -1)
        {
            left++;
            arr[i].swap(arr[left]);
        }
    }
    arr[low].swap(arr[left]);
    return (left + 1);
}

void quickSort(std::string arr[], int low, int high)
{
    if (low < high)
    {
        int ok = partition(arr, low, high); //partitioning index, to pi einai sti swsti thesi tou tr
        //kanw quisort gia ta prin kai gia ta meta xwria
        quickSort(arr, low, ok - 1);
        quickSort(arr, ok + 1, high);
    }
}