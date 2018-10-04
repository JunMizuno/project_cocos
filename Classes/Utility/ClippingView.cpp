//
//  ClippingView.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/03.
//

#include "ClippingView.hpp"

USING_NS_CC;

namespace project {
    namespace {
        const Color4B MASK_VIEW_COLOR = Color4B(0, 0, 0, 128);
    }
    
    /**
     *  @brief 生成
     */
    ClippingView* ClippingView::create(const cocos2d::Size& _viewSize) {
        ClippingView* pRet = new (std::nothrow) ClippingView();
        
        if (pRet) {
            if (pRet->init(_viewSize)) {
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
     *  @brief 初期化
     */
    bool ClippingView::init(const cocos2d::Size& _viewSize) {
        if (!Node::init()) {
            return false;
        }
        
        contentSize_ = _viewSize;
        
        initSettings();
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void ClippingView::initSettings() {
        createMaskView();
    }
    
    /**
     *  @brief マスク画像を生成
     */
    void ClippingView::createMaskView() {
        pLayerColor_ = LayerColor::create(MASK_VIEW_COLOR, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
        
        pEventListener_ = EventListenerTouchOneByOne::create();
        pEventListener_->setSwallowTouches(false);
        pEventListener_->onTouchBegan = [this](Touch* _pTouch, Event* _pEvent) {
            return true;
        };
        pEventListener_->onTouchMoved = [this](Touch* _pTouch, Event* _pEvent) {
            return true;
        };
        pEventListener_->onTouchEnded = [this](Touch* _pTouch, Event* _pEvent) {
            if (onTapView_) {
                onTapView_();
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pEventListener_, this);
        this->getEventDispatcher()->setPriority(pEventListener_, -1);
        
        // @memo. 同時に切り抜くパターンが発生する場合はこの親ノードに追加する
        pStencilNodeParent_ = Node::create();
        pStencilNode_ = DrawNode::create();
        pStencilNodeParent_->addChild(pStencilNode_);
        
        pClippingNode_ = ClippingNode::create();
        // @memo. 切り抜いた部分以外を表示
        pClippingNode_->setInverted(true);
        pClippingNode_->setStencil(pStencilNodeParent_);
        this->addChild(pClippingNode_);
        pClippingNode_->addChild(pLayerColor_, static_cast<int32_t>(DrawPriority::Mask));
    }
    
    /**
     *  @brief 切り抜き範囲を変更
     *  @details 円形に切り抜きを設定する
     */
    void ClippingView::changeStencil(float _radius, const cocos2d::Vec2& _position) {
        clearStencil();
        
        pStencilNode_->drawDot(_position, _radius, Color4F::WHITE);
        pClippingNode_->setStencil(pStencilNodeParent_);
    }
    
    /**
     *  @brief 切り抜き範囲を変更
     *  @details 頂点を指定して切り抜きを設定する
     */
    void ClippingView::changeStencil(std::vector<cocos2d::Vec2> _vertexVector, const cocos2d::Vec2& _position) {
        clearStencil();
        
        pStencilNode_->drawPolygon(&_vertexVector[0], static_cast<int32_t>(_vertexVector.size()), Color4F::WHITE, 0, Color4F::WHITE);
        pStencilNode_->setPosition(_position);
        pClippingNode_->setStencil(pStencilNodeParent_);
    }
    
    /**
     *  @brief 切り抜き範囲を変更
     *  @details 矩形に切り抜きを設定する
     */
    void ClippingView::changeStencil(const cocos2d::Size &_size, const cocos2d::Vec2 &_posision) {
        std::vector<cocos2d::Vec2> _vertexVector = {{0.0f, 0.0f}, {0.0f, _size.height}, {_size.width, _size.height}, {_size.width, 0.0f}};
        changeStencil(_vertexVector, _posision - _size / 2.0f);
    }
    
    /**
     *  @brief 切り抜き範囲を変更
     *  @details 矩形に切り抜きを設定する、座標はRectの値に依存する
     */
    void ClippingView::changeStencil(const cocos2d::Rect& _rect) {
        float width = _rect.getMaxX() - _rect.getMinX();
        float height = _rect.getMaxY() - _rect.getMinY();
        std::vector<cocos2d::Vec2> _vertexVector = {{0.0f, 0.0f}, {0.0f, height}, {width, height}, {width, 0.0f}};
        changeStencil(_vertexVector, Vec2(_rect.getMinX(), _rect.getMinY()));
    }
    
    /**
     *  @brief 切り抜き範囲を初期化する
     */
    void ClippingView::clearStencil() {
        pStencilNode_->clear();
        pClippingNode_->setStencil(pStencilNodeParent_);
    }
}
