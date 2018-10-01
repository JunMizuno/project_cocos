//
//  GaugeSprite.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/27.
//

#ifndef GaugeSprite_hpp
#define GaugeSprite_hpp

#include "cocos2d.h"

namespace project {
    /**
     *  @brief ゲージ画像
     */
    class GaugeSprite : public cocos2d::Node {
    private:
        cocos2d::Size commonContentSize_{0.0f, 0.0f};
        cocos2d::Sprite* frameSprite_{nullptr};
        cocos2d::Sprite* mainGaugeSprite_{nullptr};
        cocos2d::Sprite* highlightGaugeSprite_{nullptr};
        float targetScaleX_{0.0f};
        float scalePerFrame_{0.0f};
        
    private:
        GaugeSprite();
        virtual ~GaugeSprite();
        
        bool init(cocos2d::Size _contentSize);
        void initSettings();
        void createFrame();
        void createMainGauge();
        void createHighlightGauge();
        bool setGaugeAnimationState(int32_t _currentValue, int32_t _maxValue);
        
    public:
        static GaugeSprite* create(cocos2d::Size _contentSize);
        
        void changeGaugeScale(int32_t _currentValue, int32_t _maxValue);
        void startGaugeAnimationByAction(int32_t _currentValue, int32_t _maxValue, float _frames, bool _isAdd = true);
        void startGaugeAnimationByScheduler(int32_t _currentValue, int32_t _maxValue, float _frames, float _interval, bool _paused, bool _isAdd = true);
        void stopGaugeAnimation();
    };
}

#endif /* GaugeSprite_hpp */
