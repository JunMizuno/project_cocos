//
//  MyTJPlacementListener.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef MyTJPlacementListener_hpp
#define MyTJPlacementListener_hpp

#include "cocos2d.h"
#include "Tapjoy/TapjoyCpp.h"

class MyTJPlacementListener : public tapjoy::TJPlacementListener {
private:
    std::function<void(void)> onRequestSuccessedCallback_{nullptr};
    std::function<void(void)> onRequestFailedCallback_{nullptr};
    std::function<void(void)> onContentShowCallback_{nullptr};
    std::function<void(void)> onContentDismissCallback_{nullptr};
    
public:
    virtual void onRequestSuccess(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName);
    virtual void onRequestFailure(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName, int _errorCode, const char* _errorMessage);
    virtual void onContentReady(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName);
    virtual void onContentShow(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName);
    virtual void onContentDismiss(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName);
    virtual void onPurchaseRequest(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName, tapjoy::TJActionRequestHandle _requestHandle, const char* _requestId, const char* _requestToken, const char* _productId);
    virtual void onRewardRequest(tapjoy::TJPlacementHandle _placementHandle, const char* _placementName, tapjoy::TJActionRequestHandle _requestHandle, const char* _requestId, const char* _requestToken, const char* _itemId, int _quantity);

    void setRequestSuccessedCallback(std::function<void(void)> _callback);
    void setRequestFailedCallback(std::function<void(void)> _callback);
    void setContentShowCallback(std::function<void(void)> _callback);
    void setContentDismissedCallback(std::function<void(void)> _callback);
};

#endif /* MyTJPlacementListener_hpp */
