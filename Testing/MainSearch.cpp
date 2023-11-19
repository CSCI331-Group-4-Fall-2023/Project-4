// ----------------------------------------------------------------------------
/**
 * @file MainSearch.cpp
 * @class MainSearch
 * @brief Uses the BlockSearch class to find zipcodes listed in command line arguments.
 * @author Andrew Clayton
 * @date 2023-17-14
 * @version 1.0
 */
// ----------------------------------------------------------------------------
/**
 * @details
 * \n Searches for zipcode designated by command line arguments using the BlockSearch class.
 */
// ----------------------------------------------------------------------------

#include "BlockSearch.cpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    BlockSearch searcher; // Assuming the default constructor is appropriate

    if (argc == 1) {
        // No arguments provided, display usage information
        cout << "Usage: " << argv[0] << " -z<zipcode> [-z<zipcode> ...]" << endl;
        cout << "Example: " << argv[0] << " -z56301 -Z61072" << endl;
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        // Check if argument starts with -z or -Z
        if (arg.size() > 2 && (arg[0] == '-' && (arg[1] == 'z' || arg[1] == 'Z'))) {
            string zipcodeStr = arg.substr(2);  // Extract the zipcode part
            int zipcode;

            try {
                zipcode = stoi(zipcodeStr);
                string result = searcher.searchForRecord(zipcode);

                if (result != "-1") {
                    cout << "Information for zipcode " << zipcode << ":\n";
                    searcher.displayRecord(result);
                } else {
                    cout << "Zipcode " << zipcode << " not found." << "\n\n";
                }
            } catch (const invalid_argument& ia) {
                cerr << "Invalid zipcode format: " << zipcodeStr << endl;
            }
        } else {
            // Invalid argument format
            cout << "Invalid argument: " << arg << endl;
            cout << "Please use the format: -z<zipcode> or -Z<zipcode>" << endl;
        }
    }

    return 0;
}
