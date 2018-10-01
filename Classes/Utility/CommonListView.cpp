//
//  CommonListView.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/28.
//

#include "CommonListView.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr float NO_ACTION_SCROLL_DURATION = 0.0001f;
        constexpr float PAGE_JUMP_EASE_RATE = 2.0f;
        constexpr char UPDATE_SCROLL_INDEX_TAG[] = "update_scroll_index";
    }
    
    /**
     *  @brief 生成
     */
    CommonListView* CommonListView::create(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin) {
        CommonListView* pRet = new (std::nothrow) CommonListView();
        
        if (pRet) {
            if (pRet->init(_contentSize, _activeItemCount, _itemMargin)) {
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
    bool CommonListView::init(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin) {
        if (!ui::Layout::init()) {
            return false;
        }
        
        initSettings(_contentSize, _activeItemCount, _itemMargin);
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void CommonListView::initSettings(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin) {
        activeItemCount_ = _activeItemCount;
        
        this->setAnchorPoint(Point::ANCHOR_MIDDLE);
        this->setContentSize(_contentSize);
        
        listView_ = ui::ListView::create();
        listView_->setTouchEnabled(false);
        listView_->setSwallowTouches(false);
        listView_->setClippingEnabled(false);
        listView_->setItemsMargin(_itemMargin);
        listView_->setAnchorPoint(Point::ANCHOR_MIDDLE);
        listView_->setPosition(_contentSize / 2.0f);
        this->addChild(listView_);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(CommonListView::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(CommonListView::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(CommonListView::onTouchEnded, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    /**
     *  @brief タッチ開始
     */
    bool CommonListView::onTouchBegan(cocos2d::Touch* _touch, cocos2d::Event* _event) {
        if (!isTouchEnabled()) {
            return false;
        }
        
        // 1度目のタッチ以外は処理をさせない
        if (_touch->getID() != 0) {
            return false;
        }
        
        // タッチの範囲外の場合は処理をさせない
        auto bounds = this->getBoundingBox();
        if (!bounds.containsPoint(this->convertTouchToNodeSpaceAR(_touch))) {
            return false;
        }
        
        moveDiffPos_.set(0.0f, 0.0f);
        
        // コンテナの動きを停止させる
        containerVelocity_ = 0.0f;
        this->unschedule(schedule_selector(CommonListView::inertia));
        listView_->getInnerContainer()->stopAllActions();
        
        // スクロール開始時に実行
        if (onScrollBegan_) {
            onScrollBegan_();
        }
        
        return true;
    }
    
    /**
     *  @brief タッチ移動
     */
    void CommonListView::onTouchMoved(cocos2d::Touch* _touch, cocos2d::Event* _event) {
        if (!isTouchEnabled()) {
            return;
        }
        
        // 1度目のタッチ以外は処理をさせない
        if (_touch->getID() != 0) {
            return;
        }

        Vec2 diffPos = _touch->getLocationInView() - _touch->getPreviousLocationInView();
        if (std::fabsf(moveDiffPos_.x) < std::fabsf(diffPos.x) ||
            std::fabsf(moveDiffPos_.y) < std::fabsf(diffPos.y)) {
            moveDiffPos_ = diffPos;
        }
        
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
                containerVelocity_ = _touch->getDelta().x / 2.0f;
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
                containerVelocity_ = _touch->getDelta().y / 2.0f;
                break;
                
            default:
                break;
        }
        
        this->moveContainer(containerVelocity_);
    }
    
    /**
     *  @brief タッチ終了
     */
    void CommonListView::onTouchEnded(cocos2d::Touch* _touch, cocos2d::Event* _event) {
        if (!isTouchEnabled()) {
            return;
        }
        
        // 1度目のタッチ以外は処理をさせない
        if (_touch->getID() != 0) {
            return;
        }

        // 慣性開始
        this->schedule(schedule_selector(CommonListView::inertia));
    }
    
    /**
     *  @brief 現在のインデックスを取得
     *  @detail リストの表示しているアイテム群の先頭のインデックスを返す
     */
    int32_t CommonListView::getCurrentHeadItemIndex() {
        int32_t index = 0;
        
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float width = itemContentSize_.width + listView_->getItemsMargin();
                float innerContainerWidth = listView_->getInnerContainerSize().width - listView_->getContentSize().width;
                float innerContainerPositionX = listView_->getInnerContainerPosition().x;
                innerContainerPositionX = clampf(innerContainerPositionX, -innerContainerWidth, 0.0f);
                index = -((innerContainerPositionX - (width / 2.0f)) / width);
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float height = itemContentSize_.height + listView_->getItemsMargin();
                float innerContainerHeight = listView_->getInnerContainerSize().height - listView_->getContentSize().height;
                float innerContainerPositionY = listView_->getInnerContainerPosition().y;
                innerContainerPositionY = clampf(innerContainerPositionY, -innerContainerHeight, 0.0f);
                index = -((innerContainerPositionY - (height / 2.0f)) / height);
            }
                break;
                
            default:
                break;
        }
        
        return index;
    }
    
    /**
     *  @brief 左端かどうか
     */
    bool CommonListView::isLeftSide() {
        return (this->getCurrentHeadItemIndex() == 0);
    }
    
    /**
     *  @brief 右端かどうか
     */
    bool CommonListView::isRightSide() {
        return ((this->getCurrentHeadItemIndex() + this->getActiveItemCount()) == getItems().size());
    }
    
    /**
     *  @brief リストに格納されているアイテム群を取得
     */
    const Vector<cocos2d::ui::Widget*>& CommonListView::getItems() {
        return this->listView_->getItems();
    }
    
    /**
     *  @brief インデックスを指定してアイテムを取得
     */
    ui::Widget* CommonListView::getItem(int32_t _itemIndex) {
        if (_itemIndex >= 0 && _itemIndex < (listView_->getItems().size())) {
            return listView_->getItem(_itemIndex);
        }
        
        return nullptr;
    }
    
    /**
     *  @brief アイテムを追加する
     */
    void CommonListView::pushBackCustomItem(cocos2d::ui::Widget* _item) {
        // まだ何もアイテムが追加されていない場合はリストビューのサイズを設定する
        if (listView_->getItems().size() == 0) {
            itemContentSize_ = _item->getContentSize();

            switch (listView_->getDirection()) {
                case ui::ScrollView::Direction::HORIZONTAL:
                {
                    float itemWidth = itemContentSize_.width + listView_->getItemsMargin();
                    float activeItemWidth = (itemWidth * activeItemCount_) - listView_->getItemsMargin();
                    listView_->setContentSize(Size(activeItemWidth, itemContentSize_.height));
                }
                    break;
                    
                case ui::ScrollView::Direction::VERTICAL:
                {
                    float itemHeight = itemContentSize_.height + listView_->getItemsMargin();
                    float activeItemHeight = (itemHeight * activeItemCount_) - listView_->getItemsMargin();
                    listView_->setContentSize(Size(itemContentSize_.width, activeItemHeight));
                }
                    break;
                    
                default:
                    break;
            }
        }
        
        listView_->pushBackCustomItem(_item);
        refreshItemColor();
    }
    
    /**
     *  @brief 指定したインデックスまで移動する
     */
    void CommonListView::jumpToItem(int32_t _itemIndex, bool isWithoutAction) {
        listView_->getInnerContainer()->stopAllActions();
        
        float originalDuration = magneticDuration_;
        if (isWithoutAction) {
            magneticDuration_ = NO_ACTION_SCROLL_DURATION;
        }
        
        MoveTo* moveAction = nullptr;
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float itemWidth = itemContentSize_.width + listView_->getItemsMargin();
                float activeItemsWidth = (itemWidth * activeItemCount_) - listView_->getItemsMargin();
                float offset = (listView_->getContentSize().width - activeItemsWidth) / 2.0f;
                moveAction = MoveTo::create(magneticDuration_, Vec2(-(_itemIndex) * itemWidth + offset, 0.0f));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float itemHeight = itemContentSize_.height + listView_->getItemsMargin();
                float activeItemsHeight = (itemHeight * activeItemCount_) - listView_->getItemsMargin();
                float offset = (listView_->getContentSize().height - activeItemsHeight) / 2.0f;
                moveAction = MoveTo::create(magneticDuration_, Vec2(0.0f, -(_itemIndex) * itemHeight + offset));
            }
                break;
                
            default:
                moveAction = MoveTo::create(0.0f, Vec2(0.0f, 0.0f));
                break;
        }
        
        listView_->getInnerContainer()->runAction(Sequence::create({
            EaseOut::create(moveAction, PAGE_JUMP_EASE_RATE),
            CallFunc::create([this, isWithoutAction, originalDuration]() {
                this->unschedule(UPDATE_SCROLL_INDEX_TAG);
                refreshItemColor();
                
                int32_t currentIndex = this->getCurrentHeadItemIndex();
                if (currentIndex != itemIndex_) {
                    itemIndex_ = currentIndex;
                    if (onChangedItemIndex_) {
                        onChangedItemIndex_(itemIndex_);
                    }
                }
                
                if (onScrollEnded_) {
                    onScrollEnded_();
                }
                
                if (isWithoutAction) {
                    magneticDuration_ = originalDuration;
                }
            }),
        }));
        
        this->schedule([this](float _delta) {
            if (onScrolledItem_) {
                onScrolledItem_(this->getPercentage());
            }
        }, UPDATE_SCROLL_INDEX_TAG);
    }

    /**
     *  @brief 現在のインデックスまで移動する
     */
    void CommonListView::jumpToCurrentIndex() {
        this->jumpToItem(this->getCurrentHeadItemIndex());
    }
    
    /**
     *  @brief スクロール中かどうか
     */
    bool CommonListView::isScrolling() {
        return (std::fabsf(containerVelocity_) >= FLT_EPSILON);
    }
    
    /**
     *  @brief コンテナ位置をパーセンテージで取得
     */
    float CommonListView::getPercentage() {
        float percent = 0.0f;
        
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                if (std::fabsf(listView_->getContentSize().width - listView_->getInnerContainerSize().width) <= FLT_EPSILON) {
                    // ゼロ除算回避
                    return 100.0f;
                }
                
                percent = 100.0f * (-listView_->getInnerContainerPosition().x / (listView_->getInnerContainerSize().width - listView_->getContentSize().width));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
                percent = this->listView_->getScrolledPercentVertical();
                break;
                
            default:
                break;
        }
        
        return percent;
    }
    
    /**
     *  @brief コンテナの位置をパーセンテージで設定
     */
    void CommonListView::setPercentage(int32_t _percent) {
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float x = -(listView_->getInnerContainerSize().width - listView_->getContentSize().width) * (_percent / 100.0f);
                listView_->setInnerContainerPosition(Vec2(x, listView_->getInnerContainerPosition().y));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float y = -(listView_->getInnerContainerSize().height - listView_->getContentSize().height) * (_percent / 100.0f);
                listView_->setInnerContainerPosition(Vec2(listView_->getInnerContainerPosition().x, y));
            }
                break;
                
            default:
                break;
        }
        
        refreshItemColor();
        
        int32_t currentIndex = this->getCurrentHeadItemIndex();
        if (currentIndex != itemIndex_) {
            itemIndex_ = currentIndex;
            if (onChangedItemIndex_) {
                onChangedItemIndex_(itemIndex_);
            }
        }
    }
    
    /**
     *  @brief コンテナを移動する
     */
    bool CommonListView::moveContainer(float _velocity) {
        bool isOutOfBounds = false;
        
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float newPositionX = listView_->getInnerContainerPosition().x + _velocity;
                float innerContainerWidth = listView_->getInnerContainerSize().width - listView_->getContentSize().width;
                
                // 境界からはみ出ないように座標を丸める
                if (newPositionX <= -(innerContainerWidth + boundaryOutsideRange_)) {
                    newPositionX = -(innerContainerWidth + boundaryOutsideRange_);
                    isOutOfBounds = true;
                }
                else if (newPositionX >= 0.0f + boundaryOutsideRange_) {
                    newPositionX = 0.0f + boundaryOutsideRange_;
                    isOutOfBounds = true;
                }
                
                listView_->setInnerContainerPosition(Vec2(newPositionX, listView_->getInnerContainerPosition().y));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float newPositionY = listView_->getInnerContainerPosition().y + _velocity;
                float innerContainerHeight = listView_->getInnerContainerSize().height - listView_->getContentSize().height;

                // 境界からはみ出ないように座標を丸める
                if (newPositionY <= -(innerContainerHeight + boundaryOutsideRange_)) {
                    newPositionY = -(innerContainerHeight + boundaryOutsideRange_);
                    isOutOfBounds = true;
                }
                else if (newPositionY >= 0.0f + boundaryOutsideRange_) {
                    newPositionY = 0.0f + boundaryOutsideRange_;
                    isOutOfBounds = true;
                }
                
                listView_->setInnerContainerPosition(Vec2(listView_->getInnerContainerPosition().x, newPositionY));
            }
                break;
                
            default:
                break;
        }
        
        refreshItemColor();
        
        if (onScrolledItem_) {
            onScrolledItem_(this->getPercentage());
        }
        
        int32_t currentIndex = this->getCurrentHeadItemIndex();
        if (currentIndex != itemIndex_) {
            itemIndex_ = currentIndex;
            if (onChangedItemIndex_) {
                onChangedItemIndex_(itemIndex_);
            }
        }

        return isOutOfBounds;
    }
    
    /**
     *  @brief 慣性
     */
    void CommonListView::inertia(float _delta) {
        containerVelocity_ *= inertiaStlength_;
        
        // マグネット
        if (std::fabsf(containerVelocity_) < velocityEndInertia_) {
            this->unschedule(schedule_selector(CommonListView::inertia));
            this->jumpToItem(this->getCurrentHeadItemIndex());
        }
        // 慣性
        else {
            if (moveContainer(containerVelocity_)) {
                containerVelocity_ = 0.0f;
            }
        }
    }
    
    /**
     *  @brief アイテムの色を更新
     */
    void CommonListView::refreshItemColor() {
        int32_t currentIndex = this->getCurrentHeadItemIndex();
        auto items = this->getItems();

        // @memo. 初回コンテナ生成時にインデックスが正しく取得出来ないため、サイズを超えたインデックスの場合は最後尾のインデックスをさすようにする
        // @todo. 本当に必要かどうか動作をチェックすること
        if (currentIndex > static_cast<int32_t>(listView_->getItems().size()) - activeItemCount_) {
            currentIndex = static_cast<int32_t>(listView_->getItems().size()) - activeItemCount_;
        }
        
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                for (auto itr = items.begin(), end = items.end(); itr != end; itr++) {
                    size_t index = std::distance(items.begin(), itr);
                    
                    if ((index >= currentIndex) && index < (currentIndex + activeItemCount_)) {
                        (*itr)->setColor(Color3B::WHITE);
                    }
                    // @memo. currentIndexがマイナス値になる場合はスクロールエリア内に全てのアイテムが収まっていることになるのでホワイト化
                    // @todo. 要確認
                    else if (currentIndex <= 0) {
                        (*itr)->setColor(Color3B::WHITE);
                    }
                    else {
                        (*itr)->setColor(Color3B::GRAY);
                    }
                }
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                for (auto itr = items.rbegin(), end = items.rend(); itr != end; itr++) {
                    size_t index = std::distance(items.rbegin(), itr);
                    
                    if ((index >= currentIndex) && index < (currentIndex + activeItemCount_)) {
                        (*itr)->setColor(Color3B::WHITE);
                    }
                    // @memo. currentIndexがマイナス値になる場合はスクロールエリア内に全てのアイテムが収まっていることになるのでホワイト化
                    // @todo. 要確認
                    else if (currentIndex <= 0) {
                        (*itr)->setColor(Color3B::WHITE);
                    }
                    else {
                        (*itr)->setColor(Color3B::GRAY);
                    }
                }
            }
                break;
                
            default:
                break;
        }
    }
    
    /**
     *  @brief リスト内の全てのアイテムを削除
     */
    void CommonListView::removeAllItemsInList() {
        if (listView_->getItems().empty()) {
            return;
        }
        
        listView_->removeAllItems();
        
        // インデックスをクリアする意味合いで、コンテナを左端あるいは最上部へ戻す
        switch (listView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
                listView_->ListView::jumpToLeft();
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
                listView_->ListView::jumpToTop();
                break;
                
            default:
                break;
        }
    }
    
    /**
     *  @brief 指定したアイテムを削除
     */
    void CommonListView::removeItem(Node* _item) {
        if (!_item) {
            return;
        }
        
        listView_->removeChild(_item);
    }
    
    /**
     *  @brief スクロールする方向を設定
     */
    void CommonListView::setScrollDiretion(ui::ScrollView::Direction _direction) {
        listView_->setDirection(_direction);
    }
    
    /**
     *  @brief クリッピングを設定
     */
    void CommonListView::setClippingEnabled(bool _enabled) {
        listView_->setClippingEnabled(_enabled);
    }
    
    /**
     *  @brief コンテナを取得
     */
    ui::Layout* CommonListView::getContainer() const {
        return listView_->getInnerContainer();
    }
    
    /**
     *  @brief スクロールを止める
     */
    void CommonListView::suspendScrolling()  {
        this->setTouchEnabled(false);
        this->jumpToItem(this->getCurrentHeadItemIndex());
        containerVelocity_ = 0.0f;
    }
    
    /**
     *  @brief スクロールを再開する
     */
    void CommonListView::resumeScrolling() {
        this->setTouchEnabled(true);
    }
    
    /**
     *  @brief コンテナのサイズを取得
     */
    Size CommonListView::getInnerContainerSize() const {
        return listView_->getInnerContainerSize();
    }
    
    /**
     *  @brief レイアウトを更新
     */
    void CommonListView::doLayout() {
        listView_->doLayout();
    }
}
