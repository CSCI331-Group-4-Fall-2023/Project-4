// ----------------------------------------------------------------------------
/**
 * @file block_idx_gen.cpp
 * @brief This class creates an index file for a blocked data file. 
 * @author Andrew Clayton
 * @date 11/13/2023
 * @version 1.0
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

using namespace std;

int findZipcode(const string& record) {
    size_t firstComma = record.find(',');
    size_t secondComma = record.find(',', firstComma + 1);
    if (firstComma == string::npos || secondComma == string::npos) {
        cerr << "Error parsing record for zipcode: " << record << "\n";
        return -1;
    }
    // cout << "Zipcode: " << record.substr(firstComma + 1, secondComma - firstComma - 1) << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
    return stoi(record.substr(firstComma + 1, secondComma - firstComma - 1));
}


int main() {
    ofstream writeFile("blockedIndex.txt");
    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file 'blockedIndex.txt' for writing.\n";
        return 1;
    }
    
    ifstream readFile("blocked_postalcodes.txt");
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file 'blocked_postalcodes.txt' for reading.\n";
        return 1;
    }

    int blockNumber = 0;
    int maxZipcode = 0;
    string currentBlock;

    // testing...
    // getline(readFile, currentBlock);
    // cout << "\n" << currentBlock << "\n\n";
    // int startOfRecord = stoi(currentBlock.substr(0, 2));
    // int recordLength = stoi(currentBlock.substr(startOfRecord, 2)) + 3; // plus three since `LI,` does not include itself
    
    // cout << "Substring at start: " << currentBlock.substr(startOfRecord, 15) << "\n";
    // cout << "First record length: " << recordLength << "\n";
    // int endOfBlock = currentBlock.find('~');
    // cout << "Whole first record: " << currentBlock.substr(startOfRecord, recordLength) << "\n";
    // cout << "End of block: " << endOfBlock<< "\n";
    // cout << 

    // actual program
    while (getline(readFile, currentBlock)) {
        // for each line, we have to skip the metadata!  
        int start = stoi(currentBlock.substr(0, 2));
        size_t endOfBlock = currentBlock.find('~'); // Assuming '~' is the padding character

        while (start < endOfBlock) {
            size_t recordLength;
            try {
                // cout << "recordLength: " << currentBlock.substr(start, 2) << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
                recordLength = stoi(currentBlock.substr(start, 2)) + 3; // plus three since `LI,` does not include itself
            } catch (const std::invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << " for record length at block " << blockNumber << "\n";
                return 1;
            }
            // cout << "Whole current record: " << currentBlock.substr(start, recordLength) << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf

            string currentRecord = currentBlock.substr(start, recordLength);
            // cout << "currentRecord: " << currentRecord << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
            int currentZipcode = findZipcode(currentRecord);

            if (currentZipcode > maxZipcode) {
                maxZipcode = currentZipcode;
            }
            start += recordLength;
        }

        writeFile << blockNumber << "," << maxZipcode << "\n";

        blockNumber++;
        maxZipcode = 0;
    }

    readFile.close();
    writeFile.close();
    return 0;
}