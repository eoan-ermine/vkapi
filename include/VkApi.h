#pragma once

#include "nlohmann/json.hpp"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "URI.h"

const std::string CLIENT_ID = "3697615";
const std::string CLIENT_SECRET = "AlVXZFMUqyrnABp8ncuU";
const std::string DEFAULT_VERSION = "5.101";

class VkApi {
private:
    Poco::Net::HTTPSClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    URI uri;
   
    std::string login, password, access_token, version;
public:
    VkApi() : VkApi("", "", "", DEFAULT_VERSION) { }
    
    VkApi(const std::string& token) : VkApi("", "", token, DEFAULT_VERSION) { }
    
    VkApi(const std::string& login, const std::string& password) : VkApi(login, password, "", DEFAULT_VERSION) { }
    
    VkApi(const std::string& login, const std::string& password, const std::string& token, const std::string& version) : session("vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")), request(Poco::Net::HTTPMessage::HTTP_1_1), login(login), password(password), access_token(token), version(version) {
        request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
    }

    nlohmann::json executeMethod(const std::string& method, const std::initializer_list<std::pair<std::string, std::string>>& params={}) {      
        uri.setURI("https://api.vk.com/method/");
        uri.setMethod(method);
        
        if(params.size()) {
            uri.setQueryParameters(params);
        }
        uri.addQueryParameter("access_token", access_token);
        uri.addQueryParameter("v", version);
        request.setURI(uri.toString());
        
        session.sendRequest(request);
        
        nlohmann::json result;
        session.receiveResponse(response) >> result;
        
        return result;
    }
                           
    bool auth() {
        uri.setURI("https://oauth.vk.com/");
        uri.setMethod("token");
        
        uri.setQueryParameters({{"grant_type", "password"}, {"client_id", CLIENT_ID}, {"client_secret", CLIENT_SECRET}, {"username", login}, {"password", password}});
        request.setURI(uri.toString());
            
        session.sendRequest(request);
            
        nlohmann::json result;
        session.receiveResponse(response) >> result;
            
        if(result.count("access_token")) {
            this->access_token = result["access_token"].get<std::string>();
            return true;
        }
        
        return false;
    }
    
    bool reAuth(const std::string login, const std::string password) {
        this->login = std::move(login);
        this->password = std::move(password);
        
        return auth();   
    }

    const std::string& getVersion() const {
        return version;
    }
    
    const std::string& getToken() const {
        return access_token;
    }
    
    void setToken(const std::string token) {
        access_token = std::move(token);
    }
    
    void setVersion(const std::string version) {
        this->version = std::move(version);
    }
};
