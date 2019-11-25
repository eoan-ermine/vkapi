#pragma once

#include <vector>
#include <future>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "URI.h"

using eventObject = rapidjson::Document;

std::vector<future<void>> futureQueue;

class VKLongpoll {
private:
    std::mutex eventLock;
    std::deque<EventObject> events;
    
    Poco::Net::HTTPSClientSession session{("vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")};
    Poco::Net::HTTPRequest request{Poco::Net::HTTPMessage::HTTP_1_1};
    Poco::Net::HTTPResponse response;
    URI uri;
    
    std::string access_token;
public:
    VKLongpoll(std::string access_token) : access_token(std::move(access_token)) {
        request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
        
        uri.setURI("https://api.vk.com/method/");
        uri.setMethod("messages.getLongPollServer");
        
        uri.addQueryParameter("access_token", access_token);
        
        request.setURI(uri.toString());
        
        session.sendRequest(request);
        
        rapidjson::Document data;
        rapidjson::IStreamWrapper isw(session.receiveResponse(response));
        data.ParseStream(isw);
        
        std::string longpollUrl = "https://";
        longpollUrl.append(data["response"]["server"].GetString());
        longpollUrl.append("?");
        longpollUrl.append("act=a_check&key=");
        longpollUrl.append(data["response"]["key"].GetString());
        longpollUrl.append("&wait=20&mode=2&version=2&ts=");
        
        int ts = data['response']['ts'].GetInt();
        
        futureQueue.push_back(std::async([this, longpollUrl, ts]() {
            while(true) {
                EventObject event = this->requestEvent(uri, ts);
                if(!event.Empty()) {
                    this->eventLock.lock();
                    this->events.push_back(std::move(event));
                    this->eventLock.unlock();
                }
            }
        }, std::launch::async));       
    }
    
    EventObject getEvent() {
        EventObject event = Event{};
    
        eventLock.lock();
        
        if(!events.empty()) {
            event = events.back();
            events.pop_back();
        }
        
        eventLock.unlock();
        
        return event;     
    }
        
    EventObject requestEvent(const std::string& longpollUrl, int ts) {
        request.setURI(longpollUrl + std::to_string(ts));
        
        session.sendRequest(request);
        
        rapidjson::Document result;
        rapidjson::IStreamWrapper isw(session.receiveResponse(response));
        result.ParseStream(isw);
        
        return result;
    }
}
