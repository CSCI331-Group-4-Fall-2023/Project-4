/// @file ZipCodeBuffer.cpp
/// @class ZipCodeBuffer
/// See ZipCodeBuffer.h for full documentation.

#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <sstream> 
#include "ZipCodeBuffer.h"

/// @brief Constructor that accepts the CSV filename.
ZipCodeBuffer::ZipCodeBuffer(std::string fileName, char fileType = 'L') : fileName(fileName), fileType(std::toupper(fileType)) {
    file.open(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
    }
    else if (this->fileType == 'C') {
        // If CSV, skip the header line.
        std::string line;
        getline(file, line); // Skip header line
    }
    else if (this->fileType == 'L') {
        // If length-indicated file, skip the header line.
        std::string line;
        getline(file, line); // Skip header line
        // TODO skip the header record instead once implemented
    }
};

/// @brief Destructor to close the file when done.
ZipCodeBuffer::~ZipCodeBuffer() {
    if (file.is_open()) {
        file.close();
    }
};

/// @brief Reads the next ZIP Code record from the file.
ZipCodeRecord ZipCodeBuffer::readNextRecord() {
    ZipCodeRecord record;
    std::string line;

    if (fileType == 'C')
    {
        getline(file, line);
    }
    else if (fileType == 'L')
    {
        int numCharactersToRead = 0;
        file >> numCharactersToRead;          // Read the length indicator, the first field in each record.
        file.ignore(1);                       // Skip the comma after the length field
        line.resize(numCharactersToRead);
        file.read(&line[0], numCharactersToRead);
    }

    if (!file.eof()) {
        std::istringstream ss(line);
        std::string token;

        // Parse the CSV record fields using istringstream 
        std::vector<std::string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 6)
        {
            // Fill the ZipCodeRecord struct with the data from the CSV record
            record.zipCode = tokens[0];
            record.placeName = tokens[1];
            record.state = tokens[2];
            record.county = tokens[3];
            record.latitude = std::stod(tokens[4]);  // Convert string to double 
            record.longitude = std::stod(tokens[5]);
        }
        else
        {
            // The record is malformed and does not have 6 fields. Return terminal character
            record.zipCode = "";
            std::cerr << "A record in the file contains an invalid number of fields: "
                << line << std::endl;
        }
    }
    else {
        // End of file reached. Return terminal character
        record.zipCode = "";
    }
    
    return record;
};

/// @brief Method to get the current position in the file.
std::streampos ZipCodeBuffer::getCurrentPosition() {
    return file.tellg();
}

/// @brief Method to set the current position in the file to a given streampos.
std::ifstream& ZipCodeBuffer::setCurrentPosition(std::streampos pos) {
    file.seekg(pos);
    return file;
}