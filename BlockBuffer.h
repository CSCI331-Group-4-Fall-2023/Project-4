// ----------------------------------------------------------------------------
/**
 * @file BlockBuffer.h
 * @class BlockBuffer
 * @brief This class unpacks a record from a block into a record buffer
 * @author Andrew Clayton
 * @author Kent Biernath
 * @date 11/9/2023
 * @version 1.0
 */
 // ----------------------------------------------------------------------------
 /**
  * @details
  *
  * The block buffer unpacks a record from a block into a record buffer.
  * So this will break down a block into indiviual records (as an array of strings)
  */
  // ----------------------------------------------------------------------------

#ifndef BLOCKBUFFER_H
#define BLOCKBUFFER_H

using namespace std;

class BlockBuffer {
private: 
    std::ifstream &file;              // The ifstream to read blocks from.
    string block = "";               // The current block in string form
    int numRecordsInBlock = 0;  // Number of records in the current block (read from metadata)
    int currentRBN = -1;        // Relative Block Number (RBN) of the current block
    int prevRBN = -1;           // RBN of the previous block in the linked list
    int nextRBN = -1;           // RBN of the next block in the linked list 
    int blockSize = 512; // TODO replace hardcoded 512 MB with reading from the header record buffer
    int headerSize = 55; // TODO replace hardcoded with reading from header record buffer


public:
    
    // // Constructors
    // /**
    //  * @brief Default Constructor: Construct a new Block Buffer object
    //  * @pre: None
    //  * @post: A new Block Buffer object is created
    //  */
    // BlockBuffer() : block("") {}
 
    // /**
    //  * @brief Construct a new Block Buffer object
    //  * @param block is the block to set
    //  * @pre: The block is a string
    //  * @post: A new Block Buffer object is created
    //  */
    // BlockBuffer(string block) : block(block) {}
    
    /**
     * @brief Construct a new Block Buffer object
     * @param file is the file to set
     * @pre: The block is a string
     * @post: A new Block Buffer object is created
     */
    BlockBuffer(std::ifstream &file) : file(file) {}

    // Getters and setters
    /**
     * @brief Get the block
     * @return the block as a string
     * @pre: The block is a string
     * @post: The block is returned
    */
    string getBlock() const { return block; }

    /**
     * @brief Set the block
     * @param block the block to set
     * @pre: The block is a string
     * @post: The block is set
    */
    void setBlock(string block) { this->block = block; }

    /**
     * @brief Creates a vector of records (stored as strings at this point) from a block.
     * @return a vector of strings, the records within a block
     * @pre: The block is a string
     * @post: The block is broken down into records
     */
    vector<string> createRecords();
    
    

    /**
     * @brief Reads the block metadata for the current block.
     * @pre blockStream is at the start of the block before the 5 metadata fields.
     * @post The 5 metadata fields have been read into the member variables and the istringstream
     * pointer is after the metadata fields.
    */
    void readBlockMetadata();

    // Metadata getters
    int getCurrentRBN() const { return currentRBN; }
    int getPrevRBN() const { return prevRBN; }
    int getNextRBN() const { return nextRBN; }
    int getNumRecordsInBlock() const { return numRecordsInBlock; }



    // TODO document
    // It is given friend access to ZipCodeBuffer

    /**
     * @brief Reads the next block and returns it as a vector of records in string form.
     * @param file
     * @return a vector of strings, the records within a block
     * @pre: The file pointer is at the start of the block.
     * @post: The block is broken down into records and the file pointer is after the block.
     */
    vector<string> readNextBlock();
    vector<string> readPreviousBlock();







    int calculateBlockAddress(int relativeBlockNumber);

    /**
     * @brief Moves the file pointer to the address of the block at the given Relative Block Number (RBN).
     * @return None.
     * @pre The file is open.
     * @post The file pointer is moved to the start of the block at the given RBN.
    */
    void moveToBlock(int rbn);

};

//#include "BlockBuffer.cpp"

#endif