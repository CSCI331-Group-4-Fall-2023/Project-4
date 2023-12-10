/// @file TreeBlockBuffer.cpp
/// @class TreeBlockBuffer
/// See TreeBlockBuffer.h for full documentation.

#include <iostream>
#include <fstream> // for file operations
#include <string>
#include <vector>
#include "TreeBlockBuffer.h"
#include "HeaderBuffer.h"
#include <sstream>



TreeBlockBuffer::TreeBlockBuffer(std::ifstream &file, HeaderBuffer headerBuffer) : file(file) {
    headerBuffer.readHeader();
    headerSize = headerBuffer.getHeaderSizeBytes();
    blockSize = headerBuffer.getBlockSize();
    nextRBN = headerBuffer.getRBNS();

    // TODO call readBlock on new metadata field value for where the index tree begins
}


vector<string> TreeBlockBuffer::unpackBlockRecords() {
    // This will convert a block to a vector of records
    size_t idx = 0;
    vector<string> records;

    for (size_t i = 0; i < getNumRecordsInBlock(); i++)
    {
        // Reads the length and retrieves that many characters for the record
        std::string recordString;
        int numCharactersToRead = 0;
        file >> numCharactersToRead;   // Read the length indicator, the first field in each record
        file.ignore(1);                // Skip the comma after the length field
        recordString.resize(numCharactersToRead);
        file.read(&recordString[0], numCharactersToRead);
        records.push_back(recordString);
    }
    
    return records;
}



/// @brief Reads the block metadata for the current block.
void TreeBlockBuffer::readBlockMetadata() {
    int metadataRecordLength = -1;
    int newRelativeBlockNumber = -1;
    int newNumRecordsInBlock = -1;
    int newPrevRBN = -1;
    int newNextRBN = -1;
    
    file >> metadataRecordLength;
    file.ignore(1); // Ignore the commas separating the fields
    file >> newRelativeBlockNumber;
    file.ignore(1);
    file >> newNumRecordsInBlock;
    file.ignore(1);
    file >> newPrevRBN;
    file.ignore(1);
    file >> newNextRBN;
    file.ignore(1); // Skip the comma after the last metadata field

    // TODO throw exception if any of these reads failed or the values are invalid

    currentRBN = newRelativeBlockNumber;
    numRecordsInBlock = newNumRecordsInBlock;
    prevRBN = newPrevRBN;
    nextRBN = newNextRBN;
}



/// @brief Calculates the address of a Relative Block Number (RBN) within the file.
int TreeBlockBuffer::calculateBlockAddress(int relativeBlockNumber) {
    return headerSize + relativeBlockNumber*blockSize;
}



/// @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
void TreeBlockBuffer::moveToBlock(int relativeBlockNumber) {
    int address = calculateBlockAddress(relativeBlockNumber);
    file.seekg(address);
}



/// @brief Reads the block at the given Relative Block Number (RBN) and returns it as a vector of records in string form.
vector<string> TreeBlockBuffer::readBlock(int relativeBlockNumber) {
    vector<string> recordStrings;
    std::string line;

    // If the RBN is -1, the end of the chain has been reached.
    if (relativeBlockNumber == -1)
    {
        currentRBN = -1;
        return recordStrings;
    }
    
    moveToBlock(relativeBlockNumber);               // Move to the next block
    return readCurrentBlock();          // Read the metadata and the records
}



/// @brief Reads the current block and returns it as a vector of records in string form.
vector<string> TreeBlockBuffer::readCurrentBlock() {
    readBlockMetadata();                // Read the metadata for the block
    return unpackBlockRecords();        // Read the length-indicated records into strings and return them
}


vector<string> TreeBlockBuffer::readNextBlock() {
    return readBlock(nextRBN);
}
