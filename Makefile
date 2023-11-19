# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++11

# Source files
SOURCES = ZipCodeTableViewer.cpp ZipCodeBuffer.cpp ZipCodeIndexer.cpp ZipCodeRecordSearch.cpp BlockBuffer.cpp HeaderBuffer.cpp

# Output executable name
OUTPUT = ZipCode.exe

# Default target
all: $(OUTPUT)

# Compile the program
$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

# Clean the compiled files
clean:
	rm -f $(OUTPUT)

.PHONY: all clean