// PrintBlock.cpp

#include "PrintBlock.h"
#include <iostream>

PrintBlock::PrintBlock(std::ifstream& file, HeaderBuffer headerBuffer) : blockBuffer(file, headerBuffer) {
    // The constructor takes an ifstream and HeaderBuffer, initializing the BlockBuffer
    // with the provided file and headerBuffer.
}

void PrintBlock::logical() {
    // Print blocks sequentially by their logical ordering
    int currentRBN = 0;  // Starting RBN, you can change it based on your requirements
    std::cout << "List Head: " << currentRBN <<"\n";
    std::cout << "Avail Head: " << currentRBN <<"\n";//we dont have an avai list
    while (currentRBN < 4620) {
        std::vector<std::string> records = blockBuffer.readBlock(currentRBN);

        // Print block metadata or any other information you need

        std::cout << currentRBN;

        // Print each record in the block
        for (const auto& record : records) {
            std::cout << record;
        }
        std::cout << "\n";
        // Move to the next block
        currentRBN = blockBuffer.getNextRBN();
    }
}

void PrintBlock::physical() {
    // Print blocks sequentially by their physical ordering
    int currentRBN;
    //= blockBuffer.readNextBlockMetadata();

    while (currentRBN != -1) {
        std::vector<std::string> records = blockBuffer.readBlock(currentRBN);

        // Print block metadata or any other information you need
        std::cout << "Block " << currentRBN << " (Logical Order):\n";

        // Print each record in the block
        for (const auto& record : records) {
            std::cout << record << '\n';
        }

        // Move to the next block
        currentRBN = blockBuffer.getNextRBN();
    }
}
