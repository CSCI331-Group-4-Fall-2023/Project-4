// ----------------------------------------------------------------------------
/**
 * @file BlockBuffer.h
 * @class BlockBuffer
 * @brief Reads blocks of length-indicated records from a blocked file.
 * @author Kent Biernath
 * @author Andrew Clayton
 * @date 2023-11-14
 * @version 1.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n Reads a block of length-indicated records from a blocked file, which is
 *    passed to the buffer by reference in the constructor.
 * \n
 * \n  Each block starts with these five metadata fields:
 * \n  -- Length of metadata record (int)
 * \n  -- Relative Block Number (int)
 * \n  -- Number of records in the block (int)
 * \n  -- Previous Relative Block Number (int)
 * \n  -- Next Relative Block Number (int)
 * \n
 * \n The Relative Block Number (RBN) is used to navigate the file and starts
 *    at 0 for the first block. The Previous RBN and Next RBN are used to
 *    navigate the file in logical order. A Previous/Next RBN of -1 means the
 *    block is the first/last block.
 * \n
 * \n The records within each block are length-indicated and have no other
 *    delimiters. The length field is separated from the rest of the record
 *    by a comma delimiter.
 */
// ----------------------------------------------------------------------------

#ifndef BLOCKBUFFER_H
#define BLOCKBUFFER_H

using namespace std;

class BlockBuffer {
private: 
    std::ifstream &file;        // The ifstream to read blocks from.
    int numRecordsInBlock = 0;  // Number of records in the current block (read from metadata)
    int currentRBN = 0;         // Relative Block Number (RBN) of the current block
    int prevRBN = -1;           // RBN of the previous block in the linked list
    int nextRBN = 0;            // RBN of the next block in the linked list 
    int blockSize = 512; // TODO replace hardcoded 512 bytes with reading from the header record buffer
    int headerSize = 53; // TODO replace hardcoded bytes with reading from header record buffer

public:
    /**
     * @brief Construct a new Block Buffer object.
     * @param file The file to read.
     * @pre: The block is a string.
     * @post: A new Block Buffer object is created.
     */
    BlockBuffer(std::ifstream &file) : file(file) {}


    /**
     * @brief Unpacks the length-indicated records from the block into a string vector.
     * @return A vector of strings, the records within a block.
     * @pre: The file pointer is at the start of the records within the block after the block metadata was read.
     * @post: The block is unpacked into individual strings for each record in the block, returned as a vector.
     */
    vector<string> unpackBlockRecords();
    

    /**
     * @brief Reads the block metadata for the current block.
     * @pre The file pointer is at the start of the block before the 5 metadata fields.
     * @post The 5 metadata fields have been read into the member variables and the file pointer is after the metadata.
    */
    void readBlockMetadata();

    // Metadata getters
    int getCurrentRBN() const { return currentRBN; }
    int getPrevRBN() const { return prevRBN; }
    int getNextRBN() const { return nextRBN; }
    int getNumRecordsInBlock() const { return numRecordsInBlock; }


    /**
     * @brief Reads the block at the given Relative Block Number (RBN) and returns it as a vector of records in string form.
     * @return A vector of strings where each string represents a record within the block.
     * \n      The length indication field for each record is read but not returned in the string.
     * @pre: The file is open and in a blocked length-indicated file format.
     * @post: The block is broken down into records and the file pointer is after the records in the block.
     */
    vector<string> readBlock(int relativeBlockNumber);

    /**
     * @brief Reads the current block after the file pointer and returns it as a vector of records in string form.
     * @return A vector of strings where each string represents a record within the block.
     * \n      The length indication field for each record is read but not returned in the string.
     * @pre: The file pointer is at the start of the block.
     * @post: The block is broken down into records and the file pointer is after the records in the block.
     */
    vector<string> readCurrentBlock();

    /**
     * @brief Moves to and reads the next block and returns it as a vector of records in string form.
     * @return A vector of strings where each string represents a record within the block.
     * \n      The length indication field for each record is read but not returned in the string.
     * @pre: The file is open and in a blocked length-indicated file format.
     * @post: The block is broken down into records and the file pointer is after the records in the block.
     */
    vector<string> readNextBlock();
    //vector<string> readPreviousBlock();


    /**
     * @brief Calculates the address of a Relative Block Number (RBN) within the file.
     * @return The address of the RBN.
     * @pre The file metadata has been read.
     * @post The calculation results have been returned.
    */
    int calculateBlockAddress(int relativeBlockNumber);


    /**
     * @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
     * @pre The file is open.
     * @post The file pointer is moved to the start of the block at the given RBN.
    */
    void moveToBlock(int relativeBlockNumber);

};

//#include "BlockBuffer.cpp"

#endif