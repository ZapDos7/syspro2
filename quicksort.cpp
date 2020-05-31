#include <string>

#include "quicksort.h"
#include "date.h"

using namespace std;

int partition(std::string arr[], int low, int high)
{
    std::string pivot = arr[low];
    cerr << "pivot is:\t" << pivot << "\n";
    int left = low;

    for (int i = low + 1; i <= high; i++)
    {
        cout << "\ncomparing:";
        date d1(arr[i]); cout << d1.get_date_as_string() << "\t";
        date d2(pivot);  cout << d2.get_date_as_string();
        cerr << endl;
        if (isLater(d1, d2) == -1)
        {
            cout << d1.get_date_as_string() << " later than  " << d2.get_date_as_string() << " ara DEN allazw\n";
            //left++;
            //arr[i].swap(arr[left]);
        }
        else
        {
            cout << d1.get_date_as_string() << " not later than  " << d2.get_date_as_string() << " ara allazw\n";
            left++;
            arr[i].swap(arr[left]);
        }
    }
    arr[low].swap(arr[left]);
    //return (left + 1);
    return left;
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