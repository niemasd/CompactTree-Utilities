// imports
#include <iostream>
#include <string>

// global constants and definitions
#define CT_UTILS_VERSION "0.0.1"
#define ERROR_OPEN_FILE "Unable to open file"

// helpful functions
bool check_open(std::istream & input, const std::string & fn = "", bool throw_error = true) {
    if(!input.good()) {
        if(throw_error) {
            std::string message = ERROR_OPEN_FILE; if(fn != "") { message += (": " + fn); }
            throw std::invalid_argument(message);
        } else { return false; }
    } return true;
}
