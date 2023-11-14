/// @file BlockBuffer.cpp
/// @class BlockBuffer
/// See BlockBuffer.h for full documentation.

#include <iostream>
#include <fstream> // for file operations
#include <string>
#include <vector>
#include "BlockBuffer.h"
//#include "ZipCodeBuffer.h"

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

vector<string> BlockBuffer::readNextBlock(ifstream &file) {
    // TODO read block metadata, including RBN links, to find the next line (and use ZipCodeBuffer.setCurrentPosition())
    // TODO needs to handle finding the first block in the file when there is no current block
    std::string line;
    std::getline(file, line);
    setBlock(line);
    return createRecords();
}
