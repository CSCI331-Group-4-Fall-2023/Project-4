// ----------------------------------------------------------------------------
/**
 * @file BlockGenerator.cpp (Blocked Sequence Set Generator)
 * @brief File for generating a blocked sequence set
 * @author Andrew Clayton
 * @date 11/13/2023
 * @version 1.1
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * The BlockGenerator class converts the length-indicated data into blocked data.
  * Block size is 512 bytes, and the minimum block capacity is 50%. All records in blocks are complete.
  * Blocks are separated on different lines (end of line character), and records within a block are only distinct
  * via length indication. 
  * This file includes metadata: relative block number (RBN), number of records in the block, RBN of previous block, and RBN of next block.
  *
  Assumptions:
  *
   -- The file is in the same directory as the program.
  */
  // ----------------------------------------------------------------------------





/* Metadata we want: block number, number of records in the block, RBN of the previous block, and RBN of the next block. Metadata will be length-indicated.


48,1438,East Templeton,MA,Worcester,42.5631,-72.03741,1440,Gardner,MA,Worcester,42.574,-71.989846,1441,Westminster,MA,Worcester,42.5459,-71.910641,1450,Groton,MA,Middlesex,42.6124,-71.558442,1451,Harvard,MA,Worcester,42.4986,-71.575346,1452,Hubbardston,MA,Worcester,42.4865,-72.001245,1453,Leominster,MA,Worcester,42.5274,-71.756344,1460,Littleton,MA,Middlesex,42.5401,-71.4877~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
->
LI,RBN,#ofRecords,prevBlock,nextBlock,48,1438,East Templeton,MA,Worcester,42.5631,-72.03741,1440,Gardner,MA,Worcester,42.574,-71.989846,1441,Westminster,MA,Worcester,42.5459,-71.910641,1450,Groton,MA,Middlesex,42.6124,-71.558442,1451,Harvard,MA,Worcester,42.4986,-71.575346,1452,Hubbardston,MA,Worcester,42.4865,-72.001245,1453,Leominster,MA,Worcester,42.5274,-71.756344,1460,Littleton,MA,Middlesex,42.5401,-71.4877~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                     ^
                                     |



As we process through records, we store all of them in a vector. When the vector cannot add another record without reaching block capacity, then we 
write the (length-indicated) metadata to the file, followed by all of the vectors. Then we proceeding with checking more records.
 - This may entail storing the last checked record
 - Definitely will need to store current block number, and current number of records
        - First block will have its previous block number as -1
        - Last block (when we reach the end of the file for records) will have its next block number as -1


*/

#include <iostream>
#include <fstream> // for file operations
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    const int BLOCK_SIZE = 512;
    const int BLOCK_CAPACITY = 0.75 * BLOCK_SIZE; // 75% of the block size

    // Check if the correct number of command line arguments were given
    if (argc < 2) {
        cerr << "Error: No file name given. Your command line arguments should include the name for the blocked sequence set data file.\n";
        return 1;
    }

    // File to write data out to
    string blockedDataFile = argv[1]; // Assumes the first command line argument is the file name
    string fileName = blockedDataFile + ".txt";
    ofstream writeFile(fileName);

    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file " << blockedDataFile << "for writing.\n";
        return 1;
    }
    
    // File to read information from
    ifstream readFile("us_postal_codes.txt");
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file us_postal_codes.txt fo reading.\n";
        return 1;
    }

    // Now we go through the file and convert the length-indicated data to blocked data, ensuring that
    // records stay complete within the BLOCK_CAPACITY

    string currentLine; // the current line being read

    // Read and ignore the first line of the file (metadata)
    getline(readFile, currentLine);
    int currentBlockSize = 0;
    int currentLineLength = 0;
    int currentBlock = 0;
    int numRecords = 0;

    // While loop to keep going while there's data to be read

    /*
    As we process through records, we store all of them in a vector. When the vector cannot add another record without reaching block capacity, then we 
    write the (length-indicated) metadata to the file, followed by all of the vectors. Then we proceeding with checking more records.
    - This may entail storing the last checked record
    - Definitely will need to store current block number, and current number of records
        - First block will have its previous block number as -1
        - Last block (when we reach the end of the file for records) will have its next block number as -1           */
    vector<string> recordsInBlock;
    while (getline(readFile, currentLine)) {
        try {
            currentLineLength = stoi(currentLine.substr(0, 2)) + 3; // including the length of the length indicator (LI) and comma
        } catch (const invalid_argument& ia) {
            cerr << "Invalid argument: " << ia.what() << "\n";
            return 1;
        }

        numRecords++; // each time we reach this point, we've added another record
        recordsInBlock.push_back(currentLine);

        // If adding the current line exceeds the block capacity, end the current block
        if (currentBlockSize + currentLineLength > BLOCK_CAPACITY) {
            // We first write the metadata, and add the length of this to the length of the records.
            
            // in order to write the metadata, we first need to figure out the metadata..

            // LI,RBN,#ofRecords,prevBlock,nextBlock,
            string metadata = "";

            string rbn = to_string(currentBlock++); // block calculation -- should work to increment the block number after saving
            string numRecordsS = to_string(recordsInBlock.size()); // num records string
            string prevBlock = to_string(currentBlock - 1);
            string nextBlock = to_string(currentBlock + 1);

            // Checks for first and last block            
            if (currentBlock == 0) prevBlock = "-1";
            if (readFile.eof()) string nextBlock = "-1"; // this may not be correct!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!


            metadata += rbn + "," + numRecordsS + "," + prevBlock + "," + nextBlock + ",";

            // Writing metadata, with length indication. 1 added to length for our added comma
            writeFile << metadata.length()+1 << "," << metadata << "\n";

            for (string record : recordsInBlock) { // adding each record to the block
                writeFile << record;
            }


            currentBlockSize += metadata.length();
            for (int i = 0; i < BLOCK_SIZE - currentBlockSize; i++) {
                writeFile << "~";
            }
            // updating variables for next iteration
            currentBlockSize = 0; numRecords = 0; recordsInBlock.clear(); // clear individual variables, and clear the vector of records

        }
        // By now, all records have been aggregated into blocks

        // If the last block is not full, it needs padding, and 
        // this also would mean it did not even get printed, or its metadata. (since capacity length wasn't exceeded)
        // So we need to write the metadata for the last block, and then add padding to it.
        if (currentBlockSize > 0 && currentBlockSize < BLOCK_CAPACITY) {
            // We first write the metadata, and add the length of this to the length of the records.
             string metadata = "";

            string rbn = to_string(currentBlock++); // block calculation -- should work to increment the block number after saving
            string numRecordsS = to_string(recordsInBlock.size()); // num records string
            string prevBlock = to_string(currentBlock - 1);
            string nextBlock = to_string(currentBlock + 1);

            // Checks for first and last block            
            if (currentBlock == 0) prevBlock = "-1";
            if (readFile.eof()) string nextBlock = "-1"; // this may not be correct!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!


            metadata += rbn + "," + numRecordsS + "," + prevBlock + "," + nextBlock + ",";

            // Writing metadata, with length indication. 1 added to length for our added comma
            writeFile << metadata.length()+1 << "," << metadata << "\n";

            for (string record : recordsInBlock) { // adding each record to the block
                writeFile << record;
            }


            currentBlockSize += metadata.length();
            for (int i = 0; i < BLOCK_SIZE - currentBlockSize; i++) {
                writeFile << "~";
            }
            // updating variables for next iteration
        }

    }


    readFile.close();
    writeFile.close();
    return 0;

}