//
//  CommonListView.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/28.
//

#ifndef CommonListView_hpp
#define CommonListView_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace project {
    /**
     *  @brief カスタムリストビュー
     */
    class CommonListView : public cocos2d::ui::Layout {
    private:
        float inertiaStlength_{0.9f};           // 慣性
        float boundaryOutsideRange_{100.0f};    // はみだす範囲
        float velocityEndInertia_{2.0f};        // 慣性が止まる速度
        float magneticDuration_{0.2f};          // 吸い付きの値
        cocos2d::ui::ListView* pListView_{nullptr};
        float containerVelocity_{0.0f};
        cocos2d::Size itemContentSize_{0.0f, 0.0f};
        int32_t activeItemCount_{0};
        size_t itemIndex_{0};
        cocos2d::Vec2 moveDiffPos_{0.0f, 0.0f};
        std::function<void(float)> onScrolledItem_{nullptr};
        std::function<void(size_t)> onChangedItemIndex_{nullptr};
        std::function<void(void)> onScrollBegan_{nullptr};
        std::function<void(void)> onScrollEnded_{nullptr};
        
    private:
        bool init(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin);
        void initSettings(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin);
        virtual bool onTouchBegan(cocos2d::Touch* _touch, cocos2d::Event* _event);
        virtual void onTouchMoved(cocos2d::Touch* _touch, cocos2d::Event* _event);
        virtual void onTouchEnded(cocos2d::Touch* _touch, cocos2d::Event* _event);
        
    protected:
        CommonListView() = default;
        virtual ~CommonListView() = default;
        
        bool moveContainer(float _velocity);
        void inertia(float _delta);
        
    public:
        static CommonListView* create(const cocos2d::Size& _contentSize, int32_t _activeItemCount, float _itemMargin);
        
        void setInertiaStlength(float _stlength);
        void setBoundaryOutsideRange(float _range);
        void setVelocityEndInertia(float _velocity);
        void setMagneticDuration(float _duration);
        int32_t getCurrentHeadItemIndex();
        bool isLeftSide();
        bool isRightSide();
        const cocos2d::Vector<cocos2d::ui::Widget*>& getItems();
        cocos2d::ui::Widget* getItem(int32_t _itemIndex);
        void pushBackCustomItem(cocos2d::ui::Widget* _item);
        void jumpToItem(int32_t _itemIndex, bool _isWithoutAction = false);
        void jumpToCurrentIndex();
        bool isScrolling();
        void setScrolledItemCallback(std::function<void(float)> _callback){ onScrolledItem_ = _callback; };
        void setChangedIndexCallback(std::function<void(size_t)> _callback){ onChangedItemIndex_ = _callback; };
        void setScrollBeganCallback(std::function<void(void)> _callback){ onScrollBegan_ = _callback; };
        void setScrollEndedCallback(std::function<void(void)> _callback){ onScrollEnded_ = _callback; };
        inline int32_t getActiveItemCount() const { return activeItemCount_; }
        float getPercentage();
        void setPercentage(int32_t _percent);
        void refreshItemColor();
        void removeAllItemsInList();
        void removeItem(cocos2d::Node* _item);
        void setScrollDiretion(cocos2d::ui::ScrollView::Direction _direction);
        void setClippingEnabled(bool _enabled);
        cocos2d::ui::Layout* getContainer() const;
        void suspendScrolling();
        void resumeScrolling();
        inline cocos2d::Vec2 getMoveDiffPos() const { return moveDiffPos_; }
        cocos2d::Size getInnerContainerSize() const;
        void doLayout();
        void refreshViewItemsScale();
    };
}

#endif /* CommonListView_hpp */
