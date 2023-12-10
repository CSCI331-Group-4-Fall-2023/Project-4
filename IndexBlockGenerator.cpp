// ----------------------------------------------------------------------------
/**
 * @file IndexBlockGenerator.cpp (Blocked Sequence Set Generator)
 * @brief File for generating a blocked sequence set for the index
 * @author Kent Biernath
 * @author Andrew Clayton
 * @date 2023-12-09
 * @version 1.0
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * \n The IndexBlockGenerator class converts the data into blocked data.
  * \n Block size is 512 bytes, and the minimum block capacity is 50%. Block capacity is currently set to 75%. All records in blocks are complete.
  * \n Blocks are separated on different lines (end of line character), and records are separated by commas.
  * \n This file includes metadata: relative block number (RBN), number of records in the block, RBN of previous block, and RBN of next block.
  *
  *///----------------------------------------------------------------------------

    
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "HeaderBuffer.h"

using namespace std;

int main(int argc, char* argv[]) {
    const int BLOCK_SIZE = 510;
    const int BLOCK_CAPACITY = 0.75 * BLOCK_SIZE; // 75% of the block size


    // We need to first read and write the header of the file
    //HeaderBuffer header = HeaderBuffer("blocked_Index.txt");
    //header.readHeader();
    //header.writeHeaderToFile("us_postal_codes_blocked.txt");

    // Now we can proceed with the blocked data generation, but we have to make sure the file opens up where we left off



    // Check if the correct number of command line arguments were given
    // if (argc < 2) {
    //     cerr << "Error: No file name given.\n";
    //     return 1;
    // }

    // File to write data out to
    string blockedDataFile = "blocked_blocked_index.txt"; //argv[1]; // Assumes the first command line argument is the file name
    ofstream writeFile;
    // we need to append to the file, not overwrite it
    writeFile.open(blockedDataFile, ios::app | ios::ate);
    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file " << blockedDataFile << " for writing.\n";
        return 1;
    }  
    
    // File to read information from
    ifstream readFile("blocked_Index.txt", ios::binary);
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file for reading.\n";
        return 1;
    }

    // We need to have the file open to the actual content, past the metadata. 

    // Now we go through the file and convert the length-indicated data to blocked data, ensuring that
    // records stay complete within the BLOCK_CAPACITY

    // Initialize variables for looping
    string currentLine;
    //getline(readFile, currentLine); // Skipping metadata
    int currentBlockSize = 0;
    int currentBlock = 0;
    int numRecords = 0;
    vector<string> recordsInBlock;
    bool isLastBlock = false;

    /*
    As we process through records, we store all of them in a vector. When the vector cannot add another record without reaching block capacity, then we 
    write the (length-indicated) metadata to the file, followed by all of the vectors. Then we proceeding with checking more records.
        - First block will have its previous block number as -1
        - Last block (when we reach the end of the file for records) will have its next block number as -1           */

    while (getline(readFile, currentLine)) {

        int currentLineLength = currentLine.size() + 1; // Including LI and comma
        recordsInBlock.push_back(currentLine);
        numRecords++;
        currentBlockSize += currentLineLength;

        if (currentBlockSize > BLOCK_CAPACITY || readFile.eof()) {
            isLastBlock = readFile.eof();

            // Metadata calculation
            string metadata = to_string(currentBlock) + "," + to_string(numRecords) + "," + (currentBlock == 0 ? "-1" : to_string(currentBlock - 1)) + "," + (isLastBlock ? "-1" : to_string(currentBlock + 1));
            int metadataLength = metadata.length() + 3; // Including LI and comma and ending comma

            // Write metadata and records
            // Metadata format: LI,RBN,#ofRecords,prevBlock,nextBlock,
            writeFile << metadataLength << "," << metadata;
            for (const string& record : recordsInBlock) {
                writeFile << ',' << record;
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

    // Anything left in the vector now will be the last block
    isLastBlock = true;

    // We need to print out everything left in the vector
    string metadata = to_string(currentBlock) + "," + to_string(numRecords) + "," + (currentBlock == 0 ? "-1" : to_string(currentBlock - 1)) + "," + (isLastBlock ? "-1" : to_string(currentBlock + 1)) + ",";
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

    /*
    // Once all of this is done, we need to create an empty avail list. The next and previous RBNs will be -1
    // The avail list will be at the end of the file, and will be the last block
    // The metadata will be: LI,RBN,#ofRecords,prevBlock,nextBlock,
    string availListMetadata = to_string(currentBlock+1) + ",0,-1,-1,";
    int availListMetadataLength = availListMetadata.length() + 3; // Including LI and comma and ending comma
    writeFile << "\n" << availListMetadataLength << "," << availListMetadata;
    for (int i = 0; i < BLOCK_SIZE - availListMetadataLength; i++) {
        writeFile << "~";
    }
    */

    readFile.close();
    writeFile.close();
    return 0;
}
