#include "FileReader.h"

FileReader::FileReader(const std::string& filename) {
    m_file.open(filename);
}

FileReader::~FileReader() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

bool FileReader::readLine(std::string& line) {
    return static_cast<bool>(std::getline(m_file, line));
}

bool FileReader::isValid() const {
    return m_file.is_open() && m_file.good();
}