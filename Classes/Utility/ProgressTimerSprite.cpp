//
//  ProgressTimerSprite.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "ProgressTimerSprite.hpp"

USING_NS_CC;

namespace project {
    namespace {
        
    }
    
    /**
     *  @brief 生成
     */
    ProgressTimerSprite* ProgressTimerSprite::create(const std::string _filePath) {
        ProgressTimerSprite* pRet = new (std::nothrow)ProgressTimerSprite();
        
        if (pRet) {
            if (pRet->init(Sprite::create(_filePath))) {
                pRet->autorelease();
            }
        }
        else {
            CC_SAFE_DELETE(pRet);
        }
        
        return pRet;
    }
    
    /**
     *  @brief 生成
     */
    ProgressTimerSprite* ProgressTimerSprite::create(cocos2d::Sprite* _sprite) {
        ProgressTimerSprite* pRet = new (std::nothrow)ProgressTimerSprite();

        if (pRet) {
            if (pRet->init(_sprite)) {
                pRet->autorelease();
            }
        }
        else {
            CC_SAFE_DELETE(pRet);
        }
        
        return pRet;
    }
    
    /**
     *  @brief 初期化
     */
    bool ProgressTimerSprite::init(cocos2d::Sprite* _sprite) {
        if (!Node::init()) {
            return false;
        }
        
        initSettings(_sprite);
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void ProgressTimerSprite::initSettings(cocos2d::Sprite* _sprite) {
        pProgressTimer_ = ProgressTimer::create(_sprite);
        pProgressTimer_->setPosition({0.0f, 0.0f});
        this->addChild(pProgressTimer_);
        
        
        
        
    }
    
    /**
     *  @brief
     */
    void ProgressTimerSprite::setBarType(cocos2d::ProgressTimer::Type _typr) {
        
    }
    
    /**
     *  @brief
     */
    float ProgressTimerSprite::getPercentage() {
        
    }
}
