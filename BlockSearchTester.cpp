#include "BlockSearch.h"
#include <iostream>

void testSearch(BlockSearch& searcher, const int& zipcode, const string& expected) {
    
    string result = searcher.searchForRecord(zipcode);
    if (result == expected) {
        cout << "Test passed for zipcode " << zipcode << endl;
    } else {
        cout << "\nTest failed for zipcode " << zipcode << endl;
        cout << "Expected: " << expected << endl;
        cout << "Got: " << result << "\n";
    }
}

int main() {
    /*string currentBlock = "20,2892,9,2891,2893,43,60913,Bonfield,IL,Kankakee,41.1573,-88.061945,60914,Bourbonnais,IL,Kankakee,41.1661,-87.87942,60915,Bradley,IL,Kankakee,41.1454,-87.860145,60917,Buckingham,IL,Kankakee,41.0433,-88.177242,60918,Buckley,IL,Iroquois,40.6018,-88.036137,60919,Cabery,IL,Ford,40.9819,-88.192143,60920,Campus,IL,Livingston,41.0248,-88.307247,60921,Chatsworth,IL,Livingston,40.7484,-88.293743,60922,Chebanse,IL,Iroquois,41.0254,-87.8959~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    int maxZipcode = 0;
    int start = stoi(currentBlock.substr(0, 2));
    size_t endOfBlock = currentBlock.find('~'); // Assuming '~' is the padding character

    while (start < endOfBlock) {
        size_t recordLength;
        try {
            recordLength = stoi(currentBlock.substr(start, 2)) + 3; // plus three since `LI,` does not include itself
        } catch (const std::invalid_argument& ia) {
            cerr << "Invalid argument: " << ia.what() << " for record length at block " << "\n";
            // return 1;
        }
        
        // Traversing record by record
        string currentRecord = currentBlock.substr(start, recordLength);
        int currentZipcode = findZipcode(currentRecord);
        // Checking the zipcode of each individual record
        if (currentZipcode > maxZipcode) {
            maxZipcode = currentZipcode;
            cout << "Current maxZipcode is: " << maxZipcode << "\n";
        }
        start += recordLength;
    }*/
    // cout << "Final maxzipcode is : " << maxZipcode << "\n";

    BlockSearch blockSearcher; // Assuming default constructor is sufficient for testing

    // Test cases
    testSearch(blockSearcher, 56301, "44,56301,Saint Cloud,MN,Stearns,45.541,-94.1819");
    testSearch(blockSearcher, 61072, "43,61072,Rockton,IL,Winnebago,42.4544,-89.0887");
    testSearch(blockSearcher, 1453, "45,1453,Leominster,MA,Worcester,42.5274,-71.7563");
    testSearch(blockSearcher, 999, "-1");

    return 0;
}
