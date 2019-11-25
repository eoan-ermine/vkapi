#pragma once

#include <iostream>
#include <vector>
#include <future>
#include <deque>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "URI.h"

using EventObject = rapidjson::Document;

std::vector<std::future<void>> futureQueue;

class VKLongpoll {
private:
    std::mutex eventLock;
    std::deque<EventObject> events;
    
    Poco::Net::HTTPSClientSession session{"vk.com", 443, new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")};
    Poco::Net::HTTPRequest request{Poco::Net::HTTPMessage::HTTP_1_1};
    Poco::Net::HTTPResponse response;
    URI uri;
    
    std::string access_token;
public:
    VKLongpoll(std::string access_token) : access_token(std::move(access_token)) {
        request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
        
        uri.setURI("https://api.vk.com/method/");
        uri.setMethod("messages.getLongPollServer");
        
        uri.addQueryParameter("access_token", this->access_token);
        uri.addQueryParameter("v", "5.101");
        
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
        
        int ts = data["response"]["ts"].GetInt();
        
        std::cout << "Final" << "\n";
        
        futureQueue.push_back(std::async(std::launch::async, [this, longpollUrl, ts]() {
            while(true) {
                std::cout << "Searching events..." << "\n";
                EventObject event = this->requestEvent(longpollUrl, ts);
                std::cout << "We got event!\n";
                
                if(!event.Empty()) {
                    std::cout << "Event was found!" << "\n";
                    this->eventLock.lock();
                    this->events.push_back(std::move(event));
                    this->eventLock.unlock();
                }
                
                ts = event["ts"].GetInt();
            }
        }));       
    }
    
    EventObject getEvent() {
        EventObject event = EventObject{};
    
        eventLock.lock();
        
        if(!events.empty()) {
            event = std::move(events.back());
            this->events.pop_back();
        }
        
        eventLock.unlock();
        
        return event;     
    }
        
    EventObject requestEvent(const std::string& longpollUrl, int ts) {
        std::cout << "In requestEvent\n";
    
        request.setURI(longpollUrl + std::to_string(ts));
        
        std::cout << "Do request\n";
        
        session.sendRequest(request);
        
        std::cout << "Request event...\n";
        
        rapidjson::Document result;
        rapidjson::IStreamWrapper isw(session.receiveResponse(response));
        result.ParseStream(isw);
        
        return result;
    }
};