//
//  ProgressTimerSprite.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef ProgressTimerSprite_hpp
#define ProgressTimerSprite_hpp

#include "cocos2d.h"

namespace project {
    /**
     *  @brief
     */
    class ProgressTimerSprite : public cocos2d::Node {
    private:
        cocos2d::ProgressTimer* pProgressTimer_{nullptr};
        
    private:
        ProgressTimerSprite() = default;
        virtual ~ProgressTimerSprite() = default;
        
        bool init(cocos2d::Sprite* _sprite);
        void initSettings(cocos2d::Sprite* _sprite);
        
    public:
        ProgressTimerSprite* create(const std::string _filePath);
        ProgressTimerSprite* create(cocos2d::Sprite* _sprite);
        
        void setBarType(cocos2d::ProgressTimer::Type _typr);
        float getPercentage();
    };
}

#endif /* ProgressTimerSprite_hpp */
