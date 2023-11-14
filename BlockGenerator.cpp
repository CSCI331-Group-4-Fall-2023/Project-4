// ----------------------------------------------------------------------------
/**
 * @file BlockGenerator.cpp (Blocked Sequence Set Generator)
 * @brief File for generating a blocked sequence set
 * @author Andrew Clayton
 * @date 11/13/2023
 * @version 1.2
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * The BlockGenerator class converts the length-indicated data into blocked data.
  * Block size is 512 bytes, and the minimum block capacity is 50%. Block capacity is currently set to 75%. All records in blocks are complete.
  * Blocks are separated on different lines (end of line character), and records within a block are only distinct via length indication. 
  * This file includes metadata: relative block number (RBN), number of records in the block, RBN of previous block, and RBN of next block.
  *
  Assumptions:
  *
   -- The file is in the same directory as the program.
  */
  // ----------------------------------------------------------------------------

    
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    const int BLOCK_SIZE = 512;
    const int BLOCK_CAPACITY = 0.75 * BLOCK_SIZE; // 75% of the block size

    // Check if the correct number of command line arguments were given
    if (argc < 2) {
        cerr << "Error: No file name given.\n";
        return 1;
    }

    // File to write data out to
    string blockedDataFile = argv[1]; // Assumes the first command line argument is the file name
    ofstream writeFile(blockedDataFile + ".txt");
    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file " << blockedDataFile << " for writing.\n";
        return 1;
    }
    
    // File to read information from
    ifstream readFile("us_postal_codes.txt");
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file us_postal_codes.txt for reading.\n";
        return 1;
    }

    // Now we go through the file and convert the length-indicated data to blocked data, ensuring that
    // records stay complete within the BLOCK_CAPACITY

    // Initialize variables for looping
    string currentLine;
    getline(readFile, currentLine); // Skipping metadata
    int currentBlockSize = 0;
    int currentBlock = 1;
    int numRecords = 0;
    vector<string> recordsInBlock;
    bool isLastBlock = false;

    /*
    As we process through records, we store all of them in a vector. When the vector cannot add another record without reaching block capacity, then we 
    write the (length-indicated) metadata to the file, followed by all of the vectors. Then we proceeding with checking more records.
        - First block will have its previous block number as -1
        - Last block (when we reach the end of the file for records) will have its next block number as -1           */

    while (getline(readFile, currentLine)) {
        int currentLineLength = stoi(currentLine.substr(0, 2)) + 3; // Including LI and comma
        recordsInBlock.push_back(currentLine);
        numRecords++;
        currentBlockSize += currentLineLength;

        if (currentBlockSize > BLOCK_CAPACITY || readFile.eof()) {
            isLastBlock = readFile.eof();

            // Metadata calculation
            string metadata = to_string(currentBlock) + "," + to_string(numRecords) + "," + (currentBlock == 1 ? "-1" : to_string(currentBlock - 1)) + "," + (isLastBlock ? "-1" : to_string(currentBlock + 1)) + ",";
            int metadataLength = metadata.length() + 3; // Including LI and comma and ending comma

            // Write metadata and records
            // Metadata format: LI,RBN,#ofRecords,prevBlock,nextBlock,
            writeFile << metadataLength << "," << metadata;
            for (const string& record : recordsInBlock) {
                writeFile << record;
            }

            // Pad the block with '~'
            for (int i = 0; i < BLOCK_SIZE - currentBlockSize - metadataLength; i++) {
                writeFile << "~";
            }
            writeFile << "\n"; // New block

            // Reset for the next block
            recordsInBlock.clear();
            currentBlockSize = 0;
            numRecords = 0;
            currentBlock++;
        }
    }

    readFile.close();
    writeFile.close();
    return 0;
}
