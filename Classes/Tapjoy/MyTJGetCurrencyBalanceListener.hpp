//
//  MyTJGetCurrencyBalanceListener.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef MyTJGetCurrencyBalanceListener_hpp
#define MyTJGetCurrencyBalanceListener_hpp

#include "cocos2d.h"
#include "Tapjoy/TapjoyCpp.h"

class MyTJGetCurrencyBalanceListener : public tapjoy::TJGetCurrencyBalanceListener {
private:
    std::function<void(std::string, int32_t)> onCurrencyResponseSuccessedCallback_{nullptr};
    std::function<void(void)> onCurrencyResponseFailedCallback_{nullptr};
    
public:
    virtual void onGetCurrencyBalanceResponse(const char* _currencyName, int _balance) override;
    virtual void onGetCurrencyBalanceResponseFailure(const char* _error) override;

    void setCurrencyResponseSuccessedCallback(std::function<void(std::string, int32_t)> _callback);
    void setCurrencyResponseFailedCallback(std::function<void(void)> _callback);
};

#endif /* MyTJGetCurrencyBalanceListener_hpp */
