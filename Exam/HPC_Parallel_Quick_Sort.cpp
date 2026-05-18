#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Sequential Quicksort (used locally by each process)
void quicksort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[right]);

    int p = i + 1;
    quicksort(arr, left, p - 1);
    quicksort(arr, p + 1, right);
}

// Sequential quicksort wrapper
void sequential_sort(vector<int>& arr) {
    quicksort(arr, 0, arr.size() - 1);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    vector<int> data;

    // Input (only root)
    if (rank == 0) {
        cout << "Enter number of elements: ";
        cin >> n;

        data.resize(n);
        cout << "Enter elements:\n";
        for (int i = 0; i < n; i++) {
            cin >> data[i];
        }
    }

    // Broadcast size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double seq_time = 0;

    // Sequential execution
    if (rank == 0) {
        vector<int> temp = data;

        double start_seq = MPI_Wtime();
        sequential_sort(temp);
        double end_seq = MPI_Wtime();

        seq_time = end_seq - start_seq;

        cout << "\nSequential Time: " << seq_time << " seconds\n";
    }

    int local_n = n / size;
    vector<int> local_data(local_n);

    // Scatter data
    MPI_Scatter(data.data(), local_n, MPI_INT,
                local_data.data(), local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Start parallel timing
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // Local sort
    quicksort(local_data, 0, local_n - 1);

    // Gather results
    MPI_Gather(local_data.data(), local_n, MPI_INT,
               data.data(), local_n, MPI_INT,
               0, MPI_COMM_WORLD);

    // End timing
    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    // Final output
    if (rank == 0) {
        sort(data.begin(), data.end());

        double parallel_time = end - start;

        cout << "Parallel Time: " << parallel_time << " seconds\n";

        double speedup = seq_time / parallel_time;
        double efficiency = speedup / size;

        cout << "Speedup: " << speedup << endl;
        cout << "Efficiency: " << efficiency << endl;

        cout << "\nSorted array:\n";
        for (int i = 0; i < n; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}







/*
OUTPUT:

shubham@Shubham:/mnt/c/Users/Arnav/Desktop/OMP$ mpic++ HPC_Parallel_Quick_Sort.cpp -o quicksort
shubham@Shubham:/mnt/c/Users/Arnav/Desktop/OMP$ mpirun -np 4 ./quicksort
Enter number of elements: 8
Enter elements:
2 5 8 1 4 3 9 6

Sequential Time: 1.5e-06 seconds
Parallel Time: 2.55e-05 seconds
Speedup: 0.0588235
Efficiency: 0.0147059

Sorted array:
1 2 3 4 5 6 8 9
*/
