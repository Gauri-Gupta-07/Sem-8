#include <iostream>
#include <omp.h>
using namespace std;

// Sequential Bubble Sort
void bubble(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

// Parallel Bubble (Odd-Even)
void parallelBubble(int arr[], int n) {
    #pragma omp parallel
    {
        for (int i = 0; i < n; i++) {

            #pragma omp for
            for (int j = 1; j < n; j += 2)
                if (arr[j] < arr[j-1])
                    swap(arr[j], arr[j-1]);

            #pragma omp barrier

            #pragma omp for
            for (int j = 2; j < n; j += 2)
                if (arr[j] < arr[j-1])
                    swap(arr[j], arr[j-1]);
        }
    }
}

int main() {
    int n;
    cout << "Enter size: ";
    cin >> n;

    int arr[n], arr2[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        arr2[i] = arr[i]; // copy for parallel
    }

    double start, end;

    // Sequential timing
    start = omp_get_wtime();
    bubble(arr, n);
    end = omp_get_wtime();
    cout << "Sequential Time: " << end - start << endl;

    // Parallel timing
    start = omp_get_wtime();
    parallelBubble(arr2, n);
    end = omp_get_wtime();
    cout << "Parallel Time: " << end - start << endl;

    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << arr2[i] << " ";
}



/*
OUTPUT : 

Enter size: 10 
Enter elements: 10 4 2 6 7 1 3 9 8 5 
Sequential Time: 0 
Parallel Time: 0.013 
Sorted: 1 2 3 4 5 6 7 8 9 10
*/
