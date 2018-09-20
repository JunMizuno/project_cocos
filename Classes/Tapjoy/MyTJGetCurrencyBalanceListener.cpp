//
//  MyTJGetCurrencyBalanceListener.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "MyTJGetCurrencyBalanceListener.hpp"

/**
 *  @brief サーバー上のアイテム在庫の取得成功時
 */
void MyTJGetCurrencyBalanceListener::onGetCurrencyBalanceResponse(const char* _currencyName, int _balance) {
    if (onCurrencyResponseSuccessedCallback_) {
        onCurrencyResponseSuccessedCallback_(std::string(_currencyName), static_cast<int32_t>(_balance));
    }
}

/**
 *  @brief サーバー上のアイテム在庫の取得失敗時
 */
void MyTJGetCurrencyBalanceListener::onGetCurrencyBalanceResponseFailure(const char* _error) {
    if (onCurrencyResponseFailedCallback_) {
        onCurrencyResponseFailedCallback_();
    }
}

/**
 *  @brief サーバー上のアイテム在庫の取得成功時のコールバック
 */
void MyTJGetCurrencyBalanceListener::setCurrencyResponseSuccessedCallback(std::function<void(std::string, int32_t)> _callback) {
    onCurrencyResponseSuccessedCallback_ = _callback;
}

/**
 *  @brief サーバー上のアイテム在庫の取得失敗時のコールバック
 */
void MyTJGetCurrencyBalanceListener::setCurrencyResponseFailedCallback(std::function<void(void)> _callback) {
    onCurrencyResponseFailedCallback_ = _callback;
}
