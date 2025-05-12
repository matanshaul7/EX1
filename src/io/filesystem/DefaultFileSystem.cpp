#include "DefaultFileSystem.h"
#include <fstream>
#include <filesystem>

bool DefaultFileSystem::exists(const std::string& path) const {
    return std::filesystem::exists(path);
}

bool DefaultFileSystem::readTextFile(const std::string& path, std::unordered_set<std::string>& lines) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.insert(line);
        }
    }
    
    return true;
}

bool DefaultFileSystem::writeTextFile(const std::string& path, const std::unordered_set<std::string>& lines) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& line : lines) {
        file << line << std::endl;
    }
    
    return file.good();
}

bool DefaultFileSystem::readBinaryFile(const std::string& path, std::vector<char>& data) const {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read the data
    data.resize(size);
    if (size > 0) {
        file.read(data.data(), size);
    }
    
    return file.good();
}

bool DefaultFileSystem::writeBinaryFile(const std::string& path, const std::vector<char>& data) const {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    if (!data.empty()) {
        file.write(data.data(), data.size());
    }
    
    return file.good();
}