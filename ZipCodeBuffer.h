// ----------------------------------------------------------------------------
/**
 * @file ZipCodeBuffer.h
 * @class ZipCodeBuffer
 * @brief Class to parse ZIP code records in a file.
 * @author Kent Biernath
 * @author Emma Hoffmann, Emily Yang
 * @date 2023-11-11
 * @version 3.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n The ZipCodeBuffer class reads a record from a file with these six fields:
 * \n  -- ZIP Code (string)
 * \n  -- Place Name (string)
 * \n  -- State Code (string)
 * \n  -- County (string)
 * \n  -- Latitude (double)
 * \n  -- Longitude (double)
 * \n
 * \n Whenever readNextRecord is called, it reads the next record from the
 *    file and returns it in a ZipCodeRecord struct after parsing it with
 *    parseRecord.
 * \n
 * \n The name of the file to be opened is passed to the class constructor
 *    as a string.
 * \n
 * \n  Assumptions:
 * \n  -- The file is in the same directory as the program.
 * \n  -- The file records always contain exactly six fields.
 * \n  -- The file has column headers on the first line.
 */
// ----------------------------------------------------------------------------


#ifndef ZIPCODEBUFFER_H
#define ZIPCODEBUFFER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "BlockBuffer.h"

/// @brief Structure to hold a ZIP Code record.
struct ZipCodeRecord {
    std::string zipCode;
    std::string placeName;
    std::string state;
    std::string county;
    double latitude = 0.0;
    double longitude = 0.0;
};

/// @brief The ZipCodeBuffer class parses the file one record at a time
///     and returns the fields in a ZipCodeRecord struct.
class ZipCodeBuffer {
private:
    std::string fileName;
    std::ifstream file;
    char fileType;
    vector<string> blockRecords; // Stores the current block of records if using a block file format
    int blockRecordsIndex = -1; // Default to index below 0 so it retrieves the first block on first check
    BlockBuffer blockBuffer; // Stores the block metadata if using a block file format

public:
    /**
     * @brief Constructor that accepts the filename.
     * @pre The file has one column header row to skip.
     * @post The file is opened and the header row is skipped.
     * @param fileName The name of the file to open.
     * @param fileType The type of the file. Case insensitive, stored in uppercase.
     * \n  -- 'C' = CSV, comma-separated values.
     * \n  -- 'L' = Length-indicated file structure format with the first field
     *              describing the length of the record.
     * \n  -- 'B' = Blocked length-indicated records.
     */
    ZipCodeBuffer(std::string fileName, char fileType);

    /** @brief Destructor to close the file when done. */
    ~ZipCodeBuffer();

    /**
     * @brief Parses a string into a ZipCodeRecord struct.
     * 
     * @pre Receives a string to parse.
     * @post The ZipCodeRecord struct is filled with data from the string and returned.
     *
     * @param recordString The string to parse into a ZipCodeRecord struct.
     * \n It must have six fields separated by commas and be in this order:
     * \n  -- ZIP Code (string)
     * \n  -- Place Name (string)
     * \n  -- State Code (string)
     * \n  -- County (string)
     * \n  -- Latitude (double)
     * \n  -- Longitude (double)
     * 
     * @return Returns the ZipCodeRecord struct filled with data parsed from the string.
     * \n If the record string is malformed, it returns an empty string for the zipCode
     *    field as the terminal string.
     */
    ZipCodeRecord parseRecord(std::string);

    /**
     * @brief Reads the next ZIP Code record from the file.
     *
     * This function reads the next line from the file, parses it into a
     * ZipCodeRecord struct, and returns it.
     * 
     * @pre The next record must have exactly six fields.
     * @post The next record in the file was returned.
     *
     * @return The next ZIP Code record from the file. When it reaches the
     *      end of the file or an invalid record, it returns a ZIP code of ""
     *      as a terminal string.
     */
    ZipCodeRecord readNextRecord();

    /// @brief Method to get the current position in the file.
    std::streampos getCurrentPosition();
    /// @brief Method to set the current position in the file to a given streampos.
    std::ifstream& setCurrentPosition(std::streampos);

    // Give BlockBuffer access to private member functions and variables.
    friend class BlockBuffer;
};

#endif // ZIPCODEBUFFER_H