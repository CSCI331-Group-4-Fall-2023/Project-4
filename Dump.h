// ----------------------------------------------------------------------------
/**
 * @file Dump.h
 * @class Dump
 * @brief Class for dumping and printing block records in logical and physical
 *        order and for dumping the simple index for the blocks.
 * @author Kent Biernath
 * @author Tristan Adams
 * @date 2023-11-19
 * @version 1.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n The Dump class provides methods to dump and print records in logical and
 *    physical order. It takes a `ZipCodeBuffer` object during initialization
 *    and uses it to access and process ZIP code records.
 * \n
 * \n The constructor takes a reference to a `ZipCodeBuffer` object, initializing
 *    the `BlockBuffer` and `HeaderBuffer` for further use. It also reads the
 *    header information from the file.
 * \n
 * \n The class includes methods for dumping records in logical order (`dumpLogicalOrder`),
 *    dumping records in physical order (`dumpPhysicalOrder`), and printing a block
 *    index from a specified file (`dumpBlockIndex`).
 * \n
 * \n The logical order dump iterates through the records in a file using the
 *    `readNextBlock` method of `BlockBuffer`.
 * \n
 * \n The physical order dump iterates through the blocks in a file using the
 *    `readBlock` method of `BlockBuffer`.
 * \n
 * \n The `printBlock` method is used internally to print the records within a block.
 * \n
 * \n The class assumes that the ZIP code records are stored in a file and follow
 *    a specific format. It also assumes that the file has header information and
 *    contains a specific structure of blocks.
 */
// ----------------------------------------------------------------------------

#ifndef PRINTBLOCK_H
#define PRINTBLOCK_H

#include "Dump.h"
#include <iostream>
#include <iomanip>
#include "ZipCodeBuffer.h"
#include "BlockBuffer.h"
#include "HeaderBuffer.h"

#include <string>
#include <vector>
#include <sstream> 
#include <fstream>

class Dump {
private:
    ZipCodeBuffer &recordBuffer;
    BlockBuffer &blockBuffer;
    HeaderBuffer &headerBuffer;

public:
    Dump(ZipCodeBuffer &recordBuffer);

    /** 
     *  @brief Dump records in logical order.
     *  Prints the List Head and Avail Head Relative Block Numbers (RBNS and RBNA) and iterates through
     *  each block in logical order, printing its contents.
     */
    void dumpLogicalOrder();

    /**
     *  @brief Dump records in physical order.
     *  Prints the List Head and Avail Head Relative Block Numbers (RBNS and RBNA) and iterates through
     *  each block in physical order, printing its contents.
     */
    void dumpPhysicalOrder();

    /**
     *  @brief Print a block of records.
     *  Prints the Previous Relative Block Number (PrevRBN), the Zip Codes from the records in the block,
     *  and the Next Relative Block Number (NextRBN).
     *  @param records A vector of strings representing the records in the block.
     */
    void printBlock(std::vector<std::string> records);

    /**
     *  @brief Dump the block index from a file.
     *  Reads and prints the Relative Block Numbers (RBN) and Primary Key values from each line of the specified file.
     *  @param filename The name of the file containing the block index.
     */
    void dumpBlockIndex(const std::string& filename);
};
#endif  // PRINTBLOCK_H