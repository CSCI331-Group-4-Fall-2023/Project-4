// PrintBlock.h

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

    // Print blocks sequentially by their physical ordering
    void dumpPhysicalOrder();

    // Print blocks sequentially by their logical ordering
    void dumpLogicalOrder();


    void printBlock(std::vector<std::string> records);

    void dumpBlockIndex(const std::string& filename);
};
#endif  // PRINTBLOCK_H