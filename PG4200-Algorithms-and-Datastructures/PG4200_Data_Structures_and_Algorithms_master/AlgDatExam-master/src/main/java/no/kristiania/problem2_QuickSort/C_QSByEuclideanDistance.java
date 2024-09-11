package no.kristiania.problem2_QuickSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class C_QSByEuclideanDistance {

    private int comparisonCount = 0;  // Counter to track the number of comparisons

    private int partition(double[] latitudes, double[] longitudes, double[] euclideanDistances, int low, int high) {
        double pivot = euclideanDistances[high]; // Choose the pivot as the last element
        int i = low - 1; // Index of the smaller element

        for (int j = low; j < high; j++) {
            comparisonCount++;  // Increment the comparison count
            // If the current element is smaller than or equal to the pivot
            if (euclideanDistances[j] <= pivot) {
                i++;
                // Swap latitudes
                double tempLat = latitudes[i];
                latitudes[i] = latitudes[j];
                latitudes[j] = tempLat;
                // Swap longitudes
                double tempLong = longitudes[i];
                longitudes[i] = longitudes[j];
                longitudes[j] = tempLong;
                // Swap euclidean distances
                double tempEuclidean = euclideanDistances[i];
                euclideanDistances[i] = euclideanDistances[j];
                euclideanDistances[j] = tempEuclidean;
            }
        }

        // Swap the pivot element to its correct position
        double tempLat = latitudes[i + 1];
        latitudes[i + 1] = latitudes[high];
        latitudes[high] = tempLat;

        double tempLong = longitudes[i + 1];
        longitudes[i + 1] = longitudes[high];
        longitudes[high] = tempLong;

        double tempEuclidean = euclideanDistances[i + 1];
        euclideanDistances[i + 1] = euclideanDistances[high];
        euclideanDistances[high] = tempEuclidean;

        return i + 1; // Return the partitioning index
    }

    private void sort(double[] latitudes, double[] longitudes, double[] euclideanDistances, int low, int high) {
        if (low < high) {
            // Partition the array and get the partitioning index
            int pi = partition(latitudes, longitudes, euclideanDistances, low, high);

            // Recursively sort elements before and after the partition
            sort(latitudes, longitudes, euclideanDistances, low, pi - 1);
            sort(latitudes, longitudes, euclideanDistances, pi + 1, high);
        }
    }

    public void sort(double[] latitudes, double[] longitudes, double[] euclideanDistances) {
        sort(latitudes, longitudes, euclideanDistances, 0, latitudes.length - 1);
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
            latitudes[i] = cities.get(i).latitude();
            longitudes[i] = cities.get(i).longitude();
        }

        // Calculate Euclidean distances
        double[] euclideanDistances = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            euclideanDistances[i] = Math.sqrt(latitudes[i] * latitudes[i] + longitudes[i] * longitudes[i]);
        }

        // Create an instance of C2QuickSort
        C_QSByEuclideanDistance quickSort = new C_QSByEuclideanDistance();

        // Sort based on Euclidean distances using quick sort
        quickSort.sort(latitudes, longitudes, euclideanDistances);

        // Output the sorted values
        System.out.println("\nSorted data (Lat, Long, Euclidean):");
        for (int i = 0; i < latitudes.length; i++) {
            System.out.printf("Euclidean distance: %9.4f%n", euclideanDistances[i]);
        }
        System.out.println("\nSorting completed based on Euclidean geometrical distance");
        System.out.println("\nTotal number of comparisons: " + quickSort.comparisonCount);
    }
}
