//
//  MyTJPlacementListener.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "MyTJPlacementListener.hpp"

using namespace tapjoy;

/**
 *  @brief プレイスメントのリクエスト成功時のコールバック
 */
void MyTJPlacementListener::onRequestSuccess(TJPlacementHandle _placementHandle, const char* _placementName) {
    if (onRequestSuccessedCallback_) {
        onRequestSuccessedCallback_();
    }
}

/**
 *  @brief プレイスメントのリクエスト失敗時のコールバック
 */
void MyTJPlacementListener::onRequestFailure(TJPlacementHandle _placementHandle, const char* _placementName, int _errorCode, const char* _errorMessage) {
    if (onRequestFailedCallback_) {
        onRequestFailedCallback_();
    }
}

/**
 *  @brief プレイスメントのコンテンツ準備完了時のコールバック
 */
void MyTJPlacementListener::onContentReady(TJPlacementHandle _placementHandle, const char* _placementName) {
    if (TJPlacement::isContentAvailable(_placementHandle) &&
        TJPlacement::isContentReady(_placementHandle)) {
        TJPlacement::showContent(_placementHandle);
    }
    else {
        if (onRequestFailedCallback_) {
            onRequestFailedCallback_();
        }
    }
}

/**
 *  @brief プレイスメントのコンテンツ表示時のコールバック
 */
void MyTJPlacementListener::onContentShow(TJPlacementHandle _placementHandle, const char* _placementName) {
    if (onContentShowCallback_) {
        onContentShowCallback_();
    }
}

/**
 *  @brief プレイスメントのコンテンツ切断時のコールバック
 */
void MyTJPlacementListener::onContentDismiss(TJPlacementHandle _placementHandle, const char* _placementName) {
    if (onContentDismissCallback_) {
        onContentDismissCallback_();
    }
}

/**
 *  @brief プレイスメントの課金時のコールバック
 */
void MyTJPlacementListener::onPurchaseRequest(TJPlacementHandle _placementHandle, const char* _placementName, TJActionRequestHandle _requestHandle, const char* _requestId, const char* _requestToken, const char* _productId) {
    
}

/**
 *  @brief プレイスメントの報酬付与時のコールバック
 */
void MyTJPlacementListener::onRewardRequest(TJPlacementHandle _placementHandle, const char* _placementName, TJActionRequestHandle _requestHandle, const char* _requestId, const char* _requestToken, const char* _itemId, int _quantity) {
    
}

/**
 *  @brief プレイスメントのリクエスト成功時のコールバックを設定
 */
void MyTJPlacementListener::setRequestSuccessedCallback(std::function<void(void)> _callback) {
    onRequestSuccessedCallback_ = _callback;
}

/**
 *  @brief プレイスメントのリクエスト失敗時のコールバックを設定
 */
void MyTJPlacementListener::setRequestFailedCallback(std::function<void(void)> _callback) {
    onRequestFailedCallback_ = _callback;
}

/**
 *  @brief プレイスメントのコンテンツ表示時のコールバックを設定
 */
void MyTJPlacementListener::setContentShowCallback(std::function<void ()> _callback) {
    onContentShowCallback_ = _callback;
}

/**
 *  @brief プレイスメントのコンテンツ切断時のコールバックを設定
 */
void MyTJPlacementListener::setContentDismissedCallback(std::function<void ()> _callback) {
    onContentDismissCallback_ = _callback;
}
