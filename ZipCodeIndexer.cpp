/// @file ZipCodeIndexer.cpp
/// @class ZipCodeIndexer
/// @brief Implementation of the ZipCodeIndexer class for indexing ZIP code records in a file.

#include "ZipCodeIndexer.h"

/// @brief Constructor for the ZipCodeIndexer class.
// Initializes the buffer object with the given file name
// and sets the index file name
/// @param fileName The name of the  file to index as a string.
/// @param idxFileName The name of the index file to save/load as a string.
ZipCodeIndexer::ZipCodeIndexer(const std::string& fileName, char fileType, const std::string& idxFileName)
    : buffer(fileName, fileType), indexFileName(idxFileName) {}

/// @brief Create an index of ZIP codes to their positions in the file.
// This function creates an index of ZIP codes to their positions in the file
// by reading each record in the file using the buffer.
void ZipCodeIndexer::createIndex() {
    ZipCodeRecord record;
    std::streampos position = buffer.getCurrentPosition();
    while (!(record = buffer.readNextRecord()).zipCode.empty()) {
        index[record.zipCode] = position; // Save the position of this ZIP code in the index
        position = buffer.getCurrentPosition(); // Get the position of the next record
    }
}

/// @brief Write the created index to a file.
/// This function writes the created index to a file. Each line in the file contains a ZIP code and its position in the file.
void ZipCodeIndexer::writeIndexToFile() {
    std::ofstream outFile(indexFileName);
    for (const auto& pair : index) {
        outFile << pair.first << " " << pair.second << "\n"; // ZIP code and position
    }
    outFile.close();
}

/// @brief Load the index from a file into RAM.
// This function loads the index from a file into RAM. The index is stored in a std::map
// where the key is the ZIP code and the value is its position in the file.
void ZipCodeIndexer::loadIndexFromRAM() {
    index.clear(); // Clear any existing index
    std::ifstream inFile(indexFileName);
    std::string zip;
    std::streampos pos;
    int posInt;
    while (inFile >> zip >> posInt) {
        pos = posInt;
        index[zip] = pos; // Load the ZIP code and its position into the index
    }
    inFile.close();
}

/// @brief Get the position in the file of the given ZIP code.
// If the ZIP code is not in the index, it returns an invalid position (-1).
/// @param zipCode The ZIP code to find the position of.
/// @return The position of the ZIP code record in the file. If not found, returns an invalid position (-1).
std::streampos ZipCodeIndexer::getRecordPosition(const std::string& zipCode) {
    if (index.find(zipCode) != index.end()) { // If the ZIP code is in the index
        return index[zipCode]; // Return its position
    }
    else {
        return std::streampos(-1);  // Invalid position to indicate not found
    }
}
