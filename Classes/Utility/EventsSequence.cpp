//
//  EventsSequence.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/25.
//

#include "EventsSequence.hpp"
#include "SequenceEvent.hpp"

USING_NS_CC;

namespace project {
    namespace {
        
    }
    
    /**
     *  @brief 生成
     */
    EventsSequence* EventsSequence::crete() {
        EventsSequence* pRet = new(std::nothrow) EventsSequence();
        
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
     *  @brief
     */
    void EventsSequence::pushEvent(SequenceEvent* _event) {
        eventsVector_.pushBack(_event);
    }
    
    /**
     *  @brief
     */
    void EventsSequence::pushEvents(const cocos2d::Vector<SequenceEvent*> _eventsVector) {
        eventsVector_.pushBack(_eventsVector);
    }
    
    /**
     *  @brief
     */
    void EventsSequence::start() {
        if (eventsVector_.size() > 0) {
            auto event = (*eventsVector_.begin());
            if (event) {
                event->setEndedCallback([this]() {
                    this->next();
                });
                event->execute();
            }
        }
        else {
            if (onEndedAllEvents_) {
                onEndedAllEvents_();
            }
        }
    }
    
    /**
     *  @brief
     */
    void EventsSequence::next() {
        if (eventsVector_.size() > 0) {
            eventsVector_.erase(eventsVector_.begin());
            start();
        }
        else {
            if (onEndedAllEvents_) {
                onEndedAllEvents_();
            }
        }
    }
}
