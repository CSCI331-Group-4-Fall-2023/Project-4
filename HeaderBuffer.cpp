/// @file HeaderBuffer.cpp
/// @class HeaderBuffer
/// @brief Implementation of the HeaderBuffer class for for handling header data.
#include "HeaderBuffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


    /// @brief Constructor to initialize HeaderBuffer with a filename.
    /// @param filename The name of the header file to be opened as a string.
    HeaderBuffer::HeaderBuffer(const std::string& filename) : filename_(filename) {
    }

    /// @brief Write the header data to a file.
    /// @pre The file must be successfully opened for writing.
    void HeaderBuffer::writeHeader() {
        std::ofstream file(filename_);

        if (!file.is_open()) {
            // Print an error mesage if the file cannot be opened
            std::cerr << "Error opening the file." << std::endl;
            return;
        }

        //good version for top line printing
        /*
        file<< "File structure type: "<< fileStructureType_ << " - File structure version: " << 
        fileStructureVersion_ <<" - Header Size (bytes): " << headerSizeBytes_ <<" - Record Size (bytes): " << recordSizeBytes_ <<" - Size Format Type: "
        << sizeFormatType_ <<" - block size: " << blockSize_ <<" - min block capacity: " << minimumBlockCapacity_ << " - Primary Key Index File: " 
        << primaryKeyIndexFileName_ << " - Primary Key Index File scheam: " << primaryKeyIndexFileSchema_ <<  " - Record Count: " << recordCount_ 
        << " - Block Count: " << blockCount_ << " - Field Count: " << fieldCount_ << " - Primary Key: " << primaryKeyFieldIndex_ << " - RBNA link: " 
        << RBNA_ <<" - RBNS link: " << RBNS_<<" - Stale Flag link: " << staleFlag_ << std::endl;
        */
        //version for seeing all the stuff
        file << "Read Header:" << std::endl;
        file << " - File structure type: " << fileStructureType_ << std::endl;
        file << " - File structure version: " << fileStructureVersion_ << std::endl;
        file << " - Header Size (bytes): " << headerSizeBytes_ << std::endl;
        file << " - Record Size (bytes): " << recordSizeBytes_ << std::endl;
        file << " - Size Format Type: " << sizeFormatType_ << std::endl;
        file << " - Primary Key Index File: " << primaryKeyIndexFileName_ << std::endl;
        file << " - Record Count: " << recordCount_ << std::endl;
        file << " - Field Count: " << fieldCount_ << std::endl;
        file << " - Primary Key: " << primaryKeyFieldIndex_ << std::endl;

        for (const Field& field : fields_) {
            file << std::endl;
            file << "- Fields:" << std::endl;
            file << "   - Zip Code: " << field.zipCode << std::endl;
            file << "   - Place Name: " << field.placeName << std::endl;
            file << "   - State: " << field.state << std::endl;
            file << "   - County: " << field.county << std::endl;
            file << "   - Latitude: " << field.latitude << std::endl;
            file << "   - Longitude: " << field.longitude << std::endl;
        }

        file << std::endl;
        file << "Data:" << std::endl;

        file.close();
    }

    /// @brief Reader header data from a file.
    /// @pre The file must be successfully opened for reading.
    void HeaderBuffer::readHeader() {
        std::ifstream file(filename_);

        if (!file.is_open()) {
            // Print an error mesage if the file cannot be opened
            std::cerr << "Error opening the file." << std::endl;
            return;
        }

        std::string line;

        while (std::getline(file, line)) {
            if (line.find("file structure type: ") != std::string::npos) {
                fileStructureType_ = line.substr(line.find(": ") + 2);
            }
            else if (line.find("version of your file structure type: ") != std::string::npos) {
                fileStructureVersion_ = line.substr(line.find(": ") + 2);
            }
            else if (line.find("header record size in bytes: ") != std::string::npos) {
                headerSizeBytes_ = std::stoi(line.substr(line.find(": ") + 2));
            }
            else if (line.find("count of bytes for each record size integer (if fixed-size): ") != std::string::npos) {
                recordSizeBytes_ = std::stoi(line.substr(line.find(": ") + 2));
            }
            else if (line.find("size format type {ASCII or binary}: ") != std::string::npos) {
                sizeFormatType_ = line.substr(line.find(": ") + 2);
            }
            else if (line.find("primary key index file name: ") != std::string::npos) {
                primaryKeyIndexFileName_ = line.substr(line.find(": ") + 2);
            }
            else if (line.find("record count: ") != std::string::npos) {
                recordCount_ = std::stoi(line.substr(line.find(": ") + 2));
            }
            else if (line.find("count of fields per record: ") != std::string::npos) {
                fieldCount_ = std::stoi(line.substr(line.find(": ") + 2));
            }
            else if (line == "Primary Key: ") {
                primaryKeyFieldIndex_ = std::stoi(line.substr(line.find(": ") + 2));
            }
            else if (line == " - Zip Code:") {
                Field field;
                while (std::getline(file, line)) {
                    if (line.find("   - Zip Code: ") != std::string::npos) {
                        field.zipCode = line.substr(line.find(": ") + 2);
                    }
                    else if (line.find("   - Place Name: ") != std::string::npos) {
                        field.placeName = line.substr(line.find(": ") + 2);
                    }
                    else if (line.find("   - State: ") != std::string::npos) {
                        field.state = line.substr(line.find(": ") + 2);
                    }
                    else if (line.find("   - County: ") != std::string::npos) {
                        field.county = line.substr(line.find(": ") + 2);
                    }
                    else if (line.find("   - Latitude: ") != std::string::npos) {
                        field.latitude = line.substr(line.find(": ") + 2);
                    }
                    else if (line.find("   - Longitude: ") != std::string::npos) {
                        field.longitude = line.substr(line.find(": ") + 2);
                    }
                }
                fields_.push_back(field);
            }
        }

        file.close();
    }

    /// @brief Setters for various header fields.
    /// @param fileStructureType The file structure type as a string.
    void HeaderBuffer::setFileStructureType(const std::string& fileStructureType) {
        fileStructureType_ = fileStructureType;
    }

    /// @param fileStructureVersion The file structure version as a string.
    void HeaderBuffer::setFileStructureVersion(const std::string& fileStructureVersion) {
        fileStructureVersion_ = fileStructureVersion;
    }

    /// @param headerSizeBytes The header size in bytes as an integer.
    void HeaderBuffer::setHeaderSizeBytes(int headerSizeBytes) {
        headerSizeBytes_ = headerSizeBytes;
    }

    /// @param recordSizeBytes The record size in bytes as an integer.
    void HeaderBuffer::setRecordSizeBytes(int recordSizeBytes) {
        recordSizeBytes_ = recordSizeBytes;
    }

    /// @param sizeFormatType The size format type as a string (ASCII or binary).
    void HeaderBuffer::setSizeFormatType(const std::string& sizeFormatType) {
        sizeFormatType_ = sizeFormatType;
    }

    /// @param blockSize The size of the blocks.
    void HeaderBuffer::setBlockSize(int blockSize) {
        blockSize_ = blockSize;
    }

    /// @param minimumBlockCapacity The smallest amount of a block that can be filled.
    void HeaderBuffer::setminimumBlockCapacity(int minimumBlockCapacity) {
        minimumBlockCapacity_ = minimumBlockCapacity;
    }

    /// @param primaryKeyIndexFileName The primary key index file name as a string.
    void HeaderBuffer::setPrimaryKeyIndexFileName(const std::string& primaryKeyIndexFileName) {
        primaryKeyIndexFileName_ = primaryKeyIndexFileName;
    }

    /// @param primaryKeyIndexFileSchema The info on how to read the index file.
    void HeaderBuffer::setprimaryKeyIndexFileSchema(const std::string& primaryKeyIndexFileSchema) {
        primaryKeyIndexFileSchema_ = primaryKeyIndexFileSchema;
    }

    /// @param recordCount The record count as an integer.
    void HeaderBuffer::setRecordCount(int recordCount) {
        recordCount_ = recordCount;
    }

    /// @param blockCount The block count as an integer.
    void HeaderBuffer::setBlockCount(int blockCount) {
        blockCount_ = blockCount;
    }

    /// @param fieldCount The field count as an integer.
    void HeaderBuffer::setFieldCount(int fieldCount) {
        fieldCount_ = fieldCount;
    }

    /// @param primaryKeyFieldIndex The primary key field index as an integer.
    void HeaderBuffer::setPrimaryKeyFieldIndex(int primaryKeyFieldIndex) {
        primaryKeyFieldIndex_ = primaryKeyFieldIndex;
    }

    /// @param RBNA The RBNA as an integer.
    void HeaderBuffer::setRBNA(int RBNA) {
        RBNA_ = RBNA;
    }

    /// @param RBNS The RBNA as an integer.
    void HeaderBuffer::setRBNS(int RBNS) {
        RBNS_ = RBNS;
    }

    /// @param staleFlag The tells if the header record is stale.
    void HeaderBuffer::setstaleFlag(bool staleFlag) {
        staleFlag_ = staleFlag;
    }

    /// @brief Add a field to the header.
    /// @param field The Field structure to be added to the header.
    void HeaderBuffer::addField(const Field& field) {
        fields_.push_back(field);
    }

    /// @brief Getters for header fields.
    std::string HeaderBuffer::getFileStructureType() const {
        return fileStructureType_;
    }

    std::string HeaderBuffer::getFileStructureVersion() const {
        return fileStructureVersion_;
    }

    int HeaderBuffer::getHeaderSizeBytes() const {
        return headerSizeBytes_;
    }

    int HeaderBuffer::getRecordSizeBytes() const {
        return recordSizeBytes_;
    }

    std::string HeaderBuffer::getSizeFormatType() const {
        return sizeFormatType_;
    }

    int HeaderBuffer::getBlockSize() const {
        return blockSize_;
    }

    int HeaderBuffer::getMinimumBlockCapacity() const {
        return minimumBlockCapacity_;
    }

    int HeaderBuffer::getBlockCount() const {
        return blockCount_;
    }
    std::string HeaderBuffer::getPrimaryKeyIndexFileName() const {
        return primaryKeyIndexFileName_;
    }

    int HeaderBuffer::getRecordCount() const {
        return recordCount_;
    }

    int HeaderBuffer::getFieldCount() const {
        return fieldCount_;
    }

    int HeaderBuffer::getPrimaryKeyFieldIndex() const {
        return primaryKeyFieldIndex_;
    }

    int HeaderBuffer::getRBNA() const {
        return RBNA_;
    }

    int HeaderBuffer::getRBNS() const {
        return RBNS_;
    }

    bool HeaderBuffer::getStaleFlag() const {
        return staleFlag_;
    }
    //const std::vector<Field>& HeaderBuffer::getFields() const {
     //   return fields_;
   // }

int headerBuffer() {
    HeaderBuffer headerBuffer("header.txt");

    // Set header fields
    headerBuffer.setFileStructureType("1.0");
    headerBuffer.setFileStructureVersion("1.0");
    headerBuffer.setHeaderSizeBytes(256);
    headerBuffer.setRecordSizeBytes(128);
    headerBuffer.setSizeFormatType("ASCII");
    headerBuffer.setPrimaryKeyIndexFileName("index.txt");
    headerBuffer.setRecordCount(1000);
    headerBuffer.setFieldCount(2);  // Set field count
    headerBuffer.setPrimaryKeyFieldIndex(1);  // Set primary key index

    // Add fields
    HeaderBuffer::Field field1;
    field1.zipCode = "string";
    field1.placeName = "string";
    field1.state = "string";
    field1.county = "string";
    field1.latitude = "double";
    field1.longitude = "double";
    headerBuffer.addField(field1);

    // Write the header to a file
    headerBuffer.writeHeader();

    // Read the header from a file
    headerBuffer.readHeader();

    return 0;
}
