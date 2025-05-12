#ifndef FILESTORAGESERVICE_H
#define FILESTORAGESERVICE_H

#include "../interfaces/IStorageService.h"
#include "../interfaces/IFileSystem.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

class FileStorageService : public IStorageService {
private:
    std::string m_bitArrayFile;
    std::string m_blacklistFile;
    std::shared_ptr<IFileSystem> m_fileSystem;

public:
    FileStorageService(
        const std::string& bitArrayPath = "data/bit_array.dat",
        const std::string& blacklistPath = "data/blacklist.txt",
        std::shared_ptr<IFileSystem> fileSystem = nullptr);
        
    bool saveBlacklist(const std::unordered_set<std::string>& blacklist) override;
    bool loadBlacklist(std::unordered_set<std::string>& blacklist) override;
    bool saveBitArray(const std::vector<bool>& bitArray) override;
    bool loadBitArray(std::vector<bool>& bitArray) override;
    bool removeFromBlacklist(const std::string& url) override;
    bool isInBlacklist(const std::string& url) override;
    bool fileExistsAndNotEmpty(const std::string& filename) override;
    bool initializeFilter(std::vector<bool>& bitArray, std::unordered_set<std::string>& blacklist) override;
};

#endif // FILESTORAGESERVICE_H