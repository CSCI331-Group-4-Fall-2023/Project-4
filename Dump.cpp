// PrintBlock.cpp

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

void Dump::dumpLogicalOrder() {
    std::cout << "List Head RBN: " << headerBuffer.getRBNS() << std::endl;
    std::cout << "Avail Head RBN: " << headerBuffer.getRBNA() << std::endl;

    while (true)
    {
        std::vector<std::string> records = blockBuffer.readNextBlock();
        if (records.size() == 0)
        {
            break;
        }
        
        printBlock(records);
    }
}

void Dump::dumpPhysicalOrder() {
    std::cout << "List Head RBN: " << headerBuffer.getRBNS() << std::endl;
    std::cout << "Avail Head RBN: " << headerBuffer.getRBNA() << std::endl;

    int i = 0;
    int endpoint = headerBuffer.getBlockCount();
    while (i < endpoint)
    {
        std::vector<std::string> records = blockBuffer.readBlock(i++);
        
        printBlock(records);
    }
}

void Dump::printBlock(std::vector<std::string> records) {
    
    std::cout << std::left << std::setw(6) << blockBuffer.getPrevRBN();
    for (const std::string& recordString : records) {
        ZipCodeRecord record = recordBuffer.parseRecord(recordString);
        std::cout << std::setw(6) << record.zipCode;
    }
    std::cout << std::left << std::setw(6) << blockBuffer.getNextRBN();
    std::cout << std::endl;
}



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
