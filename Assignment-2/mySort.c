#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------
// Bubble Sort

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

//----------------------------------------------------------------
// Merge Sort - Divide and Conquer, recursively. Then merge, recursively until you get the full sorted array.

void merge(int arr[], int l, int m, int r) {
    // Calculate the sizes of the left and right subarrays
    int leftSize = m - l + 1; // Size of left subarray
    int rightSize = r - m;    // Size of right subarray

    // Create temporary arrays for left and right subarrays
    int temp_left[leftSize], temp_right[rightSize];

    // Copy data to temporary arrays
    for (int i = 0; i < leftSize; i++) {
        temp_left[i] = arr[l + i]; // Copy left subarray
    }

    for (int i = 0; i < rightSize; i++) {
        temp_right[i] = arr[m + 1 + i]; // Copy right subarray
    }

    // Merge the temporary arrays back into arr[]
    int i = 0, j = 0, k = l; // Initialize indices for left, right, and merged arrays
    while (k <= r) { // Iterate until all elements are merged
        // If there are remaining elements in left or right subarrays
        if (i < leftSize && (j >= rightSize || temp_left[i] <= temp_right[j])) {
            arr[k] = temp_left[i]; // Take element from left subarray
            i++; // Move to next element in left subarray
        } else {
            arr[k] = temp_right[j]; // Take element from right subarray
            j++; // Move to next element in right subarray
        }
        k++; // Move to the next position in the merged array
    }
}

// Function to perform merge sort using recursion
void mergeSort_recursion(int arr[], int l, int r) {
    if (l < r) { // Base case: if the array has more than one element
        int m = l + (r - l) / 2; // Find the middle index

        // Recursively sort the left half
        mergeSort_recursion(arr, l, m);
        // Recursively sort the right half
        mergeSort_recursion(arr, m + 1, r);
        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

void mergeSort(int arr[], int l, int r) {
    mergeSort_recursion(arr, l, r); // Start the recursion
}

//----------------------------------------------------------------
// Heap Sort - works by taking "max heap" weird upside down tree converting each little tree to max numbered tree.

void heapify(int arr[], int n, int rootIndex) {
    int Largest = rootIndex; // Initialize largest as root
    int leftChildIndex = 2 * rootIndex + 1; // Left child index
    int rightChildIndex = 2 * rootIndex + 2; // Right child index

    if (leftChildIndex < n && arr[leftChildIndex] > arr[Largest]) { // If left child index is bigger than root, make it the largest
        Largest = leftChildIndex; 
    }

    if (rightChildIndex < n && arr[rightChildIndex] > arr[Largest]) { // If right child index is bigger than root, make it the largest
        Largest = rightChildIndex; 
    }

    if (Largest != rootIndex) { // If largest is not the root anymore
        int temp = arr[rootIndex]; // Swap root and largest
        arr[rootIndex] = arr[Largest];
        arr[Largest] = temp; // Swap root and largest in the original array

        heapify(arr, n, Largest); // "Heapify" the branch that has changed
    }
}

// Function to perform heap sort
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        // Build max heap
        heapify(arr, n, i);
    }

    // Take out the max element and place it at the end then add it into arr[]
    for (int i = n - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Heapify yet again to maintain max heap after the swap (from element 0)
        heapify(arr, i, 0);
    }
}  

//----------------------------------------------------------------
// Insertion Sort: simple sort that builds the sorted part one element at a time

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) { // Iterate through each element in the array starting from the second element
        int initial = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1], that are greater than initial,
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > initial) {
            arr[j + 1] = arr[j]; // Shift element to the right
            j--; // Move to the next element on the left
        }
        // Place the initial element at its correct position
        arr[j + 1] = initial;
    }
}

//----------------------------------------------------------------
// Counting Sort: simple sort that builds the sorted part one element at a time

void countingSort(int arr[], int n) {
    // Find the maximum element in the array
    int max = arr[0], min = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    // Create a count array to store the count of each element
    int range = max - min + 1; // Total range of elements
    int *count = (int *)calloc(range, sizeof(int));
    if (count == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    // Count occurrences of each element
    for (int i = 0; i < n; i++) {
        count[arr[i] - min]++;
    }

    // Reconstruct the sorted array
    int index = 0;
    for (int i = 0; i < range; i++) {
        while (count[i] > 0) {
            arr[index++] = i + min;
            count[i]--;
        }
    }

    // Free the dynamically allocated memory
    free(count);
}

//----------------------------------------------------------------
