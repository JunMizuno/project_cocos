//
//  FirstScene.cpp
//  project_cocos-mobile
//
//  Created by Jun Mizuno on 2018/07/06.
//

#include "FirstScene.hpp"

USING_NS_CC;

namespace project {
    namespace {
        
    }
    
    /**
     *  @brief 生成
     */
    FirstScene* FirstScene::create() {
        FirstScene* pRet = new(std::nothrow) FirstScene();
        
        if (pRet) {
            if (pRet->init()) {
                pRet->autorelease();
            }
        }
        else {
            delete pRet;
            pRet = nullptr;
        }
        
        return pRet;
    }
    
    /**
     *  @brief シーンを生成
     */
    Scene* FirstScene::createScene() {
        Scene* scene = Scene::create();
        FirstScene* layer = FirstScene::create();
        scene->addChild(layer);
        
        return scene;
    }
    
    /**
     *  @brief 初期化
     */
    bool FirstScene::init() {
        if (!Layer::init()) {
            return false;
        }
        
        return true;
    }
}
