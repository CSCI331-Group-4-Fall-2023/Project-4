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
    BlockSearch blockSearcher; // Assuming default constructor is sufficient for testing

    // Test cases
    testSearch(blockSearcher, 56301, "44,56301,Saint Cloud,MN,Stearns,45.541,-94.1819");
    testSearch(blockSearcher, 61072, "43,61072,Rockton,IL,Winnebago,42.4544,-89.0887");
    testSearch(blockSearcher, 1453, "45,1453,Leominster,MA,Worcester,42.5274,-71.7563");
    // testSearch(blockSearcher, 999, "-1");

    return 0;
}
