package no.kristiania.common;

import java.util.Random;

public class ListShuffler {

    // Function to shuffle the list of cities
    public static void shuffleLists(double[] latitudes) {
        Random rnd = new Random();
        for (int i = latitudes.length - 1; i > 0; i--) {
            int index = rnd.nextInt(i + 1);
            // Swap latitudes
            double tempLatitude = latitudes[index];
            latitudes[index] = latitudes[i];
            latitudes[i] = tempLatitude;
        }
    }
}
