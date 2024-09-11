package no.kristiania.common;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class DataReader {
    public List<City> readCitiesFromCSV(String csvFilePath) {
        List<City> cities = new ArrayList<>();
        boolean isFirstRow = true; // Flag to track whether the first row has been encountered

        try (BufferedReader reader = new BufferedReader(new FileReader(csvFilePath))) {
            String line;

            while ((line = reader.readLine()) != null) {
                if (isFirstRow) {
                    isFirstRow = false; // Skip the first row
                    continue; // Move to the next iteration
                }

                String[] columns = line.split(","); // Assuming CSV format

                if (columns.length >= 4) {
                    String name = columns[1].trim(); // Column 2
                    String latitudeStr = columns[2].replaceAll("^\"|\"$", "").trim(); // Remove leading and trailing quotes, then trim
                    String longitudeStr = columns[3].replaceAll("^\"|\"$", "").trim(); // Remove leading and trailing quotes, then trim

                    // Validate latitude and longitude values
                    if (!latitudeStr.isEmpty() && !longitudeStr.isEmpty()) {
                        try {
                            double latitude = Double.parseDouble(latitudeStr);
                            double longitude = Double.parseDouble(longitudeStr);

                            City city = new City(name, latitude, longitude);
                            cities.add(city);
                        } catch (NumberFormatException e) {
                            // Log or handle the invalid data
                            System.err.println("Error parsing latitude or longitude value. Row: " + line);
                            e.printStackTrace();
                        }
                    } else {
                        // Log or handle empty latitude or longitude values
                        System.err.println("Empty latitude or longitude value. Row: " + line);
                    }
                } else {
                    // Log or handle rows with insufficient columns
                    System.err.println("Row has insufficient columns. Row: " + line);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return cities;
    }
}