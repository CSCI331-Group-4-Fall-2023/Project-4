/// @file BlockSearch.cpp
/// @class BlockSearc
/// @brief Implementation of the BlockSearch class for searchign for records in the blocked index file.


#include <string>
#include <fstream>
#include <iostream>
#include "BlockSearch.h"
#include <vector>

// Default constructor
BlockSearch::BlockSearch(string idxFile) {
    indexFile = idxFile;
}


int findZipcode(const string& record) {
    // The zipcode is the substring between the first and second comma
    size_t firstComma = record.find(',');
    size_t secondComma = record.find(',', firstComma + 1);
    if (firstComma == string::npos || secondComma == string::npos) {
        cerr << "Error parsing record for zipcode: " << record << "\n";
        return -1;
    }
    return stoi(record.substr(firstComma + 1, secondComma - firstComma - 1));
}

vector<string> blockToRecords(const string& block) {
    vector<string> records;
    size_t start = 0;
    int recordLength;
    while (start < block.size()) {
        try {
            recordLength = stoi(block.substr(start, 2));
        } catch (invalid_argument& e) {
            cerr << "Error parsing record length: " << e.what() << endl;
            return records;
        }

        // int recordLength = stoi(block.substr(start, 2));
        cout << recordLength;
        records.push_back(block.substr(start, recordLength));
        start += recordLength + 3; // +3 for "LI,"
    }
    return records;
}

// Searches for a record in the blocked index file by key (zipcode)
string BlockSearch::searchForRecord(string target) {
    // Open the index file
    ifstream readFile(indexFile);

    // We have to skip past the metadata, up to the "Data: line"
    string line;
    while (getline(readFile, line)) {
        if (line == "Data:") {
            break;
        }
    }

    // Now we are to the content of the file - we will read through the file until we find target < greatestKeyInBlock

    // Iterate through each line of the file
    while (getline(readFile, line)) {
        line.find(',');
        string greatestKeyInBlock = line.substr(line.find(','));
        if (target < greatestKeyInBlock) {

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        /////////////// ONCE WE HAVE OUR BLOCK, WE NEED TO USE THE INDEX FILE TO OPEN THE BLOCK IN THE POSTAL CODES.
        /////////////// PROBABLY NEED TO USE BLOCK BUFFER TO LOCATE THE SPECIFIC BLOCK BY RBN AND OPEN IT UP


            /////////////////////////////////////////////////////////////////////////////////////////////////////////


            ///////////////////////////////////////////////////////////////////////////
            // We have found the block that contains the record we are looking for

            // First, we skip past the metadata
            int metadataLength = stoi(line.substr(0, 2));
            string blockContent = line.substr(metadataLength);

            // We break down all the block into a vector of records
            vector<string> records = blockToRecords(blockContent);

            for (string record : records) { // Check if each record is the target record
                int zipcode = findZipcode(record);
                if (zipcode == stoi(target)) {
                    return record;
                }
            }
            
            // If we get here, we have not found the record
            return "-1";
        } // else target >= greatestKeyInBlock.. so we just want to keep going

        // We could not find the block that contains the record we are looking for
        return "-1";          
        
    }

    // void BlockSearch::displayRecord(string record) {    }
void BlockSearch::displayRecord(string record) {
    // The format of a record is: 
}


}