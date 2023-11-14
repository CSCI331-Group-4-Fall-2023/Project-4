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

vector<string> BlockBuffer::createRecords() {
    // This will convert a block to a vector of records
    size_t idx = 0;
    vector<string> records;
    // int currentRecordLength;


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

    // TODO throw exception if any of these reads failed

    currentRBN = newRelativeBlockNumber;
    numRecordsInBlock = newNumRecordsInBlock;
    prevRBN = newPrevRBN;
    nextRBN = newNextRBN;
}



/// @brief 
int BlockBuffer::calculateBlockAddress(int relativeBlockNumber) {
    // TODO change to RBN starting at 1 instead of 0 if needed
    return headerSize + relativeBlockNumber*blockSize;
}


/// @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
void BlockBuffer::moveToBlock(int relativeBlockNumber) {
    int address = calculateBlockAddress(relativeBlockNumber);
    file.seekg(address);
}




vector<string> BlockBuffer::readNextBlock() {
    // TODO read block metadata, including RBN links, to find the next line (and use ZipCodeBuffer.setCurrentPosition())
    // TODO needs to handle finding the first block in the file when there is no current block
    // This will require metadata from the header (the length of the header record)
    
    std::string line;
    readBlockMetadata(); // TODO should instead pass string stream or something, change how the block is stored
    std::getline(file, line);
    setBlock(line);

    // If the next RBN is -1, the end of the chain has been reached.
    if (nextRBN == -1)
    {
        currentRBN = nextRBN;
    }
    
    calculateBlockAddress(nextRBN);
    
    return createRecords();

    // TODO read links, navigate to them
}
