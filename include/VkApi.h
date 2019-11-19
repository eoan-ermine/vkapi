#pragma once

#include "nlohmann/json.hpp"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"

#include <string_view>

class VkApi {
private:
    Poco::Net::HTTPSClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    Poco::URI uri;
    
    const std::string access_token;
    const std::string version;
    
    std::string longUrl;
public:
    VkApi(std::string token, std::string version_);
    nlohmann::json executeMethod(const std::string&,
                                          const Poco::URI::QueryParameters& params={});
    const std::string& getVersion() const;
    const std::string& getToken() const;
    
    //  I will implement work with VK LongPoll later
    
    //void initializeLongPoll(const std::string& server_, const std::string& key_, const std::string& wait_, const std::string& mode_, const std::string& version_);
    //nlohmann::json getLongPoll(const std::string& ts);
};
