#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter size: ";
    cin >> n;

    int arr[n];
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    int minv = arr[0], maxv = arr[0], sum = 0;

    #pragma omp parallel for reduction(min:minv) reduction(max:maxv) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < minv) minv = arr[i];
        if (arr[i] > maxv) maxv = arr[i];
        sum += arr[i];
    }

    double avg = (double)sum / n;

    cout << "Min: " << minv << endl;
    cout << "Max: " << maxv << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
}


/*
OUTPUT: 

Enter size: 5
Enter elements:
12 54 31 75 69
Min: 12
Max: 75
Sum: 241
Average: 48.2
*/
