#include <iostream>
#include <omp.h>
using namespace std;

void merge(int arr[], int l, int m, int r) {
    int n1 = m-l+1, n2 = r-m;
    int L[n1], R[n2];

    for(int i=0;i<n1;i++) L[i]=arr[l+i];
    for(int i=0;i<n2;i++) R[i]=arr[m+1+i];

    int i=0,j=0,k=l;

    while(i<n1 && j<n2)
        arr[k++] = (L[i]<=R[j]) ? L[i++] : R[j++];

    while(i<n1) arr[k++] = L[i++];
    while(j<n2) arr[k++] = R[j++];
}

// Sequential Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l+r)/2;
        mergeSort(arr,l,m);
        mergeSort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l+r)/2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr,l,m);

            #pragma omp section
            parallelMergeSort(arr,m+1,r);
        }

        merge(arr,l,m,r);
    }
}

int main() {
    int n;
    cout << "Enter size: ";
    cin >> n;

    int arr[n], arr2[n];

    cout << "Enter elements:\n";
    for(int i=0;i<n;i++) {
        cin >> arr[i];
        arr2[i] = arr[i];
    }

    double start, end;

    // Sequential
    start = omp_get_wtime();
    mergeSort(arr,0,n-1);
    end = omp_get_wtime();
    cout << "Sequential Time: " << end-start << endl;

    // Parallel
    start = omp_get_wtime();
    parallelMergeSort(arr2,0,n-1);
    end = omp_get_wtime();
    cout << "Parallel Time: " << end-start << endl;

    cout << "Sorted: ";
    for(int i=0;i<n;i++) cout << arr2[i] << " ";
}




/*
OUTPUT: 

Enter size: 8
Enter elements:
38 27 43 3 9 82 10 1
Sequential Time: 0
Parallel Time: 0.00399995
Sorted: 1 3 9 10 27 38 43 82
*/
