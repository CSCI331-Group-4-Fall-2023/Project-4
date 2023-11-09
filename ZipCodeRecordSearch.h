/*****************************************************************************
 * @file ZipCodeRecordSearch.h
 * @author Devon Lattery
 * @author Kent Biernath
 * @author Emma Hoffmann
 * @author Emily Yang, Rediet Gelaw, Bhukima Basnet
 * @brief Console program for searching a record file for one or more ZIP
 *      codes as given by the user input.
 * @version 1.0
 * @date 2023-10-16
 *
 *****************************************************************************
 * @details
 * \n Searches the records for given search terms in the console using flags.
 *    To perform a search, the -Z flag should be used followed by the ZIP code.
 *    e.g. -Z 12345 searches for the ZIP code 12345.
 * \n
 * \n If the record is in the file, it will be displayed in the console.
 *    e.g. -Z 12345 would return the following:
 *
 * \n Zip Code: 12345
 * \n Place Name: Example City
 * \n State: EG
 * \n County: Example
 * \n Latitude: 12.3456 Longitude: -12.3456
 * \n
 * \n If the record is not in the file, the console will display the message:
 *      No record of 12345
 * \n
 * \n The user may enter as many searches as they like, using the -Z flag for
 *    every search entry. This will result in multiple lines in the results
 *    table and any records not in the file will be stated after the table.
 *    e.g. -Z 12345 -Z 23456 -Z 34567 -Z 45678 would return the following:
 * \n
 * \n Zip Code: 12345
 * \n  Place Name: Example City
 * \n State: EG
 * \n County: Example
 * \n Latitude: 12.3456 Longitude: -12.3456
 * \n
 * \n Zip Code: 34567
 * \n Place Name: Sample City
 * \n State: ST
 * \n County: Sample
 * \n Latitude: 34.5678 Longitude: -34.5678
 * \n
 * \n No record of 23456
 * \n
 * \n No record of 45678
 * \n
 * \n Assumptions:
 * \n -- The record file is in the same directory as the program.
 * \n -- The first line of the file contains the label for each column.
 *****************************************************************************/

#pragma once

#ifndef ZIPCODERECORDSEARCH_H
#define ZIPCODERECORDSEARCH_H

#include <string>

/// See ZipCodeRecordSearch.cpp for details.
bool isNumber(const char* s);
void displayHelp(const std::string& commandName);
void defaultMessage(const std::string& commandName);
void searchHelper(std::string fileName, char fileType, char* zip);

#endif
