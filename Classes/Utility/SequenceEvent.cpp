//
//  SequenceEvent.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/25.
//

#include "SequenceEvent.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr char SCHEDULE_TAG_UPDATE[] = "update";
        constexpr char SCHEDULE_TAG_DELETE[] = "delete";
    }
    
    SequenceEvent::~SequenceEvent() {
        Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    }
    
    /**
     *  @brief 生成
     */
    SequenceEvent* SequenceEvent::create() {
        SequenceEvent* pRet = new(std::nothrow) SequenceEvent();
        
        if (pRet) {
            pRet->autorelease();
        }
        else {
            delete pRet;
            pRet = nullptr;
        }
        
        return pRet;
    }
    
    /**
     *  @brief 生成
     */
    SequenceEvent* SequenceEvent::create(float _seconds, std::function<void(SequenceEvent*)> _onStartedCallback, std::function<void(float, SequenceEvent*)> _onUpdateCallback) {
        SequenceEvent* pRet = SequenceEvent::create();
        
        if (pRet) {
            pRet->setDurationSeconds(_seconds);
            pRet->setStartedCallback(_onStartedCallback);
            pRet->setUpdateCallback(_onUpdateCallback);
        }
        else {
            delete pRet;
            pRet = nullptr;
        }
        
        return pRet;
    }
    
    /**
     *  @brief 実行
     */
    void SequenceEvent::execute() {
        if (onUpdateCallback_) {
            Director::getInstance()->getScheduler()->schedule([this](float delta) {
                this->onUpdateCallback_(delta, this);
            }, this, 0.0f, false, SCHEDULE_TAG_UPDATE);
        }
        
        if (durationSeconds_ >= 0.0f) {
            Director::getInstance()->getScheduler()->schedule([this](float delta) {
                this->end();
            }, this, 0.0f, 1, durationSeconds_, false, SCHEDULE_TAG_DELETE);
        }
        
        if (onStartedCallback_) {
            onStartedCallback_(this);
        }
    }
    
    /**
     *  @brief 終了
     */
    void SequenceEvent::end() {
        Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
        onUpdateCallback_ = nullptr;
        
        if (onEndedCallback_) {
            onEndedCallback_();
        }
    }
}
