#include "../include/VkApi.h"
#include <iostream>

VkApi::VkApi(const std::string token, const std::string version_)
    : session("api.vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "",
                                        "", Poco::Net::Context::VERIFY_NONE, 9,
                                        false,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")), request(Poco::Net::HTTPMessage::HTTP_1_1), access_token(std::move(token)), version(std::move(version_)), longUrl("https://") {
    request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
}

rapidjson::Document VkApi::executeMethod(
    const std::string& method, const Poco::URI::QueryParameters& params) {
    uri = "https://api.vk.com/method/" + method;
    if(params.size()) {
        uri.setQueryParameters(params);
    }
    uri.addQueryParameter("access_token", access_token);
    uri.addQueryParameter("v", version);
    request.setURI(uri.toString());
    
    session.sendRequest(request);
    rapidjson::IStreamWrapper isw(session.receiveResponse(response));
    rapidjson::Document returnDocument;
    returnDocument.ParseStream(isw);
    
    return returnDocument;
}

void VkApi::initializeLongPoll(const std::string& server_, const std::string& key_, const std::string& wait_, const std::string& mode_, const std::string& version_) {
    longUrl += server_;
    longUrl += "?act=a_check&key=";
    longUrl += key_;
    longUrl += "&wait=";
    longUrl += wait_;
    longUrl += "&mode=";
    longUrl += mode_;
    longUrl += "&version=";
    longUrl += version_;
}

rapidjson::Document VkApi::getLongPoll(const std::string& ts) {
    request.setURI(longUrl + "&ts=" + ts);
    session.sendRequest(request);
    rapidjson::IStreamWrapper isw(session.receiveResponse(response));
    rapidjson::Document returnDocument;
    returnDocument.ParseStream(isw);
    return returnDocument;
}

const std::string& VkApi::getVersion() const {
    return version;
}

const std::string& VkApi::getToken() const {
    return access_token;
}
