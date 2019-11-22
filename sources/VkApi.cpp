#include "../include/VkApi.h"
#include <iostream>

VkApi::VkApi(): VkApi("", "", "", DEFAULT_VERSION) { }

VkApi::VkApi(std::string token) : VkApi("", "", std::move(token), DEFAULT_VERSION) { }

VkApi::VkApi(std::string login, std::string password) : VkApi(std::move(login), std::move(password), "", DEFAULT_VERSION) { }

VkApi::VkApi(std::string login, std::string password, std::string token, std::string version)
    : session("vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "",
                                        "", Poco::Net::Context::VERIFY_NONE, 9,
                                        false,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")), request(Poco::Net::HTTPMessage::HTTP_1_1), login(std::move(login)), password(std::move(password)), access_token(std::move(token)), version(std::move(version)) {
    request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
}

bool VkApi::auth() {
    if(!login.empty() && !password.empty() && access_token.empty()) {
        uri = "https://oauth.vk.com/token";
        uri.setQueryParameters({{"grant_type", "password"}, {"client_id", CLIENT_ID}, {"client_secret", CLIENT_SECRET}, {"username", login}, {"password", password}});
        request.setURI(uri.toString());
        
        session.sendRequest(request);
        
        nlohmann::json result;
        session.receiveResponse(response) >> result;
        
        if(result.count("access_token")) {
            this->access_token = result["access_token"].get<std::string>();
            return true;
        }
    }
    return false;   
}

nlohmann::json VkApi::executeMethod(const std::string& method, const Poco::URI::QueryParameters& params) {
    uri = "https://api.vk.com/method/" + method;
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