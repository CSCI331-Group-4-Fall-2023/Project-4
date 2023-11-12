/// @file ZipCodeBuffer.cpp
/// @class ZipCodeBuffer
/// See ZipCodeBuffer.h for full documentation.

#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <sstream> 
#include "ZipCodeBuffer.h"
//#include "BlockBuffer.h"

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

/// @brief Parses a string into a ZipCodeRecord struct.
ZipCodeRecord ZipCodeBuffer::parseRecord(std::string recordString) {
    ZipCodeRecord record;

    std::istringstream recordStream(recordString);
    std::string field;

    // Parse the record fields using istringstream 
    std::vector<std::string> fields;
    while (getline(recordStream, field, ',')) {
        fields.push_back(field);
    }

    if (fields.size() == 6)
    {
        // Fill the ZipCodeRecord struct with the data from the record
        record.zipCode = fields[0];
        record.placeName = fields[1];
        record.state = fields[2];
        record.county = fields[3];
        record.latitude = std::stod(fields[4]);  // Convert string to double 
        record.longitude = std::stod(fields[5]);
    }
    else
    {
        // The record is malformed and does not have 6 fields. Return terminal string
        record.zipCode = "";
        std::cerr << "A record contains an invalid number of fields: "
            << recordString << std::endl;
    }

    return record;
};

/// @brief Reads the next ZIP Code record from the file.
ZipCodeRecord ZipCodeBuffer::readNextRecord() {
    ZipCodeRecord record;
    std::string recordString;

    /*
    if (fileType == 'B')
    {
        BlockBuffer blockBuffer; // TODO can it construct an empty buffer like this?
        // TODO if it can, then it should be stored in the class rather than in this scope

        if (blockRecordsIndex > blockRecords.size())
        {
            // Reached the end of the block, so retrieve the next one
            
            // TODO needs block buffer to retrieve the next block and store the vector in blockRecords
        }
        
    }
    */
    
    if (fileType == 'C')
    {
        // If CSV, retrieve the next line in the file as the record to parse
        getline(file, recordString);
    }
    else if (fileType == 'L')
    {
        // If length-indicated, reads the length and retrieves that many characters for the record
        int numCharactersToRead = 0;
        file >> numCharactersToRead;   // Read the length indicator, the first field in each record
        file.ignore(1);                // Skip the comma after the length field
        recordString.resize(numCharactersToRead);
        file.read(&recordString[0], numCharactersToRead);
    }

    if (!file.eof()) {
        // If not the end of the file, read the fields in the line into the record object
        record = parseRecord(recordString);
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