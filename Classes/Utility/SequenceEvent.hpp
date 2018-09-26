//
//  SequenceEvent.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/25.
//

#ifndef SequenceEvent_hpp
#define SequenceEvent_hpp

#include "cocos2d.h"

namespace project {
    /**
     *  @brief イベント設定
     */
    class SequenceEvent : public cocos2d::Ref {
    private:
        float durationSeconds_{-1.0f};
        std::function<void(SequenceEvent*)> onStartedCallback_{nullptr};
        std::function<void(float, SequenceEvent*)> onUpdateCallback_{nullptr};
        std::function<void(void)> onEndedCallback_{nullptr};
        
    private:
        SequenceEvent() = default;
        virtual ~SequenceEvent();
        
    public:
        
        static SequenceEvent* create();
        static SequenceEvent* create(float _seconds, std::function<void(SequenceEvent*)> _onStartedCallback, std::function<void(float, SequenceEvent*)> _onUpdateCallback = nullptr);
        
        void setDurationSeconds(float _seconds) { durationSeconds_ = _seconds; }
        void setStartedCallback(std::function<void(SequenceEvent*)> _callback) { onStartedCallback_ = _callback; };
        void setUpdateCallback(std::function<void(float, SequenceEvent*)> _callback) { onUpdateCallback_ = _callback; };
        void setEndedCallback(std::function<void(void)> _callback) { onEndedCallback_ = _callback; };
        void execute();
        void end();
    };
}

#endif /* SequenceEvent_hpp */
