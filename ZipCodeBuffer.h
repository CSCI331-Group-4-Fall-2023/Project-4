// ----------------------------------------------------------------------------
/**
 * @file ZipCodeBuffer.h
 * @class ZipCodeBuffer
 * @brief Class to parse ZIP code records in a CSV file.
 * @author Kent Biernath
 * @author Emma Hoffmann, Emily Yang
 * @author Rediet Gelaw, Devon Lattery, Bhumika Basnet
 * @date 2023-10-16
 * @version 2.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n The ZipCodeBuffer class reads a record from a CSV with these six fields:
 * \n  -- ZIP Code (string)
 * \n  -- Place Name (string)
 * \n  -- State Codes (string)
 * \n  -- County (string)
 * \n  -- Latitude (double)
 * \n  -- Longitude (double)
 * \n
 * \n Whenever readNextRecord is called, it reads the next record from the
 *    CSV file and returns it in a ZipCodeRecord struct.
 * \n
 * \n The name of the CSV file to be opened is passed to the class constructor
 *    as a string.
 * \n
 * \n  Assumptions:
 * \n  -- The CSV file is in the same directory as the program.
 * \n  -- The CSV file records always contain exactly six fields.
 * \n  -- The CSV file has column headers on the first line.
 */
// ----------------------------------------------------------------------------


#ifndef ZIPCODEBUFFER_H
#define ZIPCODEBUFFER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
//#include "BlockBuffer.h"

/// @brief Structure to hold a ZIP Code record.
struct ZipCodeRecord {
    std::string zipCode;
    std::string placeName;
    std::string state;
    std::string county;
    double latitude = 0.0;
    double longitude = 0.0;
};

/// @brief The ZipCodeBuffer class parses the CSV file one record at a time
///     and returns the fields in a ZipCodeRecord struct.
class ZipCodeBuffer {
private:
    std::string fileName;
    std::ifstream file;
    char fileType;
    //vector<ZipCodeRecord> blockRecords; // Stores the current block of records if using a block file format
    //int blockRecordsIndex = -1; // Default to index below 0 so it retrieves the first block on first check

public:
    /**
     * @brief Constructor that accepts the CSV filename.
     * @pre The CSV file has one column header row to skip.
     * @post The CSV file is opened and the first header row
     *         was skipped.
     * @param fileName The name of the CSV file to open.
     * @param fileType The type of the file. Case insensitive, stored in uppercase.
     * \n  -- 'C' = CSV, comma-separated values
     * \n  -- 'L' = Length-indicated file structure format with the first field
     *              describing the length of the record.
     */
    ZipCodeBuffer(std::string fileName, char fileType);

    /** @brief Destructor to close the file when done. */
    ~ZipCodeBuffer();

    /**
     * @brief Reads the next ZIP Code record from the CSV file.
     *
     * This function reads the next line from the CSV file, parses it into a
     * ZipCodeRecord struct, and returns it.
     * 
     * @pre The next CSV record must have exactly six fields.
     * @post The next record in the file was returned.
     *
     * @return The next ZIP Code record from the CSV file. When it reaches the
     *      end of the file or an invalid record, it returns a ZIP code of ""
     *      as a terminal string.
     */
    ZipCodeRecord readNextRecord();

    /// @brief Method to get the current position in the file.
    std::streampos getCurrentPosition();
    /// @brief Method to set the current position in the file to a given streampos.
    std::ifstream& setCurrentPosition(std::streampos);
};

#endif // ZIPCODEBUFFER_H