// ----------------------------------------------------------------------------
/**
 * @file HeaderBuffer.h
 * @class HeaderBuffer
 * @brief Represents a class for handling header data..
 * @author Emma Hoffmann
 * @author Kent Biernath
 * @date 2023-10-16
 * @version 1.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n The HeaderBuffer class is responsible for reading and writing header data to and from a file.
 * \n
 * \n Header Fields:
 * \n  -- File Structure Type (string)
 * \n  -- File Structure Version (string)
 * \n  -- Header Size (bytes) (int)
 * \n  -- Record Size (bytes) (int)
 * \n  -- Size Format Type (string)
 * \n  -- Primary Key Index File (string)
 * \n  -- Record Count (int)
 * \n  -- Field Count (int)
 * \n  -- Primary Key Field Index (int)
 * \n  -- Fields
 * \n     -- Zip Code (string)
 * \n     -- Place Name (string)
 * \n     -- State (string)
 * \n     -- County (string)
 * \n     -- Latiitude (double)
 * \n     -- Longitude (double)   
 * \n
 * \n Whenever readHeader is called, it reads the header data from the file specified in the constructor.
 * \n
 * \n The name of the header file to be opened is passed to the class constructor as a string.
 * \n
 * \n Assumptions:
 * \n  -- The header file is in the same directory as the program.
 * \n  -- The header file format follows a specific structure, as described in the code.
 */
// ----------------------------------------------------------------------------


#ifndef HEADERBUFFER_H
#define HEADERBUFFER_H

#include <string>
#include <vector>

class HeaderBuffer {
public:

    struct Field {
        std::string zipCode;
        std::string placeName;
        std::string state;
        std::string county;
        std::string latitude;
        std::string longitude;
    };


    /// @brief Constructor to initialize HeaderBuffer with a filename.
    ///@param filename The name of the header file to be opened as a string.
    HeaderBuffer(const std::string& filename);

    
    /// @brief Write the header data to a file.
    /// @pre The file must be successfully opened for writing.
    void writeHeader();

    /// @brief Read header data from a file.
    /// @pre The file must be successfully opened for reading.
    void readHeader();

    void setFileStructureType(const std::string& fileStructureType);
    void setFileStructureVersion(const std::string& fileStructureVersion);
    void setHeaderSizeBytes(int headerSizeBytes);
    void setRecordSizeBytes(int recordSizeBytes);
    void setSizeFormatType(const std::string& sizeFormatType);
    void setBlockSize(int blockSize);
    void setminimumBlockCapacity(int minimumBlockCapacity);
    void setPrimaryKeyIndexFileName(const std::string& primaryKeyIndexFileName);
    void setprimaryKeyIndexFileSchema(const std::string& primaryKeyIndexFileSchema);
    void setRecordCount(int recordCount);
    void setBlockCount(int blockCount);
    void setFieldCount(int fieldCount);
    void setPrimaryKeyFieldIndex(int primaryKeyFieldIndex);
    void setRBNA(int RBNA);
    void setRBNS(int RBNS);
    void setstaleFlag(bool staleFlag);
    void addField(const Field& field);

    std::string getFileStructureType() const;
    std::string getFileStructureVersion() const;
    int getHeaderSizeBytes() const;
    int getRecordSizeBytes() const;
    std::string getSizeFormatType() const;
    int getBlockSize() const;
    int getMinimumBlockCapacity() const;
    std::string getPrimaryKeyIndexFileName() const;
    int getRecordCount() const;
    int getBlockCount() const;
    int getFieldCount() const;
    int getPrimaryKeyFieldIndex() const;
    int getRBNA() const;
    int getRBNS() const;
    bool getStaleFlag() const;
    const std::vector<Field>& getFields() const;

private:
    std::string filename_;
    std::string fileStructureType_;
    std::string fileStructureVersion_;
    int headerSizeBytes_;
    int recordSizeBytes_;
    std::string sizeFormatType_;
    int blockSize_;
    int minimumBlockCapacity_;
    std::string primaryKeyIndexFileName_;
    std::string primaryKeyIndexFileSchema_;
    int recordCount_;
    int blockCount_;
    int fieldCount_;
    int primaryKeyFieldIndex_;
    int RBNA_;
    int RBNS_;
    bool staleFlag_;
    std::vector<Field> fields_;
};
#include "HeaderBuffer.cpp"
#endif // HEADERBUFFER_H
