/// See ZipCodeRecordSearch.h for details.

#include <iostream>
#include <string>
#include <stdexcept>
#include "ZipCodeBuffer.h"
#include "ZipCodeRecordSearch.h"
#include "ZipCodeIndexer.h"

/**
 * @brief Checks to see if a given string is a number.
 * 
 * @param s 
 * @return true
 * @return false 
 */
bool isNumber(const char* s) {
  try{
    int num = std::stoi(std::string(s));
    return true;
  } catch (std::invalid_argument& e) {
    return false;
  }
}

/**
 * @brief Prints usage information to the console
 * 
 * @param commandName 
 */
void displayHelp(const std::string& commandName) {
  std::cout << std::endl
    << "Usage: " << commandName << " [options]" << std::endl
    << "-h, --help            Show help options" << std::endl
    << "-Z <zipcode>," << std::endl
    << "--zipcode <zipcode>   Search record file for <zipcode>" << std::endl << std::endl;
}

/**
 * @brief Prints basic message to the console
 * 
 * @param commandName 
 */
void defaultMessage(const std::string& commandName) {
  std::cout << std::endl
    << "Try \"" << commandName << " -h\" for more information." << std::endl << std::endl;
}

/**
 * @brief Helper function to search the buffer for a given ZIP code
 * 
 * @param fileName Name of the file to open.
 * @param fileType Type of the file to open to pass to the indexer for the buffer.
 * @param zip ZIP codes to search for, if any.
 */
void searchHelper(std::string fileName, char fileType, char* zip) {
    // Create an index and load it from the index file
    std::ifstream file(fileName);
    ZipCodeIndexer index(file, fileType, fileName + "_index.txt");
    index.loadIndexFromRAM();

    // Get the position of the ZIP code in the file
    std::streampos position = index.getRecordPosition(zip);

    if (position != std::streampos(-1)) {
        // Open the buffer and set the position
        ZipCodeBuffer buffer(file, fileType);
        buffer.setCurrentPosition(position);

        // Read the record at the specified position
        ZipCodeRecord record = buffer.readNextRecord();

        // Print the record info
        std::cout << "Zip Code: " << record.zipCode << std::endl
            << "Place Name: " << record.placeName << std::endl
            << "State: " << record.state << std::endl
            << "County: " << record.county << std::endl
            << "Latitude: " << record.latitude << " Longitude: " << record.longitude
            << std::endl << std::endl;
    }
    else {
        // If record not found, print a message
        std::cout << "No record of " << zip << std::endl << std::endl;
    }
}
