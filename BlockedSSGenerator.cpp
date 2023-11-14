// ----------------------------------------------------------------------------
/**
 * @file BlockedSSGenerator.cpp (Blocked Sequence Set Generator)
 * @brief File for generating a blocked sequence set
 * @author Andrew Clayton
 * @date 11/9/2023
 * @version 1.0
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * The BlockedSSGenerator program converts the length-indicated data into blocked data.
  * Block size is 512 bytes, and the minimum block capacity is 50%. All records in blocks are complete.
  * Blocks are separated on different lines (end of line character), and records within a block are only distinct
  * via length indication.
  *
  Assumptions:
  *
   -- The file is in the same directory as the program.
  */
  // ----------------------------------------------------------------------------

#include <iostream>
#include <fstream> // for file operations
#include <string>

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
    int currentLineLength;

    // While loop to keep going while there's data to be read
    while (getline(readFile, currentLine)) {

        try {
            currentLineLength = stoi(currentLine.substr(0, 2)) + 3;
        } catch (const invalid_argument& ia) {
            cerr << "Invalid argument: " << ia.what() << "\n";
            return 1; // could be changed to continue if things need to work out
        }

        // If adding the current line exceeds the block capacity, end the current block
        if (currentBlockSize + currentLineLength> BLOCK_CAPACITY) {
            // First we have to fill the rest of the current block with filler characters
            for (int i = 0; i < BLOCK_SIZE - currentBlockSize; i++) {
                writeFile << "~";
            } 
            writeFile << "\n";

            currentBlockSize = 0;
        }

        // Add the current line to the block
        writeFile << currentLine;
        currentBlockSize += currentLineLength;

    }
    // By now, all records have been aggregated into blocks.

    // If the last block is not full, pad it
    if (currentBlockSize > 0 && currentBlockSize < BLOCK_CAPACITY) {
        for (int i = 0; i < BLOCK_SIZE - currentBlockSize; i++) {
            writeFile << "~";
        }
    }

    readFile.close();
    writeFile.close();

    return 0;
}