#include "HeaderBuffer.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;



int main(){
    cout << "testing of the write function\n" << endl;
    HeaderBuffer headerBuffer("testwrite.txt");
    // Set header fields
    headerBuffer.setFileStructureType("1.0");
    headerBuffer.setFileStructureVersion("1.0");
    headerBuffer.setHeaderSizeBytes(256);
    headerBuffer.setRecordSizeBytes(128);
    headerBuffer.setSizeFormatType("ASCII");
    headerBuffer.setPrimaryKeyIndexFileName("index.txt");
    headerBuffer.setprimaryKeyIndexFileSchema("sample");
    headerBuffer.setRecordCount(1000);
    headerBuffer.setBlockCount(1001);
    headerBuffer.setFieldCount(5);  // Set field count
    headerBuffer.setPrimaryKeyFieldIndex(1);  // Set primary key index
    headerBuffer.setRBNA(1);
    headerBuffer.setRBNS(1);
    headerBuffer.setstaleFlag(false);

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
    HeaderBuffer headerBuffer2("us_postal_codes.txt");
    headerBuffer2.readHeader();
    return 0;

}