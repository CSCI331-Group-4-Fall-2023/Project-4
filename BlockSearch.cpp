/// @file BlockSearch.cpp
/// @class BlockSearch
/// @brief Implementation of the BlockSearch class for searching for records in the blocked index file.
/// See BlockSearch.h for full documentation.

#include <string>
#include <fstream>
#include <iostream>
#include "BlockSearch.h"
#include <vector>
#include "BlockBuffer.h"
#include "HeaderBuffer.h"

using namespace std;


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
    string line;
    
    // Read through the file until we find target < greatestKeyInBlock

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

            ifstream dataFile("us_postal_codes_blocked.txt", std::ios::app);
            HeaderBuffer headerBuffer2("us_postal_codes_blocked.txt");
            BlockBuffer blockbuffer(dataFile, headerBuffer2);

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

void BlockSearch::displayRecord(string record) {
    // The format of a record is: zipcode,town,state,county,latitude,longitude
    vector<string> fields;
    stringstream ss(record);
    string field;

    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }

    if (fields.size() == 6) { // Ensure there are exactly 6 fields: zipcode, town, state, county, latitude, longitude
        cout << "Zipcode: " << fields[0] << endl;
        cout << "Town: " << fields[1] << endl;
        cout << "State: " << fields[2] << endl;
        cout << "County: " << fields[3] << endl;
        cout << "Latitude: " << fields[4] << endl;
        cout << "Longitude: " << fields[5] << "\n\n";
    } else {
        cerr << "Zipcode not found." << endl;
    }

}