package no.kristiania.problem1_MergeSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class C_MSByGeographicalPoints {
    private static int mergeCount; // Counter to track the number of merge operations
    private static final double EARTH_RADIUS_KM = 6371.0; // Earth's radius in kilometers

    public static void sort(double[] latitudes, double[] longitudes) {
        if (latitudes == null || longitudes == null || latitudes.length <= 1 || latitudes.length != longitudes.length) {
            return; // Invalid input
        }

        // Calculate distances using Haversine formula
        double[] distances = new double[latitudes.length];
        for (int i = 0; i < latitudes.length; i++) {
            distances[i] = calculateHaversineDistance(latitudes[i], longitudes[i]);
        }

        mergeCount = 0; // Reset merge count
        mergeSort(distances, latitudes, longitudes, 0, distances.length - 1); // Start merge sort
    }

    private static double calculateHaversineDistance(double lat1, double lon1) {
        double lat1Rad = Math.toRadians(lat1);
        double lon1Rad = Math.toRadians(lon1);

        double lat0Rad = Math.toRadians(0.0); // Assume reference point is the equator and prime meridian (0, 0)
        double lon0Rad = Math.toRadians(0.0);

        double deltaLat = lat1Rad - lat0Rad;
        double deltaLon = lon1Rad - lon0Rad;

        double a = Math.sin(deltaLat / 2) * Math.sin(deltaLat / 2) +
                Math.cos(lat0Rad) * Math.cos(lat1Rad) *
                        Math.sin(deltaLon / 2) * Math.sin(deltaLon / 2);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        return EARTH_RADIUS_KM * c; // Return the distance in kilometers
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
        mergeCount++;

        int n1 = mid - left + 1; // Size of the left subarray
        int n2 = right - mid; // Size of the right subarray

        double[] LDistances = new double[n1];
        double[] RDistances = new double[n2];
        double[] LLatitudes = new double[n1];
        double[] RLatitudes = new double[n2];
        double[] LLongitudes = new double[n1];
        double[] RLongitudes = new double[n2];

        System.arraycopy(distances, left, LDistances, 0, n1);
        System.arraycopy(distances, mid + 1, RDistances, 0, n2);
        System.arraycopy(latitudes, left, LLatitudes, 0, n1);
        System.arraycopy(latitudes, mid + 1, RLatitudes, 0, n2);
        System.arraycopy(longitudes, left, LLongitudes, 0, n1);
        System.arraycopy(longitudes, mid + 1, RLongitudes, 0, n2);

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

        while (i < n1) {
            distances[k] = LDistances[i];
            latitudes[k] = LLatitudes[i];
            longitudes[k] = LLongitudes[i];
            i++;
            k++;
        }

        while (j < n2) {
            distances[k] = RDistances[j];
            latitudes[k] = RLatitudes[j];
            longitudes[k] = RLongitudes[j];
            j++;
            k++;
        }
    }

    public static int getMergeCount() {
        return mergeCount;
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";

        DataReader csvReader = new DataReader();
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        // Creates an array of latitudes and longitudes from the list of cities
        double[] latitudes = new double[cities.size()];
        double[] longitudes = new double[cities.size()];
        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).getLatitude();
            longitudes[i] = cities.get(i).getLongitude();
        }

        // Sorting with Merge Sort
        sort(latitudes, longitudes);

        // Print the sorted data
        System.out.println("\nSorted Cities based on Geographical Points:");
        System.out.println("\nSorted data (Lat, Long, Geographical Point):");
        for (int i = 0; i < latitudes.length; i++) {
            double haversine = calculateHaversineDistance(latitudes[i], longitudes[i]);
            System.out.printf("Geographical Point: %9.4f%n", haversine);
        }
        System.out.println("\nSorting completed based on geographical kilometers from point Origo");
        System.out.println("\nTotal number of merges: " + getMergeCount());
    }
}
