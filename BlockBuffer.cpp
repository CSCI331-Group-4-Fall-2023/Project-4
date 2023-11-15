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
    // TODO change to RBN starting at 1 instead of 0 if needed
    return headerSize + relativeBlockNumber*blockSize;
}



/// @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
void BlockBuffer::moveToBlock(int relativeBlockNumber) {
    int address = calculateBlockAddress(relativeBlockNumber);
    file.seekg(address);
}



/// @brief Reads the next block and returns it as a vector of records in string form.
vector<string> BlockBuffer::readNextBlock() {
    vector<string> recordStrings;
    std::string line;


    // If the next RBN is -1, the end of the chain has been reached.
    if (nextRBN == -1)
    {
        currentRBN = -1;
        file.ignore(1); // Ignore a newline at the end of the file, if any
        return recordStrings;
    }
    
    moveToBlock(nextRBN);               // Move to the next block
    readBlockMetadata();                // Read the metadata for the block

    recordStrings = unpackBlockRecords(); // Split the block into record strings
    //std::getline(file, line);   // Read the remainder of the line to get to the next physical block

    return recordStrings;
}
