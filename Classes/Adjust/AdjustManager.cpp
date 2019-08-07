//
//  AdjustManager.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2019/08/07.
//

#include "AdjustManager.hpp"

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
