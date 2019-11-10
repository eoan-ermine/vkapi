#pragma once

#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
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
    Poco::JSON::Parser parser;
    
    const std::string access_token;
    const std::string version;
public:
    VkApi(std::string token, std::string version_);
    Poco::JSON::Object::Ptr executeMethod(const std::string&,
                                          const Poco::URI::QueryParameters& params={});
    const std::string& getVersion() const;
    const std::string& getToken() const;
};
