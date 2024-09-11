package no.kristiania.problem2_QuickSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;
import no.kristiania.common.ListShuffler;

import java.util.List;

public class B_QSCompareNumberOfComparisons {

    // Counter to track the number of comparisons
    private static int compareCount; // Counter to track the number of comparisons
    static double medianOfThree(double a, double b, double c){
        if ((a > b) && (a < c)){
            return a;
        } else if (b > c && (b > a)){
            return b;
        } else {
            return c;
        }
    }

    // Function to partition the array on the basis of pivot element
    private static int partition(double[] arr, int low, int high) {
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
            compareCount++;
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
    private static void sort(double[] arr, int low, int high) {
        if (low < high) {
            // Partition the array and get the partitioning index
            int pivot = partition(arr, low, high);

            // Recursively sort elements before and after the partition
            sort(arr, low, pivot - 1);
            sort(arr, pivot + 1, high);
        }
    }

    public static void sort(double[] arr) {
        // Reset comparison count before sorting
        compareCount = 0;
        sort(arr, 0, arr.length - 1);
    }

    public static int getCompareCount(){
        return compareCount;
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        // Create an instance of the AlgDat.common.CSVReader class
        DataReader csvReader = new DataReader();

        // Use the DataReader instance to read cities data from CSV file
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        // Extract latitudes into an array
        double[] latitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).latitude();
        }

        // Sort latitudes using quick sort
        sort(latitudes);
        int comparisonsBeforeShuffle = getCompareCount();
        System.out.print("\nNumber of comparisons before shuffle: " + getCompareCount());

        System.out.println("\nShuffling the values...");
        ListShuffler.shuffleLists(latitudes);

        sort(latitudes);
        int comparisonsAfterShuffle = getCompareCount();
        // Output the number of comparisons
        System.out.println("Number of comparisons after shuffle: " + getCompareCount());

        if (comparisonsBeforeShuffle == comparisonsAfterShuffle) {
            System.out.println("\nNumber of comparisons was equal in both cases.");
        } else {
            System.out.println("\nNumber of comparisons was not equal in both cases.");
        }
    }
}