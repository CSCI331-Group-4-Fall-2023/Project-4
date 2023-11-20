// PrintBlock.h

#ifndef PRINTBLOCK_H
#define PRINTBLOCK_H

#include "BlockBuffer.h"

class PrintBlock {
private:
    BlockBuffer blockBuffer;

public:
    PrintBlock(std::ifstream& file, HeaderBuffer headerBuffer);

    // Print blocks sequentially by their physical ordering
    void physical();

    // Print blocks sequentially by their logical ordering
    void logical();
};
#include "printBlock.cpp"
#endif  // PRINTBLOCK_H