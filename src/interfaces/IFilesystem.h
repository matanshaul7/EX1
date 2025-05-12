#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H
#include <string>
#include <vector>
#include <unordered_set>

class IFileSystem {
public:
    virtual ~IFileSystem() = default;
    
    virtual bool exists(const std::string& path) const = 0;
    virtual bool readTextFile(const std::string& path, std::unordered_set<std::string>& lines) const = 0;
    virtual bool writeTextFile(const std::string& path, const std::unordered_set<std::string>& lines) const = 0;
    virtual bool readBinaryFile(const std::string& path, std::vector<char>& data) const = 0;
    virtual bool writeBinaryFile(const std::string& path, const std::vector<char>& data) const = 0;
};
#endif // IFILESYSTEM_H