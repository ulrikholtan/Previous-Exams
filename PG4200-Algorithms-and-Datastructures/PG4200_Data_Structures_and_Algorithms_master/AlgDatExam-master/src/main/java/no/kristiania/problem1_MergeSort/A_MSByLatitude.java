package no.kristiania.problem1_MergeSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class A_MSByLatitude {
    public static void sort(double[] arr) {
        if (arr == null || arr.length <= 1) {
            return; // Array is already sorted or empty
        }
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
        int n1 = mid - left + 1; // Size of the left subarray
        int n2 = right - mid; // Size of the right subarray

        double[] L = new double[n1];
        double[] R = new double[n2];

        for (int i = 0; i < n1; i++) {
            L[i] = arr[left + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = arr[mid + 1 + j];
        }

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

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        DataReader csvReader = new DataReader();
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        double[] latitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).latitude();
        }

        sort(latitudes);

        System.out.println("\nSorted latitudes:");
        for (double latitude : latitudes) {
            System.out.printf("%.4f%n", latitude);
        }
    }
}
