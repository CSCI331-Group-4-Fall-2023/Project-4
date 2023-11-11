// Program to convert a CSV file to the length-indicated file structure.

#include <iostream>
#include <fstream>
#include <string>

int convertCSV() {
    std::ifstream inputFile("us_postal_codes.csv");  // Open the input file
    std::ofstream outputFile("us_postal_codes.txt");  // Open the output file

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::string record;
    int recordNumber = 1;

    std::getline(inputFile, record); // Skip the header line

    while (std::getline(inputFile, record)) {
        // Calculate the length of the record
        int recordLength = record.length();

        outputFile << recordLength << ',' << record << std::endl;

        ++recordNumber;
    }

    inputFile.close();  // Close the input file
    outputFile.close();  // Close the output file

    std::cout << "Prepended length field to " << recordNumber - 1 << " records." << std::endl;
}

// Testing GitHub