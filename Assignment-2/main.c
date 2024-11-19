// CODE: include necessary library(s)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mySort.h"
// Utility functions
void printArray(int arr[], int n);
#include "mySort.c"  // Include the implementation of sorting functions in mySort.c


// Test the sorting algorithms
int main() {
    int arr[] = {64, -134, -5, 0, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int testArr[n];
    int testArr2[n];
    int testArr3[n];
    int testArr4[n];
    int testArr5[n];

    // Bubble Sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    bubbleSort(testArr, n);
    printf("Bubble sorted array: ");
    printArray(testArr, n);

    // Merge Sort
    memcpy(testArr2, arr, n * sizeof(int));
    mergeSort(testArr2,0, n-1);
    printf("Merge sorted array: ");
    printArray(testArr2, n);
    
    //Heap Sort
    memcpy(testArr3, arr, n * sizeof(int));
    heapSort(testArr3,n);
    printf("Heap sorted array: ");
    printArray(testArr3, n);

    //Insertion Sort
    memcpy(testArr4, arr, n * sizeof(int));
    insertionSort(testArr4,n);
    printf("Insertion sorted array: ");
    printArray(testArr4, n);

    //Counting Sort
    memcpy(testArr5, arr, n * sizeof(int));
    countingSort(testArr5,n);
    printf("Counting sorted array: ");
    printArray(testArr5, n);

    return 0;
}

// Helper functions
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

