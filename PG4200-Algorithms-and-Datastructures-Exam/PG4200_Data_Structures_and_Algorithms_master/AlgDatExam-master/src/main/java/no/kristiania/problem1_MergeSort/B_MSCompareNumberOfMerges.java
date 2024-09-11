package no.kristiania.problem1_MergeSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;
import no.kristiania.common.ListShuffler;

import java.util.List;

public class
B_MSCompareNumberOfMerges {
    private static int mergeCount; // Counter to track the number of merge operations
    public static void sort(double[] arr) {
        if (arr == null || arr.length <= 1) {
            return; // Array is already sorted or empty
        }
        mergeCount = 0; // Reset merge count
        mergeSort(arr, 0, arr.length - 1); // Start merge sort
    }

    private static void mergeSort(double[] arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2; // Calculate the middle index
            mergeSort(arr, left, mid); // Sort the left half
            mergeSort(arr, mid + 1, right); // Sort the right half
            merge(arr, left, mid, right); // Merge the sorted halves
        }
    }

    private static void merge(double[] arr, int left, int mid, int right) {
        // Every time a merge is done, increment the counter
        mergeCount++;

        int n1 = mid - left + 1; // Size of the left subarray
        int n2 = right - mid; // Size of the right subarray

        // Create temporary arrays to hold the subarrays
        double[] L = new double[n1];
        double[] R = new double[n2];

        // Copy data to temporary arrays
        for (int i = 0; i < n1; i++) {
            L[i] = arr[left + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = arr[mid + 1 + j];
        }

        // Merge the two subarrays back into the original array
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy remaining elements of L[] if any
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        // Copy remaining elements of R[] if any
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    public static int getMergeCount(){
        return mergeCount;
    }


    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        // Create an instance of the DataReaderr class
        DataReader csvReader = new DataReader();

        // Use the DataReader instance to read cities data from CSV file
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        // Extract latitudes into an array
        double[] latitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).latitude();
        }

        // Sort latitudes using merge sort
        sort(latitudes);
        int mergesBeforeShuffle = getMergeCount();
        // Output the sorted latitudes
        System.out.print("\nNumber of merges before the shuffle: " + getMergeCount() + "\n");

        System.out.println("Shuffling the list...");
        ListShuffler.shuffleLists(latitudes);

        sort(latitudes);
        int mergesAfterShuffle = getMergeCount();
        System.out.print("Number of merges after the shuffle: " + getMergeCount() + "\n");

        if (mergesBeforeShuffle == mergesAfterShuffle) {
            System.out.println("\nNumber of merges was equal in both cases.");
        } else {
            System.out.println("\nNumber of merges was not equal in both cases.");
        }
    }
}

