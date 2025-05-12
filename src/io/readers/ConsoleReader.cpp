#include "ConsoleReader.h"
#include <iostream>
bool ConsoleReader::readLine(std::string& line) {
    return static_cast<bool>(std::getline(std::cin, line));
}

bool ConsoleReader::isValid() const {
    return std::cin.good();
}