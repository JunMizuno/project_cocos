//
//  DrawCircle.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef DrawCircle_hpp
#define DrawCircle_hpp

#include "cocos2d.h"

namespace project {
    /**
     *  @brief 円を描画
     */
    class DrawCircle : public cocos2d::Node {
    private:
        cocos2d::DrawNode* pSingleCircle_{nullptr};
        cocos2d::Node* pCircleNode_{nullptr};
        
    private:
        DrawCircle() = default;
        virtual ~DrawCircle() = default;
        
        bool init();
        void initSettings();
        
        void drawPolygon(float _radius, float _startDegree, float _endDegree, cocos2d::Color4F _color);
        float calcPercentageToDegree(float _percent);
        
    public:
        static DrawCircle* create();
        
        void drawSingleCircle(float _radius, cocos2d::Color4F _color);
        void drawPercentageCircle(const std::vector<float> _percentageVector, float _radius, const std::vector<cocos2d::Color4F> _colorVector);
        void setSingleCircleState(bool _visible, int32_t _opacity, cocos2d::Color3B _color = cocos2d::Color3B::WHITE);
        void setPercentageCircleState(bool _visible, int32_t _opacity, cocos2d::Color3B _color = cocos2d::Color3B::WHITE);
    };
}

#endif /* DrawCircle_hpp */
