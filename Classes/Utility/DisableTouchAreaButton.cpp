//
//  DisableTouchAreaButton.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#include "DisableTouchAreaButton.hpp"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace project {
    namespace {
        int32_t OPACITY_DISABLE_BUTTON = 192;
    }
    
    /**
     *  @brief 生成
     */
    DisableTouchAreaButton* DisableTouchAreaButton::create(const cocos2d::Size& _areaSize) {
        DisableTouchAreaButton* pRet = new (std::nothrow) DisableTouchAreaButton();
        
        if (pRet) {
            if (pRet->init(_areaSize)) {
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
     *  @brief 初期化
     */
    bool DisableTouchAreaButton::init(const cocos2d::Size& _areaSize) {
        if (!Node::init()) {
            return false;
        }
        
        this->setContentSize(_areaSize);
        
        initSettings();
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void DisableTouchAreaButton::initSettings() {
        pDisableButton_ = ui::Button::create();
        pDisableButton_->setScale9Enabled(true);
        pDisableButton_->setContentSize(this->getContentSize());
        pDisableButton_->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pDisableButton_->setPosition(this->getContentSize() / 2.0f);
        this->addChild(pDisableButton_);
        pDisableButton_->addClickEventListener([this](Ref* _node) {
            // @memo. 何も記述しない事で実質タップ不可の挙動を取る
        });
        pDisableButton_->setVisible(false);
        
        pAreaSprite_ = Sprite::create();
        pAreaSprite_->setTextureRect(Rect(0.0f, 0.0f, pDisableButton_->getContentSize().width, pDisableButton_->getContentSize().height));
        // @mremo. デフォルトはブラック設定
        pAreaSprite_->setColor(Color3B::BLACK);
        pAreaSprite_->setOpacity(OPACITY_DISABLE_BUTTON);
        pAreaSprite_->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pAreaSprite_->setPosition(pDisableButton_->getContentSize() / 2.0f);
        pDisableButton_->addChild(pAreaSprite_);
    }
    
    /**
     *  @brief 表示サイズを設定
     */
    void DisableTouchAreaButton::setAreaSize(const cocos2d::Size& _areaSize) {
        this->setContentSize(_areaSize);
        
        if (pDisableButton_) {
            pDisableButton_->setContentSize(_areaSize);
        }
        
        if (pAreaSprite_) {
            pAreaSprite_->setTextureRect(Rect(0.0f, 0.0f, _areaSize.width, _areaSize.height));
        }
    }
    
    /**
     *  @brief 表示色を変更
     */
    void DisableTouchAreaButton::setAreaSpriteColor(cocos2d::Color4B _color) {
        if (!pAreaSprite_) {
            return;
        }
        
        pAreaSprite_->setColor(Color3B(_color));
        pAreaSprite_->setOpacity(_color.a);
    }
}
