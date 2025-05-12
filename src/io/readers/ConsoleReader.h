#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H
#include "../interfaces/IReader.h"
#include <string>

class ConsoleReader : public IReader {
public:
    ConsoleReader() = default;
    ~ConsoleReader() override = default;
    
    bool readLine(std::string& line) override;
    bool isValid() const override;
};
#endif // CONSOLEREADER_H