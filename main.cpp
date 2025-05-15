#include <iostream>
#include <memory>
#include <string>
#include "src/services/ApplicationConfig.h"
#include "src/networking/TCPSocketListener.h"
#include "src/networking/Server.h"
<<<<<<< HEAD
#include "src/io/ConfigReaderFactory.h"
#include "src/interfaces/IReader.h"
=======
#include "src/services/FileStorageService.h"

using namespace std;
>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments
        if (argc < 3) {
<<<<<<< HEAD
            std::cerr << "Usage: " << argv[0] << " <ip_address> <port> [config_file]" << std::endl;
            std::cerr << "If config_file is not provided, configuration is read from stdin" << std::endl;
            return 1;
=======
            return -1;
>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b
        }
        
<<<<<<< HEAD
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
=======
        // Parse input number
        string ipAddress = argv[1];
        int port = stoi(argv[2]);
        
        string configLine;
        // try to read configuration if added to command line
        if (argc >3) {
            for (int i = 3; i < argc; i++) {
                if (i >3) {
                    configLine += " ";
                }
                configLine += argv[i];
            }
        }else {
            return -1;
>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b
        }
        
        // Configure application
        auto appService = ApplicationConfig::configure(configLine);
        if (!appService) {
            return -1;
        }
        // Create and start server
        shared_ptr<ISocketListener> listener = make_shared<TCPSocketListener>();
        Server server(listener, appService);
        
        if (!server.start(port, ipAddress)) {
            return -1;
        }
        
        // Run server (blocks until server is stopped)
        server.run();
        return 0;
    }
<<<<<<< HEAD
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
=======
    catch (const exception& e) {
        return -1;
>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b
    }
}
