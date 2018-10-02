//
//  GameDataManager.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/01.
//

#include "GameDataManager.hpp"

static GameDataManager* gSaredInstance_ = nullptr;

GameDataManager::GameDataManager() {
    
}

GameDataManager::~GameDataManager() {
    CC_SAFE_DELETE(gSaredInstance_);
}

GameDataManager* GameDataManager::getInstance() {
    if (!gSaredInstance_) {
        gSaredInstance_ = new (std::nothrow) GameDataManager();
    }
    
    return gSaredInstance_;
}
