//
//  MyTJSpendCurrencyListener.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef MyTJSpendCurrencyListener_hpp
#define MyTJSpendCurrencyListener_hpp

#include "cocos2d.h"
#include "Tapjoy/TapjoyCpp.h"

class MyTJSpendCurrencyListener : public tapjoy::TJSpendCurrencyListener {
private:
    std::function<void(std::string, int32_t)> onSpendCurrencyResponseSuccessedCallback_{nullptr};
    std::function<void(void)> onSpendCurrencyResponseFailedCallback_{nullptr};

public:
    virtual void onSpendCurrencyResponse(const char* _currencyName, int _balance) override;
    virtual void onSpendCurrencyResponseFailure(const char* _ßerror) override;
    
    void setSpendCurrencyResponseSuccessedCallback(std::function<void(std::string, int32_t)> _callback);
    void setSpendCurrencyResponseFailedCallback(std::function<void(void)> _callback);
};

#endif /* MyTJSpendCurrencyListener_hpp */
