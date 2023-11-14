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
        cerr << "Error parsing record for zipcode: " << record << endl;
        return -1;
    }
    cout << "Zipcode: " << record.substr(firstComma + 1, secondComma - firstComma - 1) << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
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

    while (getline(readFile, currentBlock)) {
        // for each line, we have to skip the metadata!  
        int start = stoi(currentBlock.substr(0, 2)) + 1;
        cout << "Start: " << start << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf 
        size_t end = currentBlock.find('~'); // Assuming '~' is the padding character

        while (start < end) {
            size_t recordLength;
            try {
                cout << "recordLength: " << currentBlock.substr(start, 2) << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
                recordLength = stoi(currentBlock.substr(start, 2));
            } catch (const std::invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << " for record length at block " << blockNumber << endl;
                return 1;
            }

            string currentRecord = currentBlock.substr(start, recordLength);
            cout << "currentRecord: " << currentRecord << "\n"; ///////////////////////////////////////////////troubleohsinsiosd jjfsodij oasidjf
            int currentZipcode = findZipcode(currentRecord);

            if (currentZipcode > maxZipcode) {
                maxZipcode = currentZipcode;
            }
            start += recordLength;
        }

        writeFile << blockNumber << "," << maxZipcode << endl;

        blockNumber++;
        maxZipcode = 0;
    }

    readFile.close();
    writeFile.close();
    return 0;
}


///abcdefghijklmnopqrstuvwxyz