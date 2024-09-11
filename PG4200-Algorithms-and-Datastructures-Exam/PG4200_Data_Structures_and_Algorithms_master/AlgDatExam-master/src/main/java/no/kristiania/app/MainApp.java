package no.kristiania.app;

import no.kristiania.problem1_MergeSort.A_MSByLatitude;
import no.kristiania.problem1_MergeSort.B_MSCompareNumberOfMerges;
import no.kristiania.problem1_MergeSort.C_MSByEculideanDistance;
import no.kristiania.problem1_MergeSort.C_MSByGeographicalPoints;
import no.kristiania.problem2_QuickSort.A_QSByLatitude;
import no.kristiania.problem2_QuickSort.B_QSCompareNumberOfComparisons;
import no.kristiania.problem2_QuickSort.C_QSByEuclideanDistance;
import no.kristiania.problem2_QuickSort.C_QSByGeographicalPoints;

import java.util.Scanner;

public class MainApp {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean running = true; // To keep the program running

        while (running) { // Keep the program running until the user chooses to terminate it
            System.out.println("\nWelcome to the AlgDat Exam main menu for Group 1");
            System.out.println("Please choose a task you want to perform:");
            System.out.println("\n1. Problem 1A - MergeSort - By latitude");
            System.out.println("2. Problem 1B - MergeSort - Count and compare number of merges");
            System.out.println("3. Problem 1C - MergeSort - Latitude & longitude (Euclidean Distance)");
            System.out.println("4. Problem 1C - MergeSort - Latitude & longitude (Geographical Points)");
            System.out.println("5. Problem 2A - QuickSort - By latitude");
            System.out.println("6. Problem 2B - QuickSort - Count and compare number of comparisons");
            System.out.println("7. Problem 2C - QuickSort - Latitude & longitude (Euclidean Distance)");
            System.out.println("8. Problem 2C - QuickSort - Latitude & longitude (Geographical Points)");
            System.out.println("9. Terminate the program");

            System.out.print("\nType the number of the task you want to run and press enter: ");
            int choice = scanner.nextInt();
            scanner.nextLine(); // Clean the scanner buffer

            switch (choice) {
                case 1:
                    A_MSByLatitude.main(null);  // Run Problem 1A
                    break;
                case 2:
                    B_MSCompareNumberOfMerges.main(null); // Run Problem 1B
                    break;
                case 3:
                    C_MSByEculideanDistance.main(null); // Run Problem 1C with Euclidean Distance
                    break;
                case 4:
                    C_MSByGeographicalPoints.main(null); // Run Problem 1C with Geographical Points
                    break;
                case 5:
                    A_QSByLatitude.main(null); // Run Problem 2A
                    break;
                case 6:
                    B_QSCompareNumberOfComparisons.main(null);  // Run Problem 2B
                    break;
                case 7:
                    C_QSByEuclideanDistance.main(null); // Run Problem 2C with Euclidean Distance
                    break;
                case 8:
                    C_QSByGeographicalPoints.main(null); // Run Problem 2C with Geographical Points
                    break;
                case 9:
                    running = false; // Exit the program
                    System.out.println("The program is terminating. Have a nice day!");
                    break;
                default:
                    System.out.println("Invalid choice, please try again.");
                    continue; // Jump back to the beginning of the loop
            }

            if (running) { // If the program is still running after the task has been executed
                System.out.println("\nPress enter to return to the main menu.");
                scanner.nextLine(); // Wait for user input before returning to the main menu
            }
        }

        scanner.close(); // Close the scanner when the program terminates
    }

}