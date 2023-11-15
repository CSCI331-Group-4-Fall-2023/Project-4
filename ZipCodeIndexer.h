// ----------------------------------------------------------------------------
/**
 * @file ZipCodeIndexer.h
 * @class ZipCodeIndexer
 * @brief Class for indexing ZIP code records from a file.
 * @author Emily Yang
 * @author Kent Biernath
 * @author Emma Hoffmann
 * @date 2023-10-16
 * @version 1.0
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  The ZipCodeIndexer class is responsible for creating and managing an index
  * of ZIP code records in a file. It provides methods for creating the index,
  * saving it to a file, loading it into RAM, and retrieving the position of a
  * specific ZIP code record in the file.
  *

  *
  Assumptions:
  *
   -- The file is in the same directory as the program.
  */
  // ----------------------------------------------------------------------------


// Include guards to prevent double inclusion of this header file.
#ifndef ZIPCODEINDEXER_H
#define ZIPCODEINDEXER_H

// Include necessary header files
#include <map>                ///< For std::map container
#include <fstream>            ///< For file operations
#include "ZipCodeBuffer.h"    ///< For accessing the ZipCodeBuffer class

/**
 * @class ZipCodeIndexer
 * @brief Class for indexing ZIP code records from a file.
 *
 * The ZipCodeIndexer class is responsible for creating and managing an index
 * of ZIP code records in a file. It provides methods for creating the index,
 * saving it to a file, loading it into RAM, and retrieving the position of a
 * specific ZIP code record in the file.
 */
class ZipCodeIndexer {
private:
    // Map to store each ZIP code and its position in the file.
    std::map<std::string, std::streampos> index;

    // File name of the index to be saved/loaded.
    std::string indexFileName;

    // Instance of ZipCodeBuffer to read ZIP code records from the file.
    ZipCodeBuffer buffer;

public:
    /**
     * @brief Constructor: initializes the ZipCodeIndexer with a file name and index file name.
     *
     * @param fileName The name of the file to index as a string.
     * @param fileType The type of the file, [C]SV or [L]ength-indicated
     * @param idxFileName The name of the index file to save/load as a string.
     */
    ZipCodeIndexer(std::ifstream &file, char fileType, const std::string& idxFileName);

    /**
     * @brief Method to create an index by reading the file and storing ZIP codes and their positions.
     *
     * This method reads the file and creates an index of ZIP code records by storing
     * each ZIP code and its position in the file.
     */
    void createIndex();

    /**
     * @brief Method to write the created index to a file.
     *
     * This method saves the created index to a file specified by the index file name.
     */
    void writeIndexToFile();

    /**
     * @brief Method to load the index from a file into RAM.
     *
     * This method loads the index data from a file into RAM for quick retrieval.
     */
    void loadIndexFromRAM();

    /**
     * @brief Method to get the position of a specific ZIP code record in the file.
     *
     * @param zipCode The ZIP code to find the position of.
     * @return The position of the ZIP code record in the file.
     */
    std::streampos getRecordPosition(const std::string& zipCode);
};

// End of the include guard.
#endif // ZIPCODEINDEXER_H
