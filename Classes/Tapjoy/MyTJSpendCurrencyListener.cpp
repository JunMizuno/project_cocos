//
//  MyTJSpendCurrencyListener.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "MyTJSpendCurrencyListener.hpp"

/**
 *  @brief クライアント側アイテム消費の通達成功時のコールバック
 */
void MyTJSpendCurrencyListener::onSpendCurrencyResponse(const char* _currencyName, int _balance) {
    
}

/**
 *  @brief クライアント側アイテム消費の通達失敗時のコールバック
 */
void MyTJSpendCurrencyListener::onSpendCurrencyResponseFailure(const char* _error) {
    
}

/**
 *  @brief アイテム消費通達成功時のコールバックを設定
 */
void MyTJSpendCurrencyListener::setSpendCurrencyResponseSuccessedCallback(std::function<void(std::string, int32_t)> _callback) {
    onSpendCurrencyResponseSuccessedCallback_ = _callback;
}

/**
 *  @brief アイテム消費通達失敗時のコールバックを設定
 */
void MyTJSpendCurrencyListener::setSpendCurrencyResponseFailedCallback(std::function<void(void)> _callback) {
    onSpendCurrencyResponseFailedCallback_ = _callback;
}
