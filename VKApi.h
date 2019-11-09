#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"

class VKApi {
private:
    Poco::Net::HTTPSClientSession* m_pSession;
    Poco::Net::HTTPRequest m_Request;
    Poco::Net::HTTPResponse m_Response;
    Poco::URI m_Uri;
    Poco::Net::Context::Ptr m_pContext;
    Poco::JSON::Parser m_Parser;
public:
    VKApi();
    Poco::JSON::Object::Ptr executeMethod(const std::string&, const Poco::URI::QueryParameters&);
};
   