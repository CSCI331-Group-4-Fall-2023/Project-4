/// @file ZipCodeBuffer.cpp
/// @class ZipCodeBuffer
/// See ZipCodeBuffer.h for full documentation.

#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <sstream> 
#include "ZipCodeBuffer.h"
#include "BlockBuffer.h"

/// @brief Constructor that accepts the filename.
ZipCodeBuffer::ZipCodeBuffer(std::ifstream &file, char fileType) : file(file), fileType(std::toupper(fileType)), blockBuffer(BlockBuffer(file)) {
    if (this->fileType == 'C') {
        // If CSV, skip the header line.
        std::string line;
        getline(file, line); // Skip header line
    }
    else if (this->fileType == 'L' || this->fileType == 'B') {
        // If length-indicated or blocked length-indicated file, skip the header line.
        std::string line;
        getline(file, line); // Skip header line
        // TODO skip the header record instead once implemented
    }
};

// @brief Destructor to close the file when done.
/*
ZipCodeBuffer::~ZipCodeBuffer() {
    if (file.is_open()) {
        file.close();
    }
};
*/

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

    if (file.eof())
    {
        // End of file reached. Return terminal character
        record.zipCode = "";
        return record;
    }
    
    
    if (fileType == 'B')
    {
        if (blockRecordsIndex >= blockRecords.size() || blockRecordsIndex == -1)
        {
            // Reached the end of the block, so retrieve the next one
            blockRecords = blockBuffer.readNextBlock();
            if (blockRecords.size() > 0)
            {
                recordString = blockRecords[0]; // Retrieve the first record in the block
                blockRecordsIndex = 1;          // Skip 0 because it reads it immediately
            }
            else
            {
                // Did not read a valid block (likely due to the end of file), so return terminal character
                record.zipCode = "";
                return record;
            }
        }
        else
        {
            recordString = blockRecords[blockRecordsIndex++];
        }
        
        
    }
    else if (fileType == 'C')
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

    // If not the end of the file, read the fields in the line into the record object
    record = parseRecord(recordString);
    
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