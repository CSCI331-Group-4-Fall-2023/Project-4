// ----------------------------------------------------------------------------
/**
 * @file BlockBufferTest.cpp
 * @brief This class tests the BlockBuffer class and blocked sequences
 * @author Andrew Clayton
 * @date 11/9/2023
 * @version 1.0
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * The BlockBufferTest class tests the BlockBuffer class and blocked sequences, ensuring the blocks are broken
  * down properly into the correct number of records, and that the records are correct.
  * 
  */
  // ----------------------------------------------------------------------------



#include <iostream>
#include <vector>
#include "BlockBuffer.h"

using namespace std;

int main() {
    // Test 1: Test with a block containing multiple records
    BlockBuffer buffer1("42,501,Holtsville,NY,Suffolk,40.8154,-73.045142,544,Holtsville,NY,Suffolk,40.8154,-73.045139,1001,Agawam,MA,Hampden,42.0702,-72.622742,1002,Amherst,MA,Hampshire,42.3671,-72.464642,1003,Amherst,MA,Hampshire,42.3919,-72.524842,1004,Amherst,MA,Hampshire,42.3845,-72.513240,1005,Barre,MA,Worcester,42.4097,-72.108445,1007,Belchertown,MA,Hampshire,42.2751,-72.41142,1008,Blandford,MA,Hampden,42.1829,-72.9361~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    vector<string> records1 = buffer1.unpackBlockRecords();
    if (records1.size() != 9) {
        cout << "Test 1 failed.\n";
    } else {
        cout << "Test 1 passed.\n";
    }

    // Test 2: Test with a block containing multiple records and ending with filler characters
    BlockBuffer buffer2("43,1009,Bondsville,MA,Hampden,42.2061,-72.340542,1010,Brimfield,MA,Hampden,42.1165,-72.188540,1011,Chester,MA,Hampden,42.2794,-72.988847,1012,Chesterfield,MA,Hampshire,42.3923,-72.825641,1013,Chicopee,MA,Hampden,42.1487,-72.607941,1014,Chicopee,MA,Hampden,42.1707,-72.604841,1020,Chicopee,MA,Hampden,42.1764,-72.576141,1021,Chicopee,MA,Hampden,42.1707,-72.604841,1022,Chicopee,MA,Hampden,42.1934,-72.5544~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    vector<string> records2 = buffer2.unpackBlockRecords();
    if (records2.size() != 9) {
        cout << "Test 2 failed.\n";
    } else {
        cout << "Test 2 passed.\n";
    }

    // Test 3: Test with a block that should contain invalid record lengths (for this test, we need to ensure the test data has intentional errors that are handled by BlockBuffer)
    // Note: This test will depend on how BlockBuffer::createRecords() handles errors. If it stops processing on error, this test should expect fewer records.
    BlockBuffer buffer3("45,1026,Cummington,MA,Hampshire,42.4633,-72.920245,1027,Easthampton,MA,Hampshire,42.2668,-72.66948,1028,East Longmeadow,MA,Hampden,42.0672,-72.505644,1029,East Otis,MA,Berkshire,42.1909,-73.051746,1030,Feeding Hills,MA,Hampden,42.0718,-72.675147,1031,Gilbertville,MA,Worcester,42.3322,-72.198641,1032,Goshen,MA,Hampshire,42.4404,-72.799539,1033,Granby,MA,Hampshire,42.2557,-72.52~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    vector<string> records3 = buffer3.unpackBlockRecords();
    if (records3.size() != 8) { // Assuming the last record is invalid and should not be counted
        cout << "Test 3 failed.\n";
    } else {
        cout << "Test 3 passed.\n";
    }

    // Print out the records in the vector records2 to ensure they look right
    cout << "Printing out the records contained in the second block to ensure they look right visually:\n";
    for (int i = 0; i < records2.size(); i++) {
        cout << records2[i] << endl;
    }

    return 0;
}
