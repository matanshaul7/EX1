#include <iostream>
#include <memory>
#include <string>
#include "src/services/ApplicationConfig.h"
#include "src/networking/TCPSocketListener.h"
#include "src/networking/Server.h"
#include "src/io/ConfigReaderFactory.h"
#include "src/interfaces/IReader.h"

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " <ip_address> <port> [config_file]" << std::endl;
            std::cerr << "If config_file is not provided, configuration is read from stdin" << std::endl;
            return 1;
        }

        // Parse port number
        std::string ipAddress = argv[1];
        int port = std::stoi(argv[2]);
        
        // Read initial configuration using our abstraction
        auto configReader = ConfigReaderFactory::createConfigReader(argc, argv);
        
        if (!configReader || !configReader->isValid()) {
            std::cerr << "Failed to create config reader" << std::endl;
            return 1;
        }
        
        std::string configLine;
        if (!configReader->readLine(configLine)) {
            std::cerr << "Failed to read configuration" << std::endl;
            return 1;
        }
        
        // Configure application
        auto appService = ApplicationConfig::configure(configLine);
        if (!appService) {
            std::cerr << "Failed to configure application" << std::endl;
            return 1;
        }
        
        // Create and start server
        std::shared_ptr<ISocketListener> listener = std::make_shared<TCPSocketListener>();
        Server server(listener, appService);
        
        if (!server.start(port, ipAddress)) {
            return 1;
        }
        
        // Run server (blocks until server is stopped)
        server.run();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}