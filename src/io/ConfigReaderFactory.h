#ifndef CONFIGREADERFACTORY_H
#define CONFIGREADERFACTORY_H
#include "../interfaces/IReader.h"
#include <memory>
#include <string>

class ConfigReaderFactory {
public:
    // Create a configuration reader based on command line args
    static std::shared_ptr<IReader> createConfigReader(int argc, char* argv[]);
    
    // Create a file reader if filename is provided, otherwise console reader
    static std::shared_ptr<IReader> createReader(const std::string& filename = "");
};
#endif // CONFIGREADERFACTORY_H