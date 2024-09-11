package no.kristiania.problem1_MergeSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class C_MSByEculideanDistance {
    private static int mergeCount; // Counter to track the number of merge operations

    public static void sort(double[] latitudes, double[] longitudes) {
        if (latitudes == null || longitudes == null || latitudes.length <= 1 || latitudes.length != longitudes.length) {
            return; // Invalid input
        }

        // Calculate Euclidean distances for each city
        double[] distances = new double[latitudes.length];
        for (int i = 0; i < latitudes.length; i++) {
            distances[i] = calculateEuclideanDistance(latitudes[i], longitudes[i]);
        }

        mergeCount = 0; // Reset merge count
        mergeSort(distances, latitudes, longitudes, 0, distances.length - 1); // Start merge sort
    }

    private static void mergeSort(double[] distances, double[] latitudes, double[] longitudes, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2; // Calculate the middle index
            mergeSort(distances, latitudes, longitudes, left, mid); // Sort the left half
            mergeSort(distances, latitudes, longitudes, mid + 1, right); // Sort the right half
            merge(distances, latitudes, longitudes, left, mid, right); // Merge the sorted halves
        }
    }

    private static void merge(double[] distances, double[] latitudes, double[] longitudes, int left, int mid, int right) {
        // Every time a merge is done, increment the counter
        mergeCount++;

        int n1 = mid - left + 1; // Size of the left subarray
        int n2 = right - mid; // Size of the right subarray

        // Create temporary arrays to hold the subarrays
        double[] LDistances = new double[n1];
        double[] RDistances = new double[n2];
        double[] LLatitudes = new double[n1];
        double[] RLatitudes = new double[n2];
        double[] LLongitudes = new double[n1];
        double[] RLongitudes = new double[n2];

        // Copy data to temporary arrays
        System.arraycopy(distances, left, LDistances, 0, n1);
        System.arraycopy(distances, mid + 1, RDistances, 0, n2);
        System.arraycopy(latitudes, left, LLatitudes, 0, n1);
        System.arraycopy(latitudes, mid + 1, RLatitudes, 0, n2);
        System.arraycopy(longitudes, left, LLongitudes, 0, n1);
        System.arraycopy(longitudes, mid + 1, RLongitudes, 0, n2);

        // Merge the two subarrays back into the original arrays
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (LDistances[i] <= RDistances[j]) {
                distances[k] = LDistances[i];
                latitudes[k] = LLatitudes[i];
                longitudes[k] = LLongitudes[i];
                i++;
            } else {
                distances[k] = RDistances[j];
                latitudes[k] = RLatitudes[j];
                longitudes[k] = RLongitudes[j];
                j++;
            }
            k++;
        }

        // Copy remaining elements of L[] if any
        while (i < n1) {
            distances[k] = LDistances[i];
            latitudes[k] = LLatitudes[i];
            longitudes[k] = LLongitudes[i];
            i++;
            k++;
        }

        // Copy remaining elements of R[] if any
        while (j < n2) {
            distances[k] = RDistances[j];
            latitudes[k] = RLatitudes[j];
            longitudes[k] = RLongitudes[j];
            j++;
            k++;
        }
    }

    private static double calculateEuclideanDistance(double latitude, double longitude) {
        // Calculate Euclidean distance from 0, 0
        return Math.sqrt(latitude * latitude + longitude * longitude);
    }

    public static int getMergeCount(){
        return mergeCount;
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        // Create an instance of the DataReader class
        DataReader csvReader = new DataReader();

        // Use the DataReader instance to read cities data from CSV file
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        // Extract latitudes and longitudes into arrays
        double[] latitudes = new double[cities.size()];
        double[] longitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).getLatitude();
            longitudes[i] = cities.get(i).getLongitude();
        }

        // Sort cities based on Euclidean distance
        sort(latitudes, longitudes);

        // Output the sorted latitudes and longitudes
        System.out.println("\nSorted Cities based on Euclidean Distance:");
        System.out.println("\nSorted data (Lat, Long, Euclidean):");
        for (int i = 0; i < latitudes.length; i++) {
            double euclidean = calculateEuclideanDistance(latitudes[i], longitudes[i]);
            // Adjusted format string to include spaces for alignment
            System.out.printf("Euclidean distance: %9.4f%n", euclidean);
        }
        System.out.println("\nSorting completed based on Euclidean geometrical distance");
        System.out.println("\nTotal number of merges: " + getMergeCount());
    }
}

