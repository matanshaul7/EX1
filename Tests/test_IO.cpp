#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <filesystem>

// Include headers for interfaces and implementations
#include "../src/interfaces/IReader.h"
#include "../src/io/readers/FileReader.h"
#include "../src/io/readers/ConsoleReader.h"
#include "../src/io/ConfigReaderFactory.h"
#include "../src/interfaces/IFileSystem.h"
#include "../src/io/filesystem/DefaultFileSystem.h"
#include "../src/services/FileStorageService.h"

// ============================================================
// Helper Classes for Testing
// ============================================================

// Helper to redirect stdin for testing console reader
class StdinRedirector {
private:
    std::streambuf* m_originalStdin;
    std::istringstream m_testStream;

public:
    explicit StdinRedirector(const std::string& input) : m_testStream(input) {
        m_originalStdin = std::cin.rdbuf();
        std::cin.rdbuf(m_testStream.rdbuf());
    }

    ~StdinRedirector() {
        std::cin.rdbuf(m_originalStdin);
    }
};

// Custom string-based reader implementation for testing
class StringReader : public IReader {
private:
    std::string m_content;
    bool m_read = false;
    bool m_valid;
    
public:
    StringReader(const std::string& content, bool valid = true) 
        : m_content(content), m_valid(valid) {}
        
    bool readLine(std::string& line) override {
        if (!m_valid || m_read) return false;
        line = m_content;
        m_read = true;
        return true;
    }
        
    bool isValid() const override { 
        return m_valid;
    }
};

// In-memory filesystem implementation for testing
class InMemoryFileSystem : public IFileSystem {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> m_textFiles;
    std::unordered_map<std::string, std::vector<char>> m_binaryFiles;
    
public:
    bool exists(const std::string& path) const override {
        return m_textFiles.find(path) != m_textFiles.end() || 
               m_binaryFiles.find(path) != m_binaryFiles.end();
    }
    
    bool readTextFile(const std::string& path, std::unordered_set<std::string>& lines) const override {
        auto it = m_textFiles.find(path);
        if (it == m_textFiles.end()) {
            return false;
        }
        lines = it->second;
        return true;
    }
    
    bool writeTextFile(const std::string& path, const std::unordered_set<std::string>& lines) const override {
        const_cast<InMemoryFileSystem*>(this)->m_textFiles[path] = lines;
        return true;
    }
    
    bool readBinaryFile(const std::string& path, std::vector<char>& data) const override {
        auto it = m_binaryFiles.find(path);
        if (it == m_binaryFiles.end()) {
            return false;
        }
        data = it->second;
        return true;
    }
    
    bool writeBinaryFile(const std::string& path, const std::vector<char>& data) const override {
        const_cast<InMemoryFileSystem*>(this)->m_binaryFiles[path] = data;
        return true;
    }
};

// Simple consumer class that demonstrates I/O abstraction usage
class IOConsumer {
private:
    std::shared_ptr<IReader> m_reader;
    std::shared_ptr<IFileSystem> m_filesystem;
    
public:
    IOConsumer(std::shared_ptr<IReader> reader, std::shared_ptr<IFileSystem> filesystem)
        : m_reader(reader), m_filesystem(filesystem) {}
        
    bool processInput(std::string& result) {
        // Uses the reader abstraction
        if (!m_reader || !m_reader->isValid() || !m_reader->readLine(result)) {
            return false;
        }
        
        // Uses the filesystem abstraction
        std::unordered_set<std::string> lines = {result};
        return m_filesystem->writeTextFile("output.txt", lines);
    }
};

// ============================================================
// Test Fixtures
// ============================================================

// Fixture for file-based reader tests
class FileReaderTest : public ::testing::Test {
protected:
    const std::string m_testFilePath = "test_file.txt";
    
    void SetUp() override {
        // Create a test file with sample content
        std::ofstream outFile(m_testFilePath);
        outFile << "First line" << std::endl;
        outFile << "Second line" << std::endl;
        outFile << "Third line" << std::endl;
        outFile.close();
    }
    
    void TearDown() override {
        // Clean up test file
        std::remove(m_testFilePath.c_str());
    }
};

// Fixture for filesystem tests
class FileSystemTest : public ::testing::Test {
protected:
    DefaultFileSystem m_fileSystem;
    const std::string m_textFilePath = "test_text_file.txt";
    const std::string m_binaryFilePath = "test_binary_file.bin";
    std::unordered_set<std::string> m_testLines = {"Line 1", "Line 2", "Line 3"};
    
    void SetUp() override {
        // Clean up any existing test files
        std::remove(m_textFilePath.c_str());
        std::remove(m_binaryFilePath.c_str());
    }
    
    void TearDown() override {
        // Clean up test files
        std::remove(m_textFilePath.c_str());
        std::remove(m_binaryFilePath.c_str());
    }
    
    void createTextFile(const std::string& path) {
        std::ofstream file(path);
        for (const auto& line : m_testLines) {
            file << line << std::endl;
        }
        file.close();
    }
};

// Fixture for storage service tests
class StorageServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<DefaultFileSystem> m_fileSystem;
    std::unique_ptr<FileStorageService> m_storageService;
    const std::string m_bitArrayPath = "test_bit_array.dat";
    const std::string m_blacklistPath = "test_blacklist.txt";
    
    void SetUp() override {
        // Clean up any existing test files
        std::remove(m_bitArrayPath.c_str());
        std::remove(m_blacklistPath.c_str());
        
        m_fileSystem = std::make_shared<DefaultFileSystem>();
        m_storageService = std::make_unique<FileStorageService>(
            m_bitArrayPath,
            m_blacklistPath,
            m_fileSystem
        );
    }
    
    void TearDown() override {
        // Clean up test files
        std::remove(m_bitArrayPath.c_str());
        std::remove(m_blacklistPath.c_str());
    }
};

// ============================================================
// Reader Interface Tests
// ============================================================

// Test that different readers implement the interface correctly
TEST(ReaderInterfaceTest, ImplementsPolymorphism) {
    // We can use different implementations with the same interface
    std::shared_ptr<IReader> stringReader = std::make_shared<StringReader>("Test content");
    std::string result;
    
    EXPECT_TRUE(stringReader->isValid());
    EXPECT_TRUE(stringReader->readLine(result));
    EXPECT_EQ("Test content", result);
    
    // Create invalid reader
    std::shared_ptr<IReader> invalidReader = std::make_shared<StringReader>("", false);
    EXPECT_FALSE(invalidReader->isValid());
    EXPECT_FALSE(invalidReader->readLine(result));
}

// Test FileReader with real files
TEST_F(FileReaderTest, ReadsLinesCorrectly) {
    // Test FileReader as the concrete implementation
    FileReader reader(m_testFilePath);
    EXPECT_TRUE(reader.isValid());
    
    std::string line;
    EXPECT_TRUE(reader.readLine(line));
    EXPECT_EQ("First line", line);
    
    EXPECT_TRUE(reader.readLine(line));
    EXPECT_EQ("Second line", line);
    
    EXPECT_TRUE(reader.readLine(line));
    EXPECT_EQ("Third line", line);
    
    // Should return false at end of file
    EXPECT_FALSE(reader.readLine(line));
    
    // Test with IReader interface to verify polymorphism
    std::unique_ptr<IReader> interfaceReader = std::make_unique<FileReader>(m_testFilePath);
    EXPECT_TRUE(interfaceReader->isValid());
}

// Test FileReader with non-existent file
TEST(FileReaderTest, HandlesNonExistentFile) {
    FileReader reader("non_existent_file.txt");
    EXPECT_FALSE(reader.isValid());
    
    std::string line;
    EXPECT_FALSE(reader.readLine(line));
}

// Test ConsoleReader with input redirection
TEST(ConsoleReaderTest, ReadsFromConsole) {
    // Set up redirected input
    StdinRedirector redirector("Console input test\nSecond line");
    
    ConsoleReader reader;
    EXPECT_TRUE(reader.isValid());
    
    std::string line;
    EXPECT_TRUE(reader.readLine(line));
    EXPECT_EQ("Console input test", line);
    
    EXPECT_TRUE(reader.readLine(line));
    EXPECT_EQ("Second line", line);
}

// Test ConfigReaderFactory with file argument
TEST_F(FileReaderTest, FactoryCreatesCorrectReader) {
    // Set up command line args with config file
    const char* argv[] = {"program", "127.0.0.1", "8080", m_testFilePath.c_str()};
    int argc = 4;
    
    auto reader = ConfigReaderFactory::createConfigReader(argc, const_cast<char**>(argv));
    ASSERT_NE(nullptr, reader);
    EXPECT_TRUE(reader->isValid());
    
    std::string line;
    EXPECT_TRUE(reader->readLine(line));
    EXPECT_EQ("First line", line);
    
    // Test without config file (should create ConsoleReader)
    const char* argvNoFile[] = {"program", "127.0.0.1", "8080"};
    argc = 3;
    
    StdinRedirector redirector("Console config input");
    auto consoleReader = ConfigReaderFactory::createConfigReader(argc, const_cast<char**>(argvNoFile));
    
    ASSERT_NE(nullptr, consoleReader);
    EXPECT_TRUE(consoleReader->isValid());
    
    EXPECT_TRUE(consoleReader->readLine(line));
    EXPECT_EQ("Console config input", line);
}

// ============================================================
// FileSystem Interface Tests
// ============================================================

// Test that different filesystem implementations work with the interface
TEST(FileSystemInterfaceTest, SupportsPolymorphism) {
    std::shared_ptr<IFileSystem> realFS = std::make_shared<DefaultFileSystem>();
    std::shared_ptr<IFileSystem> memoryFS = std::make_shared<InMemoryFileSystem>();
    
    // Both should implement the same functionality
    std::unordered_set<std::string> testLines = {"Test line 1", "Test line 2"};
    
    // Test with memory implementation
    EXPECT_TRUE(memoryFS->writeTextFile("test.txt", testLines));
    EXPECT_TRUE(memoryFS->exists("test.txt"));
    
    std::unordered_set<std::string> readLines;
    EXPECT_TRUE(memoryFS->readTextFile("test.txt", readLines));
    EXPECT_EQ(testLines, readLines);
    
    // This shows that interfaces work correctly and implementations can be substituted
}

// Test file existence check
TEST_F(FileSystemTest, ChecksFileExistence) {
    EXPECT_FALSE(m_fileSystem.exists(m_textFilePath));
    
    // Create file and check again
    createTextFile(m_textFilePath);
    EXPECT_TRUE(m_fileSystem.exists(m_textFilePath));
}

// Test text file reading and writing
TEST_F(FileSystemTest, HandlesTextFiles) {
    // Test writing
    EXPECT_TRUE(m_fileSystem.writeTextFile(m_textFilePath, m_testLines));
    EXPECT_TRUE(m_fileSystem.exists(m_textFilePath));
    
    // Test reading
    std::unordered_set<std::string> readLines;
    EXPECT_TRUE(m_fileSystem.readTextFile(m_textFilePath, readLines));
    
    EXPECT_EQ(m_testLines.size(), readLines.size());
    for (const auto& line : m_testLines) {
        EXPECT_TRUE(readLines.find(line) != readLines.end());
    }
    
    // Test reading non-existent file
    readLines.clear();
    EXPECT_FALSE(m_fileSystem.readTextFile("non_existent.txt", readLines));
    EXPECT_TRUE(readLines.empty());
}

// Test binary file operations
TEST_F(FileSystemTest, HandlesBinaryFiles) {
    // Test data to write
    std::vector<char> testData = {'H', 'e', 'l', 'l', 'o', 0, 'W', 'o', 'r', 'l', 'd'};
    
    // Write binary data
    EXPECT_TRUE(m_fileSystem.writeBinaryFile(m_binaryFilePath, testData));
    
    // Read it back
    std::vector<char> readData;
    EXPECT_TRUE(m_fileSystem.readBinaryFile(m_binaryFilePath, readData));
    
    // Compare data
    EXPECT_EQ(testData.size(), readData.size());
    for (size_t i = 0; i < testData.size(); i++) {
        EXPECT_EQ(testData[i], readData[i]);
    }
    
    // Test reading non-existent binary file
    readData.clear();
    EXPECT_FALSE(m_fileSystem.readBinaryFile("non_existent.bin", readData));
    EXPECT_TRUE(readData.empty());
}

// ============================================================
// Storage Service Tests
// ============================================================

// Test that FileStorageService works with DefaultFileSystem
TEST_F(StorageServiceTest, BlacklistOperations) {
    std::unordered_set<std::string> testUrls = {"example.com", "test.org"};
    
    // Save blacklist
    EXPECT_TRUE(m_storageService->saveBlacklist(testUrls));
    EXPECT_TRUE(m_fileSystem->exists(m_blacklistPath));
    
    // Check if URL is in blacklist
    EXPECT_TRUE(m_storageService->isInBlacklist("example.com"));
    EXPECT_FALSE(m_storageService->isInBlacklist("notinlist.com"));
    
    // Remove URL from blacklist
    EXPECT_TRUE(m_storageService->removeFromBlacklist("example.com"));
    EXPECT_FALSE(m_storageService->isInBlacklist("example.com"));
    EXPECT_TRUE(m_storageService->isInBlacklist("test.org"));
    
    // Load the blacklist directly
    std::unordered_set<std::string> loadedUrls;
    EXPECT_TRUE(m_storageService->loadBlacklist(loadedUrls));
    EXPECT_EQ(1, loadedUrls.size());
    EXPECT_TRUE(loadedUrls.find("test.org") != loadedUrls.end());
}

// Test bit array operations
TEST_F(StorageServiceTest, BitArrayOperations) {
    std::vector<bool> testBits = {true, false, true, true, false, true, false, true, false};
    
    // Save bit array
    EXPECT_TRUE(m_storageService->saveBitArray(testBits));
    EXPECT_TRUE(m_fileSystem->exists(m_bitArrayPath));
    
    // Load bit array
    std::vector<bool> loadedBits;
    EXPECT_TRUE(m_storageService->loadBitArray(loadedBits));
    
    // Verify the bits
    EXPECT_EQ(testBits.size(), loadedBits.size());
    for (size_t i = 0; i < testBits.size(); i++) {
        EXPECT_EQ(testBits[i], loadedBits[i]);
    }
}

// Test file existence
TEST_F(StorageServiceTest, FileExistenceChecks) {
    // Create empty file
    std::ofstream emptyFile("empty_file.txt");
    emptyFile.close();
    
    // Create non-empty file
    std::ofstream nonEmptyFile("non_empty_file.txt");
    nonEmptyFile << "Content" << std::endl;
    nonEmptyFile.close();
    
    // Test file existence checks
    EXPECT_FALSE(m_storageService->fileExistsAndNotEmpty("non_existent_file.txt"));
    EXPECT_FALSE(m_storageService->fileExistsAndNotEmpty("empty_file.txt"));
    EXPECT_TRUE(m_storageService->fileExistsAndNotEmpty("non_empty_file.txt"));
    
    // Clean up
    std::remove("empty_file.txt");
    std::remove("non_empty_file.txt");
}

// Test with in-memory filesystem to verify dependency injection
TEST(StorageServiceDITest, WorksWithCustomFileSystem) {
    auto inMemoryFS = std::make_shared<InMemoryFileSystem>();
    FileStorageService storageService("memory_bit_array.dat", "memory_blacklist.txt", inMemoryFS);
    
    // Test blacklist operations
    std::unordered_set<std::string> testUrls = {"example.com", "test.org"};
    EXPECT_TRUE(storageService.saveBlacklist(testUrls));
    
    // Verify data was written to the memory filesystem
    EXPECT_TRUE(inMemoryFS->exists("memory_blacklist.txt"));
    
    std::unordered_set<std::string> loadedUrls;
    EXPECT_TRUE(storageService.loadBlacklist(loadedUrls));
    EXPECT_EQ(testUrls, loadedUrls);
    
    // This demonstrates that FileStorageService correctly depends on the
    // IFileSystem abstraction and can work with any implementation
}

// ============================================================
// Integration Tests
// ============================================================

// Test that FileReader and DefaultFileSystem can be used together
TEST_F(FileReaderTest, ReaderAndFileSystemIntegration) {
    // Create a file system
    auto fileSystem = std::make_shared<DefaultFileSystem>();
    
    // Create a file reader for the test file
    auto fileReader = std::make_shared<FileReader>(m_testFilePath);
    EXPECT_TRUE(fileReader->isValid());
    
    // Create consumer that uses both abstractions
    IOConsumer consumer(fileReader, fileSystem);
    
    // Process input (reads from file and writes to another file)
    std::string result;
    EXPECT_TRUE(consumer.processInput(result));
    EXPECT_EQ("First line", result);
    
    // Verify output file was written correctly
    EXPECT_TRUE(fileSystem->exists("output.txt"));
    
    std::unordered_set<std::string> readLines;
    EXPECT_TRUE(fileSystem->readTextFile("output.txt", readLines));
    EXPECT_EQ(1, readLines.size());
    EXPECT_TRUE(readLines.find("First line") != readLines.end());
    
    // Clean up
    std::remove("output.txt");
}

// Test that we can swap implementations
TEST_F(FileReaderTest, SwappableImplementations) {
    auto fileSystem = std::make_shared<DefaultFileSystem>();
    
    // First use a file reader
    auto fileReader = std::make_shared<FileReader>(m_testFilePath);
    IOConsumer fileConsumer(fileReader, fileSystem);
    
    std::string fileResult;
    EXPECT_TRUE(fileConsumer.processInput(fileResult));
    EXPECT_EQ("First line", fileResult);
    
    // Clean up
    std::remove("output.txt");
    
    // Then swap to a string reader
    auto stringReader = std::make_shared<StringReader>("String input");
    IOConsumer stringConsumer(stringReader, fileSystem);
    
    std::string stringResult;
    EXPECT_TRUE(stringConsumer.processInput(stringResult));
    EXPECT_EQ("String input", stringResult);
    
    // Verify output
    std::unordered_set<std::string> readLines;
    EXPECT_TRUE(fileSystem->readTextFile("output.txt", readLines));
    EXPECT_TRUE(readLines.find("String input") != readLines.end());
    
    // Clean up
    std::remove("output.txt");
    
    // Now swap filesystem implementation
    auto memoryFS = std::make_shared<InMemoryFileSystem>();
    IOConsumer memoryConsumer(stringReader, memoryFS);
    
    // Create a new StringReader since the previous one has been consumed
    stringReader = std::make_shared<StringReader>("Memory filesystem test");
    memoryConsumer = IOConsumer(stringReader, memoryFS);
    
    EXPECT_TRUE(memoryConsumer.processInput(stringResult));
    EXPECT_EQ("Memory filesystem test", stringResult);
    
    // Verify data is in memory filesystem but not in real filesystem
    EXPECT_TRUE(memoryFS->exists("output.txt"));
    EXPECT_FALSE(fileSystem->exists("output.txt"));
    
    std::unordered_set<std::string> memoryLines;
    EXPECT_TRUE(memoryFS->readTextFile("output.txt", memoryLines));
    EXPECT_TRUE(memoryLines.find("Memory filesystem test") != memoryLines.end());
}

// Test initializing bloom filter from files
TEST_F(StorageServiceTest, InitializesFilterFromStorage) {
    // Create test data
    std::vector<bool> testBits = {true, false, true, true, false};
    std::unordered_set<std::string> testUrls = {"example.com", "test.org"};
    
    // Save data
    EXPECT_TRUE(m_storageService->saveBitArray(testBits));
    EXPECT_TRUE(m_storageService->saveBlacklist(testUrls));
    
    // Create new vectors to initialize
    std::vector<bool> loadedBits;
    std::unordered_set<std::string> loadedUrls;
    
    // Initialize from storage
    EXPECT_TRUE(m_storageService->initializeFilter(loadedBits, loadedUrls));
    
    // Verify data
    EXPECT_EQ(testBits.size(), loadedBits.size());
    for (size_t i = 0; i < testBits.size(); i++) {
        EXPECT_EQ(testBits[i], loadedBits[i]);
    }
    
    EXPECT_EQ(testUrls.size(), loadedUrls.size());
    for (const auto& url : testUrls) {
        EXPECT_TRUE(loadedUrls.find(url) != loadedUrls.end());
    }
}

// ============================================================
// Main function
// ============================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}