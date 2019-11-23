#pragma once

#include <string>
#include <iostream>

class URI {
public:
    void setURI(const std::string& url) {    
        uri = url;
    }
    
    void setMethod(const std::string method) {
        uri.append(method);
        uri.push_back('?');
    }
    
    void setQueryParameters(const std::initializer_list<std::pair<std::string, std::string>>& params) {
        for(const std::pair<std::string, std::string>& element: params) {
            uri.append(element.first);
            uri.push_back('=');
            uri.append(element.second);
            uri.push_back('&');
        }
    }
    
    void addQueryParameter(const std::string& key, const std::string& value) {
        uri.append(key);
        uri.push_back('=');
        uri.append(value);
        uri.push_back('&');
    }
    
    const std::string& toString() const {
        return uri;
    }
private:
    std::string uri;
};