//
//  PageIndicator.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#ifndef PageIndicator_hpp
#define PageIndicator_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

namespace project {
    /**
     *  @brief ページインジケーター
     */
    class PageIndicator : public cocos2d::Node {
    private:
        cocos2d::Menu* pMenu_{nullptr};
        int32_t pageCount_{0};
        int32_t currentPageIndex_{0};
        
        std::function<void(int32_t)> onChangePage_{nullptr};
        
    private:
        PageIndicator() = default;
        virtual ~PageIndicator() = default;
        
        bool init(int32_t _pageCount);
        void initSettings();
        void createMenu();
        void createSlider();
        bool setPage(int32_t _page);
        
    public:
        static PageIndicator* create(int32_t _pageCount);
        
        bool setPageIndex(int32_t _pageIndex);
        void setChangePageCallback(std::function<void(int32_t)> _callback) { onChangePage_ = _callback; }
    };
}

#endif /* PageIndicator_hpp */
