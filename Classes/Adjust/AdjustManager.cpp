//
//  AdjustManager.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2019/08/07.
//

#include "AdjustManager.hpp"
#include "Adjust2dx.h"

static AdjustManager* gSharedInstance_ = nullptr;

USING_NS_CC;

AdjustManager::AdjustManager() {
    
}

AdjustManager::~AdjustManager() {
    CC_SAFE_DELETE(gSharedInstance_);
}

AdjustManager* AdjustManager::getInstance() {
    if (!gSharedInstance_) {
        gSharedInstance_ = new AdjustManager();
    }
    
    return gSharedInstance_;
}

void AdjustManager::init() {
    std::string appToken = "";
    std::string enviroment = AdjustEnvironmentSandbox2dx;
#ifdef RELEASE_BUILD
    enviroment = AdjustEnvironmentProduction2dx;
#endif
    
    AdjustConfig2dx adjustConfig = AdjustConfig2dx(appToken, enviroment);
    Adjust2dx::start(adjustConfig);
}

void AdjustManager::onPause() {
    Adjust2dx::onPause();
}

void AdjustManager::onResume() {
    Adjust2dx::onResume();
}

void AdjustManager::setTrackEvent(const std::string& eventToken, const std::string& transactionIndex) {
    AdjustEvent2dx adjustEvent = AdjustEvent2dx(eventToken);
    
    if (!transactionIndex.empty()) {
        adjustEvent.setTransactionId(transactionIndex);
    }
    
    Adjust2dx::trackEvent(adjustEvent);
}
