//
//  EventsSequence.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/25.
//

#ifndef EventsSequence_hpp
#define EventsSequence_hpp

#include "cocos2d.h"

namespace project {
    class SequenceEvent;
    
    /**
     *  @brief
     */
    class EventsSequence : public cocos2d::Node {
    private:
        cocos2d::Vector<SequenceEvent*> eventsVector_;
        std::function<void(void)> onEndedAllEvents_;
        
    private:
        EventsSequence() = default;
        virtual ~EventsSequence() = default;
        
    public:
        static EventsSequence* crete();
        
        void pushEvent(SequenceEvent* _event);
        void pushEvents(const cocos2d::Vector<SequenceEvent*> _eventsVector);
        void start();
        void next();
        void setEndedAllEventsCallback(std::function<void(void)> _callback) { onEndedAllEvents_ = _callback; }
    };
}

#endif /* EventsSequence_hpp */
