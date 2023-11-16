/// @file BlockBuffer.cpp
/// @class BlockBuffer
/// See BlockBuffer.h for full documentation.

#include <iostream>
#include <fstream> // for file operations
#include <string>
#include <vector>
#include "BlockBuffer.h"
//#include "ZipCodeBuffer.h"
#include <sstream>

using namespace std;

vector<string> BlockBuffer::unpackBlockRecords() {
    // This will convert a block to a vector of records
    size_t idx = 0;
    vector<string> records;

    for (size_t i = 0; i < getNumRecordsInBlock(); i++)
    {
        // If length-indicated, reads the length and retrieves that many characters for the record
        std::string recordString;
        int numCharactersToRead = 0;
        file >> numCharactersToRead;   // Read the length indicator, the first field in each record
        file.ignore(1);                // Skip the comma after the length field
        recordString.resize(numCharactersToRead);
        file.read(&recordString[0], numCharactersToRead);
        records.push_back(recordString);
    }

    // This code was not working for me, but it may have features that should still be implemented
    /*
    while (idx < block.length()) {
        // Reached filler characters of block (end of block)
        if (block[idx] == '~') {
            break;
        }
        
        // Checking for invalid record length
        if (idx + 2 > block.length()) {
            std::cerr << "Error: Block ends unexpectedly, or index operations errored in BlockBuffer.cpp.\n";
            break;
        }

        try {
            int currentRecordLength = std::stoi(block.substr(idx, 2));
            if (idx + 3 + currentRecordLength > block.length()) {
                std::cerr << "Error: Record length exceeds block boundary.\n";
                break;
            }
            std::string currentRecord = block.substr(idx, currentRecordLength);
            records.push_back(currentRecord);
            idx += 3 + currentRecordLength; // Move to the start of the next record (3 represents length indicator and comma)
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Could not convert record length to integer.\n";
            break;
        }

    }
    */
    
    return records;
}



/// @brief Reads the block metadata for the current block.
void BlockBuffer::readBlockMetadata() {
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
int BlockBuffer::calculateBlockAddress(int relativeBlockNumber) {
    return headerSize + relativeBlockNumber*blockSize;
}



/// @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
void BlockBuffer::moveToBlock(int relativeBlockNumber) {
    int address = calculateBlockAddress(relativeBlockNumber);
    file.seekg(address);
}



/// @brief Moves to and reads the next block and returns it as a vector of records in string form.
vector<string> BlockBuffer::readBlock(int relativeBlockNumber) {
    vector<string> recordStrings;
    std::string line;

    // If the RBN is -1, the end of the chain has been reached.
    if (relativeBlockNumber == -1)
    {
        currentRBN = -1;
        return recordStrings;
    }
    
    moveToBlock(nextRBN);               // Move to the next block
    return readCurrentBlock();          // Read the metadata and the records
}



/// @brief Reads the current block and returns it as a vector of records in string form.
vector<string> BlockBuffer::readCurrentBlock() {
    readBlockMetadata();                // Read the metadata for the block
    return unpackBlockRecords();        // Read the length-indicated records into strings and return them
}


vector<string> BlockBuffer::readNextBlock() {
    return readBlock(nextRBN);
}
