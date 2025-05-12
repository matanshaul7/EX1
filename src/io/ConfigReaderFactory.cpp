#include "ConfigReaderFactory.h"
#include "FileReader.h"
#include "ConsoleReader.h"

std::shared_ptr<IReader> ConfigReaderFactory::createConfigReader(int argc, char* argv[]) {
    if (argc >= 4) {
        // Use file reader for config file
        return std::make_shared<FileReader>(argv[3]);
    } else {
        // Use console reader
        return std::make_shared<ConsoleReader>();
    }
}

std::shared_ptr<IReader> ConfigReaderFactory::createReader(const std::string& filename) {
    if (!filename.empty()) {
        return std::make_shared<FileReader>(filename);
    } else {
        return std::make_shared<ConsoleReader>();
    }
}