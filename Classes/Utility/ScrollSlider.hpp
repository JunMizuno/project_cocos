//
//  ScrollSlider.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#ifndef ScrollSlider_hpp
#define ScrollSlider_hpp

#include "ui/CocosGUI.h"

namespace project {
    /**
     *  @brief スクロールスライダー
     */
    class ScrollSlider : public cocos2d::ui::Widget {
    private:
        cocos2d::ui::Slider* slider_{nullptr};
        cocos2d::ui::Scale9Sprite* frame_{nullptr};
        cocos2d::ui::Scale9Sprite* marker_{nullptr};
        cocos2d::Size markerOriginalSize_{cocos2d::Size::ZERO};
        std::function<void(int32_t)> onSlide_{nullptr};
        std::function<void(void)> onSlideEnded_{nullptr};
        
    private:
        ScrollSlider() = default;
        virtual ~ScrollSlider() = default;
        
        bool init() override;
        void initSettings(float _markerLengthRate, float _width);
        void setMarkerLength(float _markerLengthRate);
        
    public:
        static ScrollSlider* create();
        static ScrollSlider* createHorizontal(float _markerLengthRate, float _width);
        static ScrollSlider* createVertical(float _markerLengthRate, float _height);
        
        void setMarkerPercent(float _percentage);
        int32_t getMarkerPercent();
        void setSlideCallback(std::function<void(int32_t)> _callback) { onSlide_ = _callback; }
        void setSlideEndedCallback(std::function<void(void)> _callback) { onSlideEnded_ = _callback; }
    };
}

#endif /* ScrollSlider_hpp */
