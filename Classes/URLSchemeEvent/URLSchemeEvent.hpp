//
//  URLSchemeEvent.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2019/07/02.
//

#ifndef URLSchemeEvent_hpp
#define URLSchemeEvent_hpp

#include "cocos2d.h"

class URLSchemeEvent {
private:
    std::string schemeURL_{""};

private:
    URLSchemeEvent();
    ~URLSchemeEvent();
    
public:
    static URLSchemeEvent* getInstance();
    
    void setSchemeURL(const std::string& url);
    std::string getSchemeURL();
};

#endif /* URLSchemeEvent_hpp */
