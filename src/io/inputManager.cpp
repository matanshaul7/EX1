<<<<<<< HEAD:src/io/inputManager.cpp
#include "../io/inputManager.h"
#include "../bloom_Filter/bloomFilter.h"
=======
#include "../ioHandling/inputManager.h"
#include "../bloom_filter/bloomFilter.h"
>>>>>>> 329ab168deb56388c154863e20e97f5fcc5ff92b:src/ioHandling/inputManager.cpp
#include "../services/FileStorageService.h"
#include "../bloom_filter/hashFactory.h"

#include <sstream>



    

bool InputManager::splitRequest(string& command, string& url) {
    // Validate command format
    if (command.empty()) {
        return false;
    }
    istringstream iss(command);
    string commandType;
    iss >> commandType;
    
    string url;
    getline(iss >> ws, url);

    URLValidator urlValidator;
    string standardURL = urlValidator.standardize(url);
    if (standardURL.empty()) {
        return false;
    }
    url = standardURL;
    command = commandType;
    return true;
}
    

