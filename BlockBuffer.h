// ----------------------------------------------------------------------------
/**
 * @file BlockBuffer.h
 * @class BlockBuffer
 * @brief This class unpacks a record from a block into a record buffer
 * @author Andrew Clayton
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
    string block;

public:
    // Constructors
    /**
     * @brief Default Constructor: Construct a new Block Buffer object
     * @pre: None
     * @post: A new Block Buffer object is created
     */
    BlockBuffer() { block = ""; }

    /**
     * @brief Construct a new Block Buffer object
     * @param String block is the block to set
     * @pre: The block is a string
     * @post: A new Block Buffer object is created
     */
    BlockBuffer(string block) { this->block = block; }

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
    
};

#include "BlockBuffer.cpp"

#endif