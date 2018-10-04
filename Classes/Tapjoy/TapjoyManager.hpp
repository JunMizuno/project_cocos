//
//  TapjoyManager.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef TapjoyManager_hpp
#define TapjoyManager_hpp

#include "cocos2d.h"
#include "Tapjoy/TapjoyCpp.h"
#include "MyTJConnectListener.hpp"
#include "MyTJPlacementListener.hpp"
#include "MyTJGetCurrencyBalanceListener.hpp"
#include "MyTJSpendCurrencyListener.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_setTapjoyActivity(JNIEnv* _env, jobject _obj, jobject _thiz);
#ifdef __cplusplus
}
#endif
#endif

/**
 *  @brief TapjoyManager
 *  @details C++での実装例
 */
class TapjoyManager {
private:
    MyTJConnectListener connectListener_;
    MyTJPlacementListener placementListener_;
    MyTJGetCurrencyBalanceListener getCurrencyBalanceListener_;
    MyTJSpendCurrencyListener spendCurrencyListener_;
    tapjoy::TJPlacementHandle placementHandle_;
    
public:
    TapjoyManager();
    ~TapjoyManager();
    
    static TapjoyManager* getInstance();
    
    void init();
    void requestPlacement(const std::string& _placementKey);
    bool checkConnectTapjoy();
    void getCurrencyBalance();
    void spendCurrency(int32_t _balance);
    void dismissPlacementContent();
    void releasePlacementContent();
    bool checkContentIsReady();
    bool checkContentIsAvailable();
    
    void setPlacementRequestSuccessedCallback(std::function<void(void)> _callback);
    void setPlacementRequestFailedCallback(std::function<void(void)> _callback);
    void setPlacementContentShowCallback(std::function<void(void)> _callback);
    void setPlacementContentDismissedCallback(std::function<void(void)> _callback);
    void setCurrencyBalanceSuccessedCallback(std::function<void(std::string, int32_t)> _callback);
    void setSpendCurrencySuccessedCallback(std::function<void(std::string, int32_t)> _callback);
    void clearAllCallback();
};

#endif /* TapjoyManager_hpp */
