//
//  GaugeSprite.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/27.
//

#include "GaugeSprite.hpp"
#include "GlobalDefine.hpp"
#include "Utility.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr char GAUGE_SPRITE_ANIMATION_SCHEDULER[] = "gauge_animation";
        const Size GAUGE_COMMON_INNNER_SIZE_OFFSET = cocos2d::Size(3.0f, 2.0f);
        constexpr float GAUGE_COMMON_WIDTH_OFFSET = 2.0f;
    }
    
    GaugeSprite::GaugeSprite() {
        
    }
    
    GaugeSprite::~GaugeSprite() {
        this->stopAllActions();
        Director::getInstance()->getScheduler()->unschedule(GAUGE_SPRITE_ANIMATION_SCHEDULER, this);
    }
    
    /**
     *  @brief 生成
     */
    GaugeSprite* GaugeSprite::create(Size _contentSize) {
        GaugeSprite* pRet = new (std::nothrow) GaugeSprite();
        
        if (pRet) {
            if (pRet->init(_contentSize)) {
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
    bool GaugeSprite::init(Size _contentSize) {
        if (!Node::init()) {
            return false;
        }
        
        commonContentSize_ = _contentSize;
        this->setContentSize(commonContentSize_);
        this->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        initSettings();
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void GaugeSprite::initSettings() {
        createFrame();
        createMainGauge();
    }
    
    /**
     *  @brief 外枠画像を設定
     */
    void GaugeSprite::createFrame() {
        // @memo. 以後必要であれば固有画像に差し替える
        pFrameSprite_ = Sprite::create();
        pFrameSprite_->setContentSize(commonContentSize_);
        pFrameSprite_->setTextureRect(Rect(0.0f, 0.0f, commonContentSize_.width, commonContentSize_.height));
        pFrameSprite_->setColor(Color3B::WHITE);
        pFrameSprite_->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pFrameSprite_->setPosition(this->getContentSize() / 2.0f);
        this->addChild(pFrameSprite_);
    }
    
    /**
     *  @brief ゲージ画像を設定
     */
    void GaugeSprite::createMainGauge() {
        // @memo. 以後必要であれば固有画像に差し替える
        pMainGaugeSprite_ = Sprite::create();
        pMainGaugeSprite_->setContentSize(commonContentSize_ - GAUGE_COMMON_INNNER_SIZE_OFFSET);
        pMainGaugeSprite_->setTextureRect(Rect(0.0f, 0.0f, pMainGaugeSprite_->getContentSize().width, pMainGaugeSprite_->getContentSize().height));
        pMainGaugeSprite_->setColor(Color3B::RED);
        pMainGaugeSprite_->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        pMainGaugeSprite_->setPosition(0.0f + GAUGE_COMMON_WIDTH_OFFSET, this->getContentSize().height / 2.0f);
        this->addChild(pMainGaugeSprite_);
        pMainGaugeSprite_->setScaleX(0.0f);
    }
    
    /**
     *  @brief ハイライト画像を設定
     */
    void GaugeSprite::createHighlightGauge() {
        // @memo. 以後必要であれば固有画像に差し替える
        pHighlightGaugeSprite_ = Sprite::create();
        pHighlightGaugeSprite_->setContentSize(commonContentSize_ - GAUGE_COMMON_INNNER_SIZE_OFFSET);
        pHighlightGaugeSprite_->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        pHighlightGaugeSprite_->setPosition(0.0f + GAUGE_COMMON_WIDTH_OFFSET, this->getContentSize().height / 2.0f);
        pHighlightGaugeSprite_->setOpacity(COMMON_SKELTON_OPACITY);
        this->addChild(pHighlightGaugeSprite_);
        pHighlightGaugeSprite_->setScaleX(0.0f);
    }
    
    /**
     *  @brief ゲージのスケールを変更
     */
    void GaugeSprite::changeGaugeScale(int32_t _currentValue, int32_t _maxValue) {
        if (!pMainGaugeSprite_ || !pHighlightGaugeSprite_) {
            return;
        }
        
        if (!setGaugeAnimationState(_currentValue, _maxValue)) {
            return;
        }
        
        pMainGaugeSprite_->setScaleX(targetScaleX_);
        pHighlightGaugeSprite_->setScaleX(targetScaleX_);
    }
    
    /**
     *  @brief ゲージの伸縮設定
     */
    bool GaugeSprite::setGaugeAnimationState(int32_t _currentValue, int32_t _maxValue) {
        if (!pMainGaugeSprite_ || !pHighlightGaugeSprite_) {
            return false;
        }
        
        targetScaleX_ = static_cast<float>(_currentValue) / static_cast<float>(_maxValue);
        if (targetScaleX_ >= 1.0f + FLT_EPSILON) {
            targetScaleX_ = 1.0f;
        }
        else if (targetScaleX_ <= 0.0f - FLT_EPSILON) {
            targetScaleX_ = 0.0f;
        }
        
        return true;
    }
    
    /**
     *  @brief ゲージ伸縮のアニメーションを開始
     *  @param bool _isAdd:増加である場合はtrue、減少である場合はfalse
     */
    void GaugeSprite::startGaugeAnimationByAction(int32_t _currentValue, int32_t _maxValue, float _frames, bool _isAdd) {
        if (!setGaugeAnimationState(_currentValue, _maxValue)) {
            return;
        }
        cocos2d::Vector<FiniteTimeAction*> actions;
        actions.pushBack(CallFunc::create([this, _frames, _isAdd]() {
            if (!_isAdd) {
                pMainGaugeSprite_->setScale(1.0f);
                pHighlightGaugeSprite_->setScale(1.0f);
            }
            
            pMainGaugeSprite_->runAction(Sequence::create(ScaleTo::create(Utility::exchangeFrameToSeconds(_frames), targetScaleX_, 1.0f), nullptr));
            pHighlightGaugeSprite_->runAction(Sequence::create(ScaleTo::create(Utility::exchangeFrameToSeconds(_frames), targetScaleX_, 1.0f), nullptr));
        }));
        actions.pushBack(DelayTime::create(_frames));
        actions.pushBack(CallFunc::create([this]() {
            // @todo. 必要であれば適宜色変更をする
            //mainGaugeSprite_->setColor(Color3B::RED);
        }));
        this->runAction(Sequence::create(actions));
    }
    
    /**
     *  @brief ゲージ伸縮のアニメーションを開始
     *  @param bool _isAdd:増加である場合はtrue、減少である場合はfalse
     */
    void GaugeSprite::startGaugeAnimationByScheduler(int32_t _currentValue, int32_t _maxValue, float _frames, float _interval, bool _paused, bool _isAdd) {
        if (!setGaugeAnimationState(_currentValue, _maxValue)) {
            return;
        }
        
        float addScaleValue = (targetScaleX_ <= 0.0f) ? (1.0f / _frames) : (targetScaleX_ / _frames);
        if (!_isAdd) {
            pMainGaugeSprite_->setScaleX(1.0f);
            pHighlightGaugeSprite_->setScaleX(1.0f);
            scalePerFrame_ = 1.0f;
        }

        Scheduler* scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule([this, addScaleValue, _isAdd](float _delta) {
            bool isFinish = false;
            
            if (_isAdd) {
                scalePerFrame_ += addScaleValue;
                if (scalePerFrame_ > targetScaleX_) {
                    scalePerFrame_ = targetScaleX_;
                    isFinish = true;
                }
            }
            else {
                scalePerFrame_ -= addScaleValue;
                if (scalePerFrame_ <= 0.0f) {
                    scalePerFrame_ = 0.0f;
                    isFinish = true;
                }
            }
            
            pMainGaugeSprite_->setScale(scalePerFrame_, 1.0f);
            pHighlightGaugeSprite_->setScale(scalePerFrame_, 1.0f);
            
            if (isFinish) {
                // @todo. 必要であれば適宜色変更をする
                //mainGaugeSprite_->setColor(Color3B::RED);

                scalePerFrame_ = 0.0f;
                targetScaleX_ = 0.0f;
                Director::getInstance()->getScheduler()->unschedule(GAUGE_SPRITE_ANIMATION_SCHEDULER, this);
            }
        }, this, _interval, _paused, GAUGE_SPRITE_ANIMATION_SCHEDULER);
    }
    
    /**
     *  @brief ゲージ伸縮のアニメーションを停止
     */
    void GaugeSprite::stopGaugeAnimation() {
        this->stopAllActions();
        Director::getInstance()->getScheduler()->unschedule(GAUGE_SPRITE_ANIMATION_SCHEDULER, this);
        scalePerFrame_ = 0.0f;
        targetScaleX_ = 0.0f;
    }
}
