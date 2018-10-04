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
        constexpr float SCROLL_MAX_PERCENTAGE = 100.0f;
        constexpr float ITEMS_MAX_SCALE = 1.0f;
        constexpr float ITEMS_MIN_SCALE = 0.8f;
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
        
        pListView_ = ui::ListView::create();
        pListView_->setTouchEnabled(false);
        pListView_->setSwallowTouches(false);
        pListView_->setClippingEnabled(false);
        pListView_->setItemsMargin(_itemMargin);
        pListView_->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pListView_->setPosition(_contentSize / 2.0f);
        this->addChild(pListView_);
        
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
        pListView_->getInnerContainer()->stopAllActions();
        
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
        
        switch (pListView_->getDirection()) {
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
     *  @details リストの表示しているアイテム群の先頭のインデックスを返す
     */
    int32_t CommonListView::getCurrentHeadItemIndex() {
        int32_t index = 0;
        
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float width = itemContentSize_.width + pListView_->getItemsMargin();
                float innerContainerWidth = pListView_->getInnerContainerSize().width - pListView_->getContentSize().width;
                float innerContainerPositionX = pListView_->getInnerContainerPosition().x;
                innerContainerPositionX = clampf(innerContainerPositionX, -innerContainerWidth, 0.0f);
                index = -((innerContainerPositionX - (width / 2.0f)) / width);
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float height = itemContentSize_.height + pListView_->getItemsMargin();
                float innerContainerHeight = pListView_->getInnerContainerSize().height - pListView_->getContentSize().height;
                float innerContainerPositionY = pListView_->getInnerContainerPosition().y;
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
        return this->pListView_->getItems();
    }
    
    /**
     *  @brief インデックスを指定してアイテムを取得
     */
    ui::Widget* CommonListView::getItem(int32_t _itemIndex) {
        if (_itemIndex >= 0 && _itemIndex < (pListView_->getItems().size())) {
            return pListView_->getItem(_itemIndex);
        }
        
        return nullptr;
    }
    
    /**
     *  @brief アイテムを追加する
     */
    void CommonListView::pushBackCustomItem(cocos2d::ui::Widget* _item) {
        // まだ何もアイテムが追加されていない場合はリストビューのサイズを設定する
        if (pListView_->getItems().size() == 0) {
            itemContentSize_ = _item->getContentSize();

            switch (pListView_->getDirection()) {
                case ui::ScrollView::Direction::HORIZONTAL:
                {
                    float itemWidth = itemContentSize_.width + pListView_->getItemsMargin();
                    float activeItemWidth = (itemWidth * activeItemCount_) - pListView_->getItemsMargin();
                    pListView_->setContentSize(Size(activeItemWidth, itemContentSize_.height));
                }
                    break;
                    
                case ui::ScrollView::Direction::VERTICAL:
                {
                    float itemHeight = itemContentSize_.height + pListView_->getItemsMargin();
                    float activeItemHeight = (itemHeight * activeItemCount_) - pListView_->getItemsMargin();
                    pListView_->setContentSize(Size(itemContentSize_.width, activeItemHeight));
                }
                    break;
                    
                default:
                    break;
            }
        }
        
        pListView_->pushBackCustomItem(_item);
        refreshItemColor();
    }
    
    /**
     *  @brief 指定したインデックスまで移動する
     */
    void CommonListView::jumpToItem(int32_t _itemIndex, bool isWithoutAction) {
        pListView_->getInnerContainer()->stopAllActions();
        
        float originalDuration = magneticDuration_;
        if (isWithoutAction) {
            magneticDuration_ = NO_ACTION_SCROLL_DURATION;
        }
        
        MoveTo* moveAction = nullptr;
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float itemWidth = itemContentSize_.width + pListView_->getItemsMargin();
                float activeItemsWidth = (itemWidth * activeItemCount_) - pListView_->getItemsMargin();
                float offset = (pListView_->getContentSize().width - activeItemsWidth) / 2.0f;
                moveAction = MoveTo::create(magneticDuration_, Vec2(-(_itemIndex) * itemWidth + offset, 0.0f));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float itemHeight = itemContentSize_.height + pListView_->getItemsMargin();
                float activeItemsHeight = (itemHeight * activeItemCount_) - pListView_->getItemsMargin();
                float offset = (pListView_->getContentSize().height - activeItemsHeight) / 2.0f;
                moveAction = MoveTo::create(magneticDuration_, Vec2(0.0f, -(_itemIndex) * itemHeight + offset));
            }
                break;
                
            default:
                moveAction = MoveTo::create(0.0f, Vec2(0.0f, 0.0f));
                break;
        }
        
        pListView_->getInnerContainer()->runAction(Sequence::create({
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
        
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                if (std::fabsf(pListView_->getContentSize().width - pListView_->getInnerContainerSize().width) <= FLT_EPSILON) {
                    // ゼロ除算回避
                    return 100.0f;
                }
                
                percent = 100.0f * (-pListView_->getInnerContainerPosition().x / (pListView_->getInnerContainerSize().width - pListView_->getContentSize().width));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
                percent = this->pListView_->getScrolledPercentVertical();
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
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float x = -(pListView_->getInnerContainerSize().width - pListView_->getContentSize().width) * (_percent / 100.0f);
                pListView_->setInnerContainerPosition(Vec2(x, pListView_->getInnerContainerPosition().y));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float y = -(pListView_->getInnerContainerSize().height - pListView_->getContentSize().height) * (_percent / 100.0f);
                pListView_->setInnerContainerPosition(Vec2(pListView_->getInnerContainerPosition().x, y));
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
        
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
            {
                float newPositionX = pListView_->getInnerContainerPosition().x + _velocity;
                float innerContainerWidth = pListView_->getInnerContainerSize().width - pListView_->getContentSize().width;
                
                // 境界からはみ出ないように座標を丸める
                if (newPositionX <= -(innerContainerWidth + boundaryOutsideRange_)) {
                    newPositionX = -(innerContainerWidth + boundaryOutsideRange_);
                    isOutOfBounds = true;
                }
                else if (newPositionX >= 0.0f + boundaryOutsideRange_) {
                    newPositionX = 0.0f + boundaryOutsideRange_;
                    isOutOfBounds = true;
                }
                
                pListView_->setInnerContainerPosition(Vec2(newPositionX, pListView_->getInnerContainerPosition().y));
            }
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
            {
                float newPositionY = pListView_->getInnerContainerPosition().y + _velocity;
                float innerContainerHeight = pListView_->getInnerContainerSize().height - pListView_->getContentSize().height;

                // 境界からはみ出ないように座標を丸める
                if (newPositionY <= -(innerContainerHeight + boundaryOutsideRange_)) {
                    newPositionY = -(innerContainerHeight + boundaryOutsideRange_);
                    isOutOfBounds = true;
                }
                else if (newPositionY >= 0.0f + boundaryOutsideRange_) {
                    newPositionY = 0.0f + boundaryOutsideRange_;
                    isOutOfBounds = true;
                }
                
                pListView_->setInnerContainerPosition(Vec2(pListView_->getInnerContainerPosition().x, newPositionY));
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
        if (currentIndex > static_cast<int32_t>(pListView_->getItems().size()) - activeItemCount_) {
            currentIndex = static_cast<int32_t>(pListView_->getItems().size()) - activeItemCount_;
        }
        
        switch (pListView_->getDirection()) {
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
        if (pListView_->getItems().empty()) {
            return;
        }
        
        pListView_->removeAllItems();
        
        // インデックスをクリアする意味合いで、コンテナを左端あるいは最上部へ戻す
        switch (pListView_->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
                pListView_->ListView::jumpToLeft();
                break;
                
            case ui::ScrollView::Direction::VERTICAL:
                pListView_->ListView::jumpToTop();
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
        
        pListView_->removeChild(_item);
    }
    
    /**
     *  @brief スクロールする方向を設定
     */
    void CommonListView::setScrollDiretion(ui::ScrollView::Direction _direction) {
        pListView_->setDirection(_direction);
    }
    
    /**
     *  @brief クリッピングを設定
     */
    void CommonListView::setClippingEnabled(bool _enabled) {
        pListView_->setClippingEnabled(_enabled);
    }
    
    /**
     *  @brief コンテナを取得
     */
    ui::Layout* CommonListView::getContainer() const {
        return pListView_->getInnerContainer();
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
        return pListView_->getInnerContainerSize();
    }
    
    /**
     *  @brief レイアウトを更新
     */
    void CommonListView::doLayout() {
        pListView_->doLayout();
    }
    
    /**
     *  @brief スクロール時のアイテムのスケールを更新
     *  @details プロジェクトによっては仕様が合わない可能性あり
     *  @details 座標を調整しないといけなくなるはずなので、以降アイテムの基本座標やサイズを渡すなどして要調整
     */
    void CommonListView::refreshViewItemsScale() {
        int32_t maxIndex = static_cast<int32_t>(pListView_->getItems().size()) - 1;
        if (maxIndex <= 0) {
            return;
        }
        
        for (int32_t index = 0; index <= maxIndex; index++) {
            float onesPercent = SCROLL_MAX_PERCENTAGE / static_cast<float>(maxIndex);
            float itemPercent = (SCROLL_MAX_PERCENTAGE * index) / static_cast<float>(maxIndex);
            float distance = std::min(fabsf(itemPercent - this->getPercentage()), onesPercent);
            float scale = (ITEMS_MAX_SCALE + ((ITEMS_MIN_SCALE - ITEMS_MAX_SCALE) * (distance / onesPercent)));
            auto item = this->getItem(index);
            if (item) {
                item->setScale(scale);
            }
        }
    }
}
