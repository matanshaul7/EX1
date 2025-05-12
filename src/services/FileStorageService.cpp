#include "FileStorageService.h"
#include "../io/filesystem/DefaultFileSystem.h"
#include <fstream>

FileStorageService::FileStorageService(
    const std::string& bitArrayPath,
    const std::string& blacklistPath,
    std::shared_ptr<IFileSystem> fileSystem)
    : m_bitArrayFile(bitArrayPath), 
      m_blacklistFile(blacklistPath),
      m_fileSystem(fileSystem ? fileSystem : std::make_shared<DefaultFileSystem>()) {}

bool FileStorageService::saveBlacklist(const std::unordered_set<std::string>& blacklist) {
    return m_fileSystem->writeTextFile(m_blacklistFile, blacklist);
}

bool FileStorageService::loadBlacklist(std::unordered_set<std::string>& blacklist) {
    if (!m_fileSystem->exists(m_blacklistFile)) {
        return false;
    }
    return m_fileSystem->readTextFile(m_blacklistFile, blacklist);
}

bool FileStorageService::saveBitArray(const std::vector<bool>& bitArray) {
    // Prepare data for binary storage
    std::vector<char> data;
    
    // Store the size first
    size_t size = bitArray.size();
    data.resize(sizeof(size_t));
    std::memcpy(data.data(), &size, sizeof(size_t));
    
    // Pack bits into bytes
    size_t byteCount = (size + 7) / 8;
    data.resize(sizeof(size_t) + byteCount);
    
    for (size_t i = 0; i < size; i += 8) {
        unsigned char byte = 0;
        for (size_t j = 0; j < 8 && (i + j) < size; ++j) {
            if (bitArray[i + j]) {
                byte |= (1 << j);
            }
        }
        data[sizeof(size_t) + i/8] = byte;
    }
    
    return m_fileSystem->writeBinaryFile(m_bitArrayFile, data);
}

bool FileStorageService::loadBitArray(std::vector<bool>& bitArray) {
    if (!m_fileSystem->exists(m_bitArrayFile)) {
        return false;
    }
    
    std::vector<char> data;
    if (!m_fileSystem->readBinaryFile(m_bitArrayFile, data) || data.size() < sizeof(size_t)) {
        return false;
    }
    
    // Read the size
    size_t size;
    std::memcpy(&size, data.data(), sizeof(size_t));
    bitArray.resize(size);
    
    // Unpack bytes into bits
    for (size_t i = 0; i < size; i += 8) {
        if (sizeof(size_t) + i/8 < data.size()) {
            unsigned char byte = data[sizeof(size_t) + i/8];
            for (size_t j = 0; j < 8 && (i + j) < size; ++j) {
                bitArray[i + j] = (byte & (1 << j)) != 0;
            }
        }
    }
    
    return true;
}

bool FileStorageService::removeFromBlacklist(const std::string& url) {
    std::unordered_set<std::string> blacklist;
    if (!loadBlacklist(blacklist)) {
        return false;
    }
    
    auto it = blacklist.find(url);
    if (it == blacklist.end()) {
        return false; // URL not found
    }
    
    blacklist.erase(it);
    return saveBlacklist(blacklist);
}

bool FileStorageService::isInBlacklist(const std::string& url) {
    std::unordered_set<std::string> blacklist;
    if (!loadBlacklist(blacklist)) {
        return false;
    }
    
    return blacklist.find(url) != blacklist.end();
}

bool FileStorageService::fileExistsAndNotEmpty(const std::string& filename) {
    if (!m_fileSystem->exists(filename)) {
        return false;
    }
    
    std::unordered_set<std::string> lines;
    return m_fileSystem->readTextFile(filename, lines) && !lines.empty();
}

bool FileStorageService::initializeFilter(std::vector<bool>& bitArray, std::unordered_set<std::string>& blacklist) {
    bool bitArrayLoaded = loadBitArray(bitArray);
    bool blacklistLoaded = loadBlacklist(blacklist);
    
    return bitArrayLoaded || blacklistLoaded;
}