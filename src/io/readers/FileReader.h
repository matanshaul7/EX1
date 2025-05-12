#ifndef FILEREADER_H
#define FILEREADER_H
#include "../interfaces/IReader.h"
#include <fstream>
#include <string>

class FileReader : public IReader {
private:
    std::ifstream m_file;
    
public:
    explicit FileReader(const std::string& filename);
    ~FileReader() override;
    
    bool readLine(std::string& line) override;
    bool isValid() const override;
};
#endif // FILEREADER_H