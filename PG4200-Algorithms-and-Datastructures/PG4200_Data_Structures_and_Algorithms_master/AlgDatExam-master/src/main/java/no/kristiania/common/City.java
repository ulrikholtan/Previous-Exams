package no.kristiania.common;

// Represents a city with a name, latitude, and longitude
public record City(String name, double latitude, double longitude) {

    // Getters for the record fields
    @Override
    public String name() {
        return name;
    }

    @Override
    public double latitude() {
        return latitude;
    }

    @Override
    public double longitude() {
        return longitude;
    }

    // Override the toString method to provide a custom string representation
    @Override
    public String toString() {
        return "City{" +
                "name='" + name + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                '}';
    }

    // Convenience method to get the latitude and longitude value as a double
    public double getLatitude() {
        return latitude;
    }
    public double getLongitude() {
        return longitude;
    }
}
