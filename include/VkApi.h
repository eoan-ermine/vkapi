#pragma once

#include "nlohmann/json.hpp"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"

#include <string>

const std::string CLIENT_ID = "3697615";
const std::string CLIENT_SECRET = "AlVXZFMUqyrnABp8ncuU";
const std::string DEFAULT_VERSION = "5.101";

class VkApi {
private:
    Poco::Net::HTTPSClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    Poco::URI uri;
    
    std::string login;
    std::string password;
    std::string access_token;
    std::string version;
public:
    VkApi();
    VkApi(std::string token);
    VkApi(std::string login, std::string password);
    VkApi(std::string login, std::string password, std::string token, std::string version);
    bool auth();
    nlohmann::json executeMethod(const std::string&, const Poco::URI::QueryParameters& params={});
    
    const std::string& getVersion() const {
        return version;
    }
    
    const std::string& getToken() const {
        return access_token;
    }
    
    template <class T>
    void setToken(T&& token) {
        access_token = std::forward<T>(token);
    }
    
    template <class T>
    void setVersion(T& version) {
        this->version = std::forward<T>(version);
    }
};
