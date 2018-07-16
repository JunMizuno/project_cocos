//
//  FirstScene.cpp
//  project_cocos-mobile
//
//  Created by Jun Mizuno on 2018/07/06.
//

#include "FirstScene.hpp"
#include "Utility.hpp"

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
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(FirstScene::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(FirstScene::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(FirstScene::onTouchEnded, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        initSettings();
        
        return true;
    }
    
    /**
     *
     */
    bool FirstScene::onTouchBegan(Touch* touch, Event* event) {
        Utility::getRandomValue(1, 100);
        
        return true;
    }
    
    /**
     *
     */
    void FirstScene::onTouchMoved(Touch* touch, Event* event) {
        
    }
    
    /**
     *
     */
    void FirstScene::onTouchEnded(Touch *touch, Event *event) {
        
    }
    
    /**
     *  @brief 初期設定
     */
    void FirstScene::initSettings() {
        // 型を推測する(auto)
        {
            decltype(1+1) temp;
            temp = 1;
        }
        
        
    
        
        
        
        
        
        
        
        
        
        
        
    }
    
    /**
     *  @brief 次のシーンへ
     */
    void FirstScene::nextScene() {
        Scene* scene = Scene::create();
        // @todo. ここで次のシーンを指定する事
        FirstScene* layer = FirstScene::create();
        scene->addChild(layer);

        Director::getInstance()->replaceScene(scene);
    }
}
