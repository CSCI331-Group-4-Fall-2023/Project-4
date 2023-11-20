// ----------------------------------------------------------------------------
/**
 * @file ZipCodeTableViewer.cpp
 * @brief Console program for displaying a table of the most eastern, western,
 *      northern, and southern ZIP codes for each state code in a file.
 * @author Kent Biernath
 * @author Andrew Clayton
 * @author Emma Hoffmann, Emily Yang, Devon Lattery
 * @date 2023-11-19
 * @version 3.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n Displays a table of the most eastern, western, northern, and southern ZIP
 *    codes for each state code contained in a given file.
 * \n
 * \n It uses the ZipCodeBuffer class to retrieve each record as a
 *    ZipCodeRecord struct.
 * \n
 * \n As it processes each record, it stores the state codes in a sorted set
 *    and the most eastern/western/northern/southern ZIP codes and coordinates
 *    in maps.
 * \n
 * \n Once it has processed every record in the file, it displays a table
 *    with five columns on the console sorted alphabetically by state code.
 * \n
 * \n The table columns are (in order):
 * \n  -- State Code
 * \n  -- Easternmost ZIP Code
 * \n  -- Westernmost ZIP Code
 * \n  -- Northernmost ZIP Code
 * \n  -- Southernmost ZIP Code
 * \n
 * \n Length-indicated files also have a "Record Length" field at the start of
 *    the record.
 * \n
 * \n If the program is launched with command line arguments -Z or --Zip, it
 * \n will do a search. See ZipCodeRecordSearch.cpp and BlockSearch.cpp for
 *    details.
 * \n
 * \n  Assumptions:
 * \n  -- The file is in the same directory as the program.
 * \n  -- The file records always contain exactly six fields.
 * \n  -- The file has column headers on the first line.
 */ 
// ----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iomanip>
#include "ZipCodeBuffer.h"
#include "ZipCodeRecordSearch.h"
#include "ZipCodeIndexer.h"
#include "HeaderBuffer.h"
#include "BlockSearch.h"



int main(int argc, char* argv[]) {

    std::ifstream file;
    std::string fileName;
    char fileType = 'L'; // Default to length-indicated file type

    // Loop until a valid file name is provided by the user
    while (true) {
        // Prompt the user for a file name until a valid one is given
        
        std::cout << "Enter the file name to open: ";
        std::cin >> fileName;

        // Attempt to open the file
        file.open(fileName);

        if (file.is_open()) {
            std::cout << "File successfully opened." << std::endl;
            break;  // Exit the loop when a valid file is provided
        }
        else {
            std::cerr << "Failed to open the file. Please enter a valid file name." << std::endl;
        }
    }

    
    HeaderBuffer headerBuffer(fileName);

    if (fileName.find(".csv") != std::string::npos)
    {
        fileType = 'C';
    }
    else
    {
        // If not a CSV, then it is either a length-indicated or blocked file with a metadata record, so read metadata
        headerBuffer.readHeader();
        if (headerBuffer.getBlockSize() == 0)
        {
            // The file does not use blocks, so it is length-indicated
            
            fileType = 'L';
        }
        else
        {
            // The file uses blocks
            fileType = 'B';
        }
        
    }
    

    // Create a ZipCodeBuffer for accessing the records in the file
    ZipCodeBuffer recordBuffer(file, fileType, headerBuffer);
    ZipCodeRecord record;


    // If the program is given no arguments, display the table
    if (argc == 1) {

        // Make a set and maps to store the state code and ZIP code coordinate extrema
        std::set<std::string> stateCodes;
        std::map<std::string, std::vector<double>> stateCodeToCoordinatesMap;
        std::map<std::string, std::vector<std::string>> stateCodeToZipCodesMap;

        // Iterate through records until the terminal string "" is returned from the buffer
        while (true)
        {
            ZipCodeRecord record = recordBuffer.readNextRecord();
            if (record.zipCode == "") {
                // Exit the loop if the terminal string "" was returned from the buffer
                break;
            }

            // Try to add the state initial to the set and save the boolean result of whether it succeeded
            std::pair<std::set<std::string>::iterator, bool> result = stateCodes.insert(record.state);

            if (result.second) {
                // The initial was not already present in the set, so add it to the maps too
            
                // Create a vector with the current longitude and latitude values in all four extrema
                std::vector<double> coordinates;
                coordinates.push_back(record.longitude); // [0] Easternmost
                coordinates.push_back(record.longitude); // [1] Westernmost
                coordinates.push_back(record.latitude);  // [2] Northernmost
                coordinates.push_back(record.latitude);  // [3] Southernmost
                stateCodeToCoordinatesMap[record.state] = coordinates;

                // Create a vector with the current ZIP code in all four extrema
                std::vector<std::string> zipCodes;
                zipCodes.push_back(record.zipCode); // [0] Easternmost
                zipCodes.push_back(record.zipCode); // [1] Westernmost
                zipCodes.push_back(record.zipCode); // [2] Northernmost
                zipCodes.push_back(record.zipCode); // [3] Southernmost
                stateCodeToZipCodesMap[record.state] = zipCodes;
            }
            else {
                if (record.longitude < stateCodeToCoordinatesMap[record.state][0]) {
                    // New Easternmost (least longitude)
                    stateCodeToCoordinatesMap[record.state][0] = record.longitude;
                    stateCodeToZipCodesMap[record.state][0] = record.zipCode;
                }
                else if (record.longitude > stateCodeToCoordinatesMap[record.state][1]) {
                    // New Westernmost
                    stateCodeToCoordinatesMap[record.state][1] = record.longitude;
                    stateCodeToZipCodesMap[record.state][1] = record.zipCode;
                }

                if (record.latitude > stateCodeToCoordinatesMap[record.state][2]) {
                    // New Northernmost (greatest latitude)
                    stateCodeToCoordinatesMap[record.state][2] = record.latitude;
                    stateCodeToZipCodesMap[record.state][2] = record.zipCode;
                }
                else if (record.latitude < stateCodeToCoordinatesMap[record.state][3]) {
                    // New Southernmost
                    stateCodeToCoordinatesMap[record.state][3] = record.latitude;
                    stateCodeToZipCodesMap[record.state][3] = record.zipCode;
                }
            }
        }

        // Display the table column headers
        std::cout << std::left << std::setw(8) << "State"
                  << std::left << std::setw(8) << "East"
                  << std::left << std::setw(8) << "West"
                  << std::left << std::setw(8) << "North"
                  << std::left << std::setw(8) << "South" << std::endl;
        // Display the spacers below the table column headers
        for (size_t i = 0; i < 5; i++)
        {
            std::cout << std::left << std::setw(8) << "-----";
        }
        std::cout << std::endl;

        // Display the records in the table sorted alphabetically by state name.
        // Displays State, Easternmost ZIP Code, Westernmost ZIP Code, Northernmost ZIP Code, and Southernmost ZIP Code per row
        for (const std::string& stateCode : stateCodes) {

            std::cout << std::left << std::setw(8) << stateCode;
            for (const std::string& zipCode : stateCodeToZipCodesMap[stateCode]) {
                std::cout << std::setw(8) << zipCode;
            }
            std::cout << std::endl;
        }
    }
    else
    {
        // If command line parameters were given, do a search.

        if (fileType != 'B') {
            // Generate an index
            std::ifstream searchFile(fileName);
            ZipCodeIndexer index(searchFile, fileType, fileName + "_index.txt", headerBuffer);
            index.createIndex();
            index.writeIndexToFile();

            const std::string COMMAND_NAME = std::string(argv[0]);
            // If no flags are used, display the default message
            if (argc == 1) {
                defaultMessage(COMMAND_NAME);
                return 0;
            } // If a help flag is used, display usage information
            else if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
                displayHelp(COMMAND_NAME);
                return 0;
            }
            else {
                std::string flag = "";
                for (int i = 1; i < argc; i++) {
                    if (flag == "") {
                        flag = std::string(argv[i]);
                    }
                    else if ((flag == "-Z" || flag == "-z" || flag == "--zipcode") && isNumber(argv[i])) {
                        searchHelper(fileName, fileType, argv[i]);
                        flag = "";
                    }
                    else {
                        std::cerr << "INVALID ARGUMENT" << std::endl;
                        defaultMessage(COMMAND_NAME);
                        return 1;
                    }
                }
            }
        }
        else // else fileType == B
        {
            // Run blocked file search

            for (int i = 1; i < argc; ++i) {
                string arg = argv[i];

                // Check if argument starts with -z or -Z
                if (arg.size() > 2 && (arg[0] == '-' && (arg[1] == 'z' || arg[1] == 'Z'))) {
                    string zipcodeStr = arg.substr(2);  // Extract the zipcode part
                    int zipcode;

                    try {
                        BlockSearch searcher;
                        zipcode = stoi(zipcodeStr);
                        string result = searcher.searchForRecord(zipcode);

                        if (result != "-1") {
                            cout << "Information for zipcode " << zipcode << ":\n";
                            searcher.displayRecord(result);
                        } else {
                            cout << "Zipcode " << zipcode << " not found." << "\n\n";
                        }
                    } catch (const invalid_argument& ia) {
                        cerr << "Invalid zipcode format: " << zipcodeStr << endl;
                    }
                } else {
                    // Invalid argument format
                    cout << "Invalid argument: " << arg << endl;
                    cout << "Please use the format: -z<zipcode> or -Z<zipcode>" << endl;
                }
            }
        }
    }

    file.close();
    return 0;
}
