//
//  DisableTouchAreaButton.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#ifndef DisableTouchAreaButton_hpp
#define DisableTouchAreaButton_hpp

#include "cocos2d.h"

namespace cocos2d {
namespace ui {
    class Button;
}
}

namespace project {
    /**
     *  @brief タッチ不可領域ボタン
     */
    class DisableTouchAreaButton : public cocos2d::Node {
    private:
        cocos2d::ui::Button* pDisableButton_{nullptr};
        cocos2d::Sprite* pAreaSprite_{nullptr};
        
    private:
        DisableTouchAreaButton() = default;
        virtual ~DisableTouchAreaButton() = default;
        
        bool init(const cocos2d::Size& _areaSize);
        void initSettings();
        
    public:
        static DisableTouchAreaButton* create(const cocos2d::Size& _areaSize);
        
        void setAreaSize(const cocos2d::Size& _areaSize);
        void setAreaSpriteColor(cocos2d::Color4B _color);
    };
}

#endif /* DisableTouchAreaButton_hpp */
