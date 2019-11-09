#include "VKApi.h"

VKApi::VKApi(): m_Request(Poco::Net::HTTPMessage::HTTP_1_1) {
    m_pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    m_pSession = new Poco::Net::HTTPSClientSession(m_pContext);
    m_pSession->setHost("api.vk.com");
    m_pSession->setPort(443);
    m_Request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
}

Poco::JSON::Object::Ptr VKApi::executeMethod(const std::string& method, const Poco::URI::QueryParameters& params) {
    m_Uri = "https://api.vk.com/method/" + method;
    m_Uri.setQueryParameters(params);
    m_Request.setURI(m_Uri.toString());
    m_pSession->sendRequest(m_Request);
    return m_Parser.parse(m_pSession->receiveResponse(m_Response)).extract<Poco::JSON::Object::Ptr>();
}