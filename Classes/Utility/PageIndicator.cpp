//
//  PageIndicator.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#include "PageIndicator.hpp"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace project {
    namespace {
        // @todo. ダミー、使用時は要差し替え
        constexpr char NORMAL_IMAGE_FILE[] = "CloseNormal.png";
        constexpr char SELECTED_IMAGE_FILE[] = "CloseSelected.png";
        // @todo. スライダー用ダミー、差し替え必要？？
        constexpr char SLIDER_IMAGE_DUMMY_FILE[] = "dummy.png";
        constexpr float PAGE_INDICATOR_SPACE = 20.0f;
        constexpr float PAGE_INDICATOR_OFFSET_WIDTH = 40.0f;
        constexpr float PAGE_INDICATOR_SIZE_HEIGHT = 40.0f;
    }
    
    /**
     *  @brief 生成
     */
    PageIndicator* PageIndicator::create(int32_t _pageCount) {
        PageIndicator* pRet = new (std::nothrow) PageIndicator();
        
        if (pRet) {
            if (pRet->init(_pageCount)) {
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
    bool PageIndicator::init(int32_t _pageCount) {
        if (!Node::init()) {
            return false;
        }
        
        pageCount_ = _pageCount;
        
        initSettings();
        
        return true;
    }
    
    /**
     *  @brief 初期設定
     */
    void PageIndicator::initSettings() {
        createMenu();
        createSlider();
        setPage(0);
    }
    
    /**
     *  @brief メニューを生成
     */
    void PageIndicator::createMenu() {
        Vector<MenuItem*> menuItemVector;
        for (int32_t i = 0; i < pageCount_; i++) {
            // @todo. 以下、画像差し替え時は要処理変更
            Sprite* normalImage = Sprite::create(NORMAL_IMAGE_FILE);
            Sprite* selectedImage = Sprite::create(SELECTED_IMAGE_FILE);
            
            MenuItemSprite* menuImage = MenuItemSprite::create(normalImage, selectedImage);
            menuImage->setTag(i);
            menuItemVector.pushBack(menuImage);
        }
        
        menu_ = Menu::createWithArray(menuItemVector);
        // @todo. 縦の場合は変更が必要、プロジェクトによって切り替えが発生するはず
        menu_->alignItemsHorizontallyWithPadding(PAGE_INDICATOR_SPACE);
        menu_->setPosition(Vec2::ZERO);
        this->addChild(menu_);
    }
    
    /**
     *  @brief スライダーを生成
     */
    void PageIndicator::createSlider() {
        ui::Slider* slider = ui::Slider::create();
        slider->loadBarTexture(SLIDER_IMAGE_DUMMY_FILE, ui::Button::TextureResType::PLIST);
        slider->setMaxPercent(pageCount_);
        slider->setScale9Enabled(true);
        slider->setContentSize(Size((PAGE_INDICATOR_SPACE * pageCount_) + PAGE_INDICATOR_OFFSET_WIDTH, PAGE_INDICATOR_SIZE_HEIGHT));
        slider->addEventListener([this](Ref* _node, ui::Slider::EventType _eventType) {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(_node);
            if (slider) {
                int32_t percent = (slider->getPercent() >= pageCount_) ? pageCount_ : slider->getPercent() + 1;
                if (this->setPage(percent)) {
                    if (onChangePage_) {
                        onChangePage_(currentPageIndex_);
                    }
                }
            }
        });
        this->addChild(slider);
    }
    
    /**
     *  @brief ページの設定
     */
    bool PageIndicator::setPage(int32_t _page) {
        if (_page == currentPageIndex_) {
            return false;
        }
        
        if (!menu_) {
            return false;
        }
        
        // 前回選択のページを非表示にする
        MenuItemSprite* previousItemSprite = menu_->getChildByTag<MenuItemSprite*>(currentPageIndex_);
        if (previousItemSprite) {
            previousItemSprite->unselected();
        }
        
        // 今回選択のページを有効にする
        MenuItemSprite* currentItemSprite = menu_->getChildByTag<MenuItemSprite*>(_page);
        if (currentItemSprite) {
            currentItemSprite->selected();
        }

        currentPageIndex_ = _page;
        
        return true;
    }
    
    /**
     *  @brief ページの選択
     */
    bool PageIndicator::setPageIndex(int32_t _pageIndex) {
        return setPage(_pageIndex);
    }
}
