//
//  ScrollSlider.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#include "ScrollSlider.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr float SLIDER_VERTICAL_ANGLE = 90.0f;
        // @todo. 仮設定、実装時には要差し替え
        constexpr char SLIDER_FRAME_TEXTURE[] = "CloseNormal.png";
        constexpr char SLIDER_MARKER_TEXTURE[] = "CloseSelected.png";
        constexpr char SLIDER_DUMMY_TEXTURE[] = "dummy.png";
        const Rect SLIDER_IN_RECT = {8.0f, 5.0f, 4.0f, 2.0f};
        const Size SLIDER_FRAME_SIZE = {720.0f,12.0f};
        const Size SLIDER_TOUCH_SIZE_OFFSET = {16.0f, 24.0f};
        constexpr float MIN_MARKER_WIDTH = 20.0f;
        constexpr float PERCENTAGE_VALUE_RATE = 0.01f;
    }
    
    /**
     *  @brief 生成
     */
    ScrollSlider* ScrollSlider::create() {
        ScrollSlider* pRet = new (std::nothrow) ScrollSlider();
        
        if (pRet) {
            if (pRet->init()) {
                pRet->autorelease();
            }
        }
        else {
            delete pRet;
            pRet = nullptr;
        }
        
        return pRet;
    }
    
    /**
     *  @brief 横長バーを生成
     */
    ScrollSlider* ScrollSlider::createHorizontal(float _markerLengthRate, float _width) {
        auto slider = ScrollSlider::create();
        
        if (slider) {
            slider->initSettings(_markerLengthRate, _width);
        }
        
        return slider;
    }
    
    /**
     *  @brief 縦長バーを生成
     */
    ScrollSlider* ScrollSlider::createVertical(float _markerLengthRate, float _height) {
        auto slider = ScrollSlider::create();
        
        if (slider) {
            slider->initSettings(_markerLengthRate, _height);
            slider->setRotation(SLIDER_VERTICAL_ANGLE);
        }
        
        return slider;
    }
    
    /**
     *  @brief 初期化
     */
    bool ScrollSlider::init() {
        if (!ui::Widget::init()) {
            return false;
        }
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void ScrollSlider::initSettings(float _markerLengthRate, float _width) {
        // 枠
        pFrame_ = ui::Scale9Sprite::create(SLIDER_FRAME_TEXTURE, SLIDER_IN_RECT);
        pFrame_->setContentSize(Size(_width, SLIDER_FRAME_SIZE.height));
        this->addChild(pFrame_);
        
        // 操作バー
        pMarker_ = ui::Scale9Sprite::create(SLIDER_MARKER_TEXTURE, SLIDER_IN_RECT);
        pMarker_->setContentSize(Size(_width, SLIDER_FRAME_SIZE.height));
        this->addChild(pMarker_);
        
        // スライダー
        pSlider_ = ui::Slider::create();
        pSlider_->loadBarTexture(SLIDER_DUMMY_TEXTURE);
        pSlider_->setScale9Enabled(true);
        pSlider_->setContentSize(Size(_width, SLIDER_FRAME_SIZE.height) + SLIDER_TOUCH_SIZE_OFFSET);
        pSlider_->addEventListener([this](Ref* _node, ui::Slider::EventType _eventType) {
            int32_t percent = pSlider_->getPercent();
            setMarkerPercent(percent);
            
            switch (_eventType) {
                case ui::Slider::EventType::ON_SLIDEBALL_DOWN:
                case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
                {
                    if (onSlide_) {
                        onSlide_(percent);
                    }
                }
                    break;
                    
                case ui::Slider::EventType::ON_SLIDEBALL_UP:
                case ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
                {
                    if (onSlideEnded_) {
                        onSlideEnded_();
                    }
                }
                    break;

                default:
                    break;
            }
        });
        this->addChild(pSlider_);
        
        this->setMarkerLength(_markerLengthRate);
    }
    
    /**
     *  @brief 操作バーの長さを設定
     */
    void ScrollSlider::setMarkerLength(float _markerLengthRate) {
        if (_markerLengthRate >= 1.0f) {
            this->setEnabled(false);
            this->setVisible(false);
            return;
        }
        
        this->setEnabled(true);
        this->setVisible(true);
        
        markerOriginalSize_ = Size(std::max(MIN_MARKER_WIDTH, pFrame_->getContentSize().width * _markerLengthRate), SLIDER_FRAME_SIZE.height);
        pMarker_->setContentSize(markerOriginalSize_);
        
        this->setMarkerPercent(static_cast<float>(this->getMarkerPercent()));
    }
    
    /**
     *  @brief 操作バーのパーセンテージを設定
     */
    void ScrollSlider::setMarkerPercent(float _percentage) {
        pSlider_->setPercent(static_cast<int32_t>(_percentage));
        
        float basePosX = (-pFrame_->getContentSize().width / 2.0f) + (markerOriginalSize_.width / 2.0f);
        float value = (pFrame_->getContentSize().width - markerOriginalSize_.width) * (_percentage * PERCENTAGE_VALUE_RATE);
        
        float leftSideValue = 0.0f;
        float rightSideValue = (pFrame_->getContentSize().width - markerOriginalSize_.width);
        
        if (value <= leftSideValue) {
            float diff = leftSideValue - value;
            pMarker_->setContentSize(markerOriginalSize_ + Size(-diff, 0.0f));
            pMarker_->setPositionX(basePosX + (leftSideValue - diff / 2.0f));
        }
        else if (value >= rightSideValue) {
            float diff = rightSideValue - value;
            pMarker_->setContentSize(markerOriginalSize_ + Size(-diff, 0.0f));
            pMarker_->setPositionX(basePosX + (rightSideValue + diff / 2.0f));
        }
        else {
            pMarker_->setContentSize(markerOriginalSize_);
            pMarker_->setPositionX(basePosX + value);
        }
    }
    
    /**
     *  @brief 操作バーのパーセンテージを取得
     */
    int32_t ScrollSlider::getMarkerPercent() {
        return pSlider_->getPercent();
    }
}
