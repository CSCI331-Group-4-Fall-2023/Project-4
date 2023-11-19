// ----------------------------------------------------------------------------
/**
 * @file block_idx_gen.cpp
 * @brief This class creates an index file for a blocked data file. 
 * @author Andrew Clayton
 * @date 11/13/2023
 * @version 1.4
 */
// ----------------------------------------------------------------------------
/**
 * @details
 *
 * The index file consists of pairs of block number, and the greatest key (zipcode) value in the block. 
 * The index file is sorted by block number.
 * 
 */
// ----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "HeaderBuffer.h"

using namespace std;

int findZipcode(const string& record) {
    size_t firstComma = record.find(',');
    size_t secondComma = record.find(',', firstComma + 1);
    if (firstComma == string::npos || secondComma == string::npos) {
        cerr << "Error parsing record for zipcode: " << record << "\n";
        return -1;
    }
    return stoi(record.substr(firstComma + 1, secondComma - firstComma - 1));
}


int main() {

    // We need to first read and write the header of the file
    /* HeaderBuffer header = HeaderBuffer("blocked_postal_codes.txt");
    header.readHeader();
    header.writeHeaderToFile("blocked_Index.txt"); */          // The blocked index file actually does not include any metadata


    ofstream writeFile;
    writeFile.open("blocked_Index.txt");
    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file 'blocked_Index.txt' for writing.\n";
        return 1;
    }
    
    ifstream readFile("us_postal_codes_blocked.txt");
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file 'us_postal_codes_blocked.txt' for reading.\n";
        return 1;
    }

    // We need to skip past the metadata, up to the "Data: line"
    string line;
    while (getline(readFile, line)) {
        if (line == "Data:") {
            break;
        }
    }

    int blockNumber = 0;
    int maxZipcode = 0;
    string currentBlock;

    while (getline(readFile, currentBlock)) {
        // for each line, we have to skip the metadata!  
        int start = stoi(currentBlock.substr(0, 2));
        size_t endOfBlock = currentBlock.find('~'); // Assuming '~' is the padding character

        maxZipcode = 0;

        // Put all the records into a vector
        vector<string> records;

        while (start < endOfBlock) {
            size_t recordLength;
            try {
                recordLength = stoi(currentBlock.substr(start, 2)) + 3; // plus three since `LI,` does not include itself
            } catch (const std::invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << " for record length at block " << blockNumber << "\n";
                return 1;
            }
            
            // Traversing record by record
            string currentRecord = currentBlock.substr(start, recordLength);
            records.push_back(currentRecord);
            // Checking the zipcode of each individual record


            start += recordLength;
        }

        maxZipcode = findZipcode(records.back());

        writeFile << blockNumber << "," << maxZipcode << "\n";

        blockNumber++;
        // maxZipcode = 0;
    }

    readFile.close();
    writeFile.close();
    return 0;
}