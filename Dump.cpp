/// @file Dump.cpp
/// @brief Implementation of the Dump class methods for printing and dumping records.
/// See Dump.h for the class declaration and documentation.

#include "Dump.h"
#include <iostream>
#include <iomanip>
#include "ZipCodeBuffer.h"
#include "BlockBuffer.h"
#include "HeaderBuffer.h"

#include <string>
#include <vector>
#include <sstream> 
#include <fstream>
using namespace std;

Dump::Dump(ZipCodeBuffer &recordBuffer) : recordBuffer(recordBuffer), blockBuffer(recordBuffer.blockBuffer), headerBuffer(recordBuffer.headerBuffer) {
    // The constructor takes an ifstream and HeaderBuffer, initializing the BlockBuffer
    // with the provided file and headerBuffer.
    headerBuffer.readHeader();
}



/// @brief Dump records in logical order.
void Dump::dumpLogicalOrder() {
    // Display the list head Relative Block Numbers
    std::cout << "List Head RBN: " << headerBuffer.getRBNS() << std::endl;
    std::cout << "Avail Head RBN: " << headerBuffer.getRBNA() << std::endl;

    while (true)
    {
        std::vector<std::string> records = blockBuffer.readNextBlock();
        if (records.size() == 0) // TODO could change to when it reads -1 as next RBN to skip a step
        {
            // When it receives an empty vector, end the loop
            break;
        }
        
        printBlock(records);
    }
}



/// @brief Dump records in physical order.
void Dump::dumpPhysicalOrder() {
    // Display the list head Relative Block Numbers
    std::cout << "List Head RBN: " << headerBuffer.getRBNS() << std::endl;
    std::cout << "Avail Head RBN: " << headerBuffer.getRBNA() << std::endl;

    int i = 0;
    int endpoint = headerBuffer.getBlockCount();
    while (i < endpoint)
    {
        // Read the number of blocks listed in the file metadata
        std::vector<std::string> records = blockBuffer.readBlock(i++);
        printBlock(records);
    }
}



/// @brief Print a block of records.
void Dump::printBlock(std::vector<std::string> records) {
    
    std::cout << std::left << std::setw(6) << blockBuffer.getPrevRBN(); // Display preceding RBN
    for (const std::string& recordString : records) {
        ZipCodeRecord record = recordBuffer.parseRecord(recordString);
        std::cout << std::setw(6) << record.zipCode;                    // Display the key (zipCode) for each record in the block
    }
    std::cout << std::left << std::setw(6) << blockBuffer.getNextRBN(); // Display succeeding RBN
    std::cout << std::endl;
}



/// @brief Dump the block index from a file.
void Dump::dumpBlockIndex(const std::string& filename) {
    std::ifstream mainFile(filename);
    if (!mainFile.is_open()) {
        std::cerr << "Error opening index file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(mainFile, line)) {
        std::istringstream ss(line);
        std::vector<std::string> tokens;

        // Split the line by comma and store tokens in the vector
        while (std::getline(ss, line, ',')) {
            tokens.push_back(line);
        }
        std::cout << "RBN: ";
        int key = 0;
        // Print each value in the tokens vector
        for (const auto& token : tokens) {
            std::cout << token << " ";
            if(key == 0){
                std::cout << "Primary Key: ";
                key = 1;
            }else{
                key = 0;
            }

        }

        std::cout << "\n";
    }
}
