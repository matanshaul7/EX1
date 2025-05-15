#ifndef FILESTORAGESERVICE_H
#define FILESTORAGESERVICE_H

#include "../interfaces/IStorageService.h"
#include "../interfaces/IFileSystem.h"
#include <string>
#include <vector>
<<<<<<< HEAD
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
=======
using namespace std;
/**
 * @class FileStorageService
 * @brief A class that implements the IStorageService interface for file-based storage.
 *
 * This class provides methods to save and load bit arrays and blacklists from files.
 */
class FileStorageService : public IStorageService {
private:
    string m_bitArrayFile;
    string m_blacklistFile;
    string m_configFile;

public:
    FileStorageService(const string& bitArrayPath = "data/bit_array.dat",
                       const string& blacklistPath = "data/blacklist.txt")
        : m_bitArrayFile(bitArrayPath), m_blacklistFile(blacklistPath){}
    bool saveBlacklist(const unordered_set<string>& blacklist) override;
    bool loadBlacklist(unordered_set<string>& blacklist) override;
    bool saveBitArray(const vector<bool>& bitArray) override;
    bool loadBitArray(vector<bool>& bitArray) override;
    bool removeFromBlacklist(const string& url) override;
    bool isInBlacklist(const string& url) override;
    bool fileExistsAndNotEmpty(const string& filename) override;
    bool initializeFilter(vector<bool>& bitArray, unordered_set<string>& blacklist);

>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b
};

#endif // FILESTORAGESERVICE_H