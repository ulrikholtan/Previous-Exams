package no.kristiania.problem2_QuickSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class A_QSByLatitude {

    // Function to find the median of three numbers
    double medianOfThree(double a, double b, double c){
        if ((a > b) && (a < c)){
            return a;
        } else if (b > c && (b > a)){
            return b;
        } else {
            return c;
        }
    }

    // Function to partition the array on the basis of pivot element
    private int partition(double[] arr, int low, int high) {
        double pivot = medianOfThree(arr[low], arr[(low + high) / 2], arr[high]);

        // Find the pivot index
        int pivotIndex = low;
        if (pivot == arr[(low + high) / 2]) {
            pivotIndex = (low + high) / 2;
        } else if (pivot == arr[high]) {
            pivotIndex = high;
        }

        // Swap pivot element with the last element
        double temp = arr[pivotIndex];
        arr[pivotIndex] = arr[high];
        arr[high] = temp;

        // Partition the array
        int i = low;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                // Swap elements
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
            }
        }

        // Move pivot element to the correct position
        temp = arr[i];
        arr[i] = arr[high];
        arr[high] = temp;

        return i;
    }

    // Function to perform quicksort
    private void sort(double[] arr, int low, int high) {
        if (low < high) {
            // Partition the array and get the partitioning index
            int pi = partition(arr, low, high);

            // Recursively sort elements before and after the partition
            sort(arr, low, pi - 1);
            sort(arr, pi + 1, high);
        }
    }

    // Function to sort the array
    public void sort(double[] arr) {
        sort(arr, 0, arr.length - 1);
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        // Create an instance of the DataReader class
        DataReader csvReader = new DataReader();

        // Use the DataReader instance to read cities data from CSV file
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        // Extract latitudes into an array
        double[] latitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).latitude();
        }

        // Create an instance of A2QuickSort
        A_QSByLatitude quickSort = new A_QSByLatitude();

        // Sort latitudes using quick sort
        quickSort.sort(latitudes);

        // Output the sorted latitudes
        System.out.println("\nSorted latitudes:");
        for (double latitude : latitudes) {
            System.out.printf("%.4f%n", latitude);
        }
    }
}