#include "../include/VkApi.h"

VkApi::VkApi(const std::string token, const std::string version_)
    : session("api.vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "",
                                        "", Poco::Net::Context::VERIFY_NONE, 9,
                                        false,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")), request(Poco::Net::HTTPMessage::HTTP_1_1), access_token(std::move(token)), version(std::move(version_)) {
    request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
}

Poco::JSON::Object::Ptr VkApi::executeMethod(
    const std::string& method, const Poco::URI::QueryParameters& params) {
    uri = "https://api.vk.com/method/" + method;
    if(params.size()) {
        uri.setQueryParameters(params);
    }
    uri.addQueryParameter("access_token", access_token);
    uri.addQueryParameter("v", version);
    request.setURI(uri.toString());
    session.sendRequest(request);
    return parser.parse(session.receiveResponse(response))
        .extract<Poco::JSON::Object::Ptr>();
}

const std::string& VkApi::getVersion() const {
    return version;
}

const std::string& VkApi::getToken() const {
    return token;
}