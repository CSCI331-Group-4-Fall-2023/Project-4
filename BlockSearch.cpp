/// @file BlockSearch.cpp
/// @class BlockSearc
/// @brief Implementation of the BlockSearch class for searching for records in the blocked index file.


#include <string>
#include <fstream>
#include <iostream>
// #include "BlockSearch.h"
#include <vector>
#include "BlockBuffer.h"
#include "BlockBuffer.cpp"

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

// Searches for a record in the blocked index file by key (zipcode)
string BlockSearch::searchForRecord(int target) {
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
        int commaIdx = line.find(',');
        int rbn = 0;
        try {
            rbn = stoi(line.substr(0, commaIdx));
        } catch (invalid_argument& e) {
            cerr << "Error parsing RBN: " << e.what() << endl;
            // return "-1";
        }

        int greatestKeyInBlock;
        try {
            greatestKeyInBlock = stoi(line.substr(commaIdx+1));
        } catch (invalid_argument& e) {
            cerr << "Error parsing greatest key in block: " << e.what() << endl;
            // return "-1";
        }
        
        if (target < greatestKeyInBlock) {
            // We have found the block that contains the record we are looking for
            // now we need to actually access the block itself, which we should be able to do with BlockBuffer

            ifstream dataFile("blockedcodes.txt");
            BlockBuffer blockbuffer(dataFile);

            // We break down all the block into a vector of records

            vector<string> records = blockbuffer.readBlock(rbn);

            for (string record : records) { // Check if each record is the target record
                int commaIdx = record.find(',');
                int zipcode = stoi(record.substr(0, commaIdx));

                if (zipcode == target) {
                    return record;
                }
            }
            break; // not  found in this block, and next blocks have greater keys
        }    
    }
    // We could not find the block that contains the record we are looking for
    return "-1";   
}

    // void BlockSearch::displayRecord(string record) {    }
void BlockSearch::displayRecord(string record) {
    // The format of a record is: LI,zipcode,town,state,county,latitude,longitude
    // We want to print out each label (except length indicator) and the corresponding value
    // We can use the comma as a delimiter to split the string into a vector of strings
    vector<string> fields;
    size_t start = 0;
    size_t end = record.find(',');
    while (end != string::npos) {
        fields.push_back(record.substr(start, end - start));
        start = end + 1;
        end = record.find(',', start);
    }

}