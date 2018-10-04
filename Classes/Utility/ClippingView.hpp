//
//  ClippingView.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/03.
//

#ifndef ClippingView_hpp
#define ClippingView_hpp

#include "cocos2d.h"

namespace project {
    /**
     *  @brief 画面切り取り表示
     */
    class ClippingView : public cocos2d::Node {
    private:
        cocos2d::Size contentSize_{cocos2d::Size::ZERO};
        cocos2d::LayerColor* pLayerColor_{nullptr};
        cocos2d::Node* pStencilNodeParent_{nullptr};
        cocos2d::DrawNode* pStencilNode_{nullptr};
        cocos2d::ClippingNode* pClippingNode_{nullptr};
        cocos2d::EventListenerTouchOneByOne* pEventListener_{nullptr};
        std::function<void(void)> onTapView_{nullptr};
        
        enum class DrawPriority {
            Mask = 0,
            Button,
            Object,
        };
        
    private:
        ClippingView() = default;
        virtual ~ClippingView() = default;
        
        bool init(const cocos2d::Size& _viewSize);
        void initSettings();
        void createMaskView();
        
        
    public:
        static ClippingView* create(const cocos2d::Size& _viewSize);
        
        void changeStencil(float _radius, const cocos2d::Vec2& _position);
        void changeStencil(std::vector<cocos2d::Vec2> _vertexVector, const cocos2d::Vec2& _position);
        void changeStencil(const cocos2d::Size& _size, const cocos2d::Vec2& _posision);
        void changeStencil(const cocos2d::Rect& _rect);
        void clearStencil();
        void setTapViewCallback(std::function<void(void)> _callback) { onTapView_ = _callback; }
    };
}

#endif /* ClippingView_hpp */
