//
//  DrawCircle.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "DrawCircle.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr float EXCHANGE_DEGREE_RATE = 0.2777777;
        constexpr int32_t ADJUST_DEGREE_VALUE = -90;
        constexpr int8_t TRIANGLE_VERTEX_COUNT = 3;
    }
    
    /**
     *  @brief 生成
     */
    DrawCircle* DrawCircle::create() {
        DrawCircle* pRet = new (std::nothrow)DrawCircle();
        
        if (pRet) {
            if(pRet->init()) {
                pRet->autorelease();
            }
        }
        else {
            CC_SAFE_DELETE(pRet);
        }
        
        return pRet;
    }
    
    /**
     *  @brief 初期化
     */
    bool DrawCircle::init() {
        if (!Node::init()) {
            return false;
        }
        
        initSettings();
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void DrawCircle::initSettings() {
        pSingleCircle_ = DrawNode::create();
        pSingleCircle_->setPosition({0.0f, 0.0f});
        this->addChild(pSingleCircle_);
        pSingleCircle_->setVisible(false);
        
        pCircleNode_ = Node::create();
        pCircleNode_->setPosition({0.0f, 0.0f});
        this->addChild(pCircleNode_);
        pCircleNode_->setCascadeOpacityEnabled(true);
        pCircleNode_->setCascadeColorEnabled(true);
        pCircleNode_->setVisible(false);
    }
    
    /**
     *  @brief 単独円を描画
     *  @details 指定された半径大のドットを円とみなす
     */
    void DrawCircle::drawSingleCircle(float _radius, Color4F _color) {
        if(!pSingleCircle_) {
            return;
        }
        
        pSingleCircle_->setVisible(true);
        pSingleCircle_->drawDot({0.0f, 0.0f}, _radius, _color);
    }
    
    /**
     *  @brief 円をパーセンテージに応じて描画
     */
    void DrawCircle::drawPercentageCircle(const std::vector<float> _percentageVector, float _radius, const std::vector<Color4F> _colorVector) {
        if (!pCircleNode_) {
            return;
        }
        
        pCircleNode_->removeAllChildren();
        
        float startDegree = 0.0f;
        float endDegree = 0.0f;
        float previousPercent = 0.0f;
        int8_t count = 0;
        
        // @todo. パーセンテージとカラーのカウントチェック
        
        for (auto itr = _percentageVector.begin(); itr != _percentageVector.end(); itr++) {
            startDegree = calcPercentageToDegree(previousPercent);
            endDegree = calcPercentageToDegree(previousPercent + (*itr));
            drawPolygon(_radius, startDegree, endDegree, _colorVector[count++]);
            previousPercent = previousPercent + (*itr);
        }
    }
    
    /**
     *  @brief 指定された角度の三角形を描画
     */
    void DrawCircle::drawPolygon(float _radius, float _startDegree, float _endDegree, Color4F _color) {
        if (!pCircleNode_) {
            return;
        }
        
        pCircleNode_->setVisible(true);

        // 真上をゼロ度とするため角度を調整
        int32_t exchangedStartDegree = static_cast<int32_t>(std::round(_startDegree)) + ADJUST_DEGREE_VALUE;
        int32_t exchangedEndDegree = static_cast<int32_t>(std::round(_endDegree)) + ADJUST_DEGREE_VALUE;
        
        for (int32_t degree = exchangedStartDegree; degree < exchangedEndDegree; degree++) {
            float currentRadian = -CC_DEGREES_TO_RADIANS(degree);
            float nextRadian = -CC_DEGREES_TO_RADIANS(degree + 1);
            
            std::vector<Vec2> vertexVector = {
                {0.0f, 0.0f},
                {_radius * cos(currentRadian), _radius * sin(currentRadian)},
                {_radius * cos(nextRadian), _radius * sin(nextRadian)}
            };
            auto drawNode = DrawNode::create();
            drawNode->drawPolygon(&vertexVector[0], TRIANGLE_VERTEX_COUNT, _color, 0.0f, Color4F::BLACK);
            pCircleNode_->addChild(drawNode);
        }
    }
    
    /**
     *  @brief パーセントを角度に変換
     */
    float DrawCircle::calcPercentageToDegree(float _percent) {
        float retValue = 0.0f;
        
        if (_percent > 0.0f) {
            retValue = _percent / EXCHANGE_DEGREE_RATE;
        }
        
        return retValue;
    }
    
    /**
     *  @brief 単独円の状態を設定
     */
    void DrawCircle::setSingleCircleState(bool _visible, int32_t _opacity, Color3B _color) {
        if(!pSingleCircle_) {
            return;
        }
        
        pSingleCircle_->setColor(_color);
        pSingleCircle_->setVisible(_visible);
        pSingleCircle_->setOpacity(_opacity);
    }
    
    /**
     *  @brief 確率円の状態を設定
     */
    void DrawCircle::setPercentageCircleState(bool _visible, int32_t _opacity, Color3B _color) {
        if (!pCircleNode_) {
            return;
        }
        
        pCircleNode_->setColor(_color);
        pCircleNode_->setVisible(_visible);
        pCircleNode_->setOpacity(_opacity);
    }
}
