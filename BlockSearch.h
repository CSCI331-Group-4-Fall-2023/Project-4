// ----------------------------------------------------------------------------
/**
 * @file BlockSearch.h
 * @class BlockSearch
 * @brief Uses the blocked index file to be able to search for specific records in the file
 * @author Andrew Clayton
 * @date 2023-11-14
 * @version 1.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n Opens in the index file, and reads through the indices, locating the block where the target should be, and
 * \n finding the specific record if it exists. If it does not, it will return a -1.
 * 
 *
 */
// ----------------------------------------------------------------------------

#ifndef BLOCKSEARCH_H
#define BLOCKSEARCH_H

#include <string>
using namespace std;

class BlockSearch {
private:

    // The index file to open
    string indexFile = "blocked_Index.txt";


public:
    /**
     * @brief Construct a new Block Search object
     * @pre: none
     * @post: A new BlockSearch object is created
    */
    BlockSearch() { }

    /**
     * @brief Constructor that takes in a blocked index file
     * @param indexFile: The file to open
     * @pre: A blocked index file exists
     * @post: A new BlockSearch object is created
    */
    BlockSearch(string idxFile);


    /**
     * @brief Searches for a record in the blocked index file by key (zipcode).
     * @param target: The zipcode to search for
     * @pre: A blocked index file exists
     * @post: The record is either found or not found
     * @return: The record if it is found, or a -1 if it is not found
    */
    string searchForRecord(int target);

    /**
     * @brief Displays the record to the console
     * @param record: The record to display, in its raw data form
     * @pre: A record exists
     * @post: The record is displayed to the console
    */
    void displayRecord(string record); 

};

//#include "BlockSearch.cpp"
#endif