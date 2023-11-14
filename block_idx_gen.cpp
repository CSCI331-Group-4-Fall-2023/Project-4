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

using namespace std;

// This is a function to retrieve the value of a zipcode from a record
// Example record: 42,501,Holtsville,NY,Suffolk,40.8154,-73.0451
int findZipcode(string record) {
    // The zipcode is the substring between the first and second comma
    int firstComma = record.find(',');

    if (firstComma != std::string::npos) {
        std::cout << "The first comma is at position: " << firstComma << std::endl;
    } else {
        std::cerr << "No comma found in the string." << std::endl;
    }

    int secondComma = record.find(',', firstComma+1);
    int zipcode = 0;
    try {
        zipcode = stoi(record.substr(firstComma+1, secondComma-firstComma-1));
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << std::endl;
    } catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << std::endl;
    }

    return zipcode;
    
}

int main() {
    // This program goes through each block, and finds the greatest key (zipcode) value in the block.
    // Thus we are reading from a file, and writing to a file. We are keeping track of the block number, and the
    // greatest key value within the block. Once we've reached the end of the block, we write these out


    // File to write data out to
    string idxFile = "blockedIndex.txt";
    ofstream writeFile(idxFile);
    if (!writeFile.is_open()) {
        cerr << "Error: Could not open file 'blockedIndex.txt' for writing.\n";
        return 1;
    }
    
    // File to read information from
    ifstream readFile("blocked_postalcodes.txt");
    if (!readFile.is_open()) {
        cerr << "Error: Could not open file blocked_postalcodes.txt for reading.\n";
        return 1;
    }

    // It's simpler to count the block numbers ourselves.
    int blockNumber = 0;
    int maxZipcode = 0;

    // Skip past the metadata, iterate through each record in the block (using substrings and indices), and update our maxima
    string currentBlock;

    int start = 0, end = stoi(currentBlock.substr(0, 2)); // window to view the current record

    while (getline(readFile, currentBlock)) {
        while (end < currentBlock.length()) {
            string currentRecord = currentBlock.substr(start, end-start);
            int currentZipcode = findZipcode(currentRecord);
            maxZipcode = max(maxZipcode, currentZipcode);
            currentZipcode = 0;
            start = end+1;
            end = stoi(currentBlock.substr(start, 2));
        }
        // Moving to a new block. We need to print out RBN and maxZipcode to our output file, and reset our block-level variables

        writeFile << blockNumber << "," << maxZipcode << endl;

        blockNumber++;
        maxZipcode = 0;
        start = 0;
        end = stoi(currentBlock.substr(0, 2));
    }


    // At this point, we've iterated through every record in the file. 
    readFile.close();
    writeFile.close();

}