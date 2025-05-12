#ifndef DEFAULTFILESYSTEM_H
#define DEFAULTFILESYSTEM_H
#include "../interfaces/IFileSystem.h"

class DefaultFileSystem : public IFileSystem {
public:
    bool exists(const std::string& path) const override;
    bool readTextFile(const std::string& path, std::unordered_set<std::string>& lines) const override;
    bool writeTextFile(const std::string& path, const std::unordered_set<std::string>& lines) const override;
    bool readBinaryFile(const std::string& path, std::vector<char>& data) const override;
    bool writeBinaryFile(const std::string& path, const std::vector<char>& data) const override;
};
#endif // DEFAULTFILESYSTEM_H