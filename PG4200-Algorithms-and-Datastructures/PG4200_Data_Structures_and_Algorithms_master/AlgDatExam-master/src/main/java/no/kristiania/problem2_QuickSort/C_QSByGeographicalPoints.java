package no.kristiania.problem2_QuickSort;

import no.kristiania.common.City;
import no.kristiania.common.DataReader;

import java.util.List;

public class C_QSByGeographicalPoints {

    private int comparisonCount = 0;  // Counter to track the number of comparisons

    private static final double EARTH_RADIUS_KM = 6371.0; // Earth's radius in kilometers

    private int partition(double[] latitudes, double[] longitudes, double[] distances, int low, int high) {
        double pivot = distances[high]; // Velg pivot som siste element
        int i = low - 1; // Indeks for mindre element

        for (int j = low; j < high; j++) {
            comparisonCount++;  // Increment the comparison count
            // If the current element is smaller than or equal to the pivot
            if (distances[j] <= pivot) {
                i++;
                // Swap latitudes
                double tempLat = latitudes[i];
                latitudes[i] = latitudes[j];
                latitudes[j] = tempLat;
                // Swap longitudes
                double tempLong = longitudes[i];
                longitudes[i] = longitudes[j];
                longitudes[j] = tempLong;
                // Swap distances
                double tempDistance = distances[i];
                distances[i] = distances[j];
                distances[j] = tempDistance;
            }
        }

        // Swap the pivot element to its correct position
        double tempLat = latitudes[i + 1];
        latitudes[i + 1] = latitudes[high];
        latitudes[high] = tempLat;

        double tempLong = longitudes[i + 1];
        longitudes[i + 1] = longitudes[high];
        longitudes[high] = tempLong;

        double tempDistance = distances[i + 1];
        distances[i + 1] = distances[high];
        distances[high] = tempDistance;

        return i + 1; // Return the partitioning index
    }

    private void sort(double[] latitudes, double[] longitudes, double[] distances, int low, int high) {
        if (low < high) {
            int pi = partition(latitudes, longitudes, distances, low, high);
            sort(latitudes, longitudes, distances, low, pi - 1);
            sort(latitudes, longitudes, distances, pi + 1, high);
        }
    }

    public void sort(double[] latitudes, double[] longitudes, double[] distances) {
        sort(latitudes, longitudes, distances, 0, latitudes.length - 1);
    }

    private static double calculateHaversineDistance(double lat1, double lon1) {
        double lat1Rad = Math.toRadians(lat1);
        double lon1Rad = Math.toRadians(lon1);
        double lat0Rad = Math.toRadians(0.0); // Assume the reference point (0,0)
        double lon0Rad = Math.toRadians(0.0);

        double deltaLat = lat1Rad - lat0Rad;
        double deltaLon = lon1Rad - lon0Rad;

        double a = Math.sin(deltaLat / 2) * Math.sin(deltaLat / 2) +
                Math.cos(lat0Rad) * Math.cos(lat1Rad) *
                        Math.sin(deltaLon / 2) * Math.sin(deltaLon / 2);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        return EARTH_RADIUS_KM * c; // Return the distance in kilometers
    }

    public static void main(String[] args) {
        String csvFilePath = "src/main/resources/worldcities.csv";
        DataReader csvReader = new DataReader();
        List<City> cities = csvReader.readCitiesFromCSV(csvFilePath);

        double[] latitudes = new double[cities.size()];
        double[] longitudes = new double[cities.size()];
        double[] distances = new double[cities.size()];

        for (int i = 0; i < cities.size(); i++) {
            latitudes[i] = cities.get(i).latitude();
            longitudes[i] = cities.get(i).longitude();
            distances[i] = calculateHaversineDistance(latitudes[i], longitudes[i]);
        }

        C_QSByGeographicalPoints quickSort = new C_QSByGeographicalPoints();
        quickSort.sort(latitudes, longitudes, distances);

        System.out.println("\nSorted Cities based on Geographical Points:");
        System.out.println("\nSorted data (Lat, Long, Geographical Point):");
        for (int i = 0; i < latitudes.length; i++) {
            System.out.printf("Geographical Point: %9.4f%n", distances[i]);
        }
        System.out.println("\nSorting completed based on geographical kilometers from point Origo");
        System.out.println("\nTotal number of comparisons: " + quickSort.comparisonCount);
    }
}
