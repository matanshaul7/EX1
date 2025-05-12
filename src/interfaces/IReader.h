#ifndef IREADER_H
#define IREADER_H
#include <string>

class IReader {
public:
    virtual ~IReader() = default;
    virtual bool readLine(std::string& line) = 0;
    virtual bool isValid() const = 0;
};
#endif // IREADER_H