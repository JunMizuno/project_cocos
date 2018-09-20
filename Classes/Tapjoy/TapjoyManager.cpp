//
//  TapjoyManager.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#include "TapjoyManager.hpp"

using namespace tapjoy;
USING_NS_CC;

static TapjoyManager* gSaredInstance_ = nullptr;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
std::string TAPJOY_CONNECT_KEY = "";
#elif
#include "platform/android/jni/JniHelper.h"

std::string TAPJOY_CONNECT_KEY = "";

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_setTapjoyActivity(JNIEnv* _env, jobject _obj, jobject _thiz) {
    JavaVM* jvm;
    _env->GetJavaVM(&jvm);
    Tapjoy::setJavaVM(jvm);
    
    jobject context = _env->NewGlobalRef(_thiz);
    Tapjoy::setContext(context);
}
#endif

TapjoyManager::TapjoyManager() {
    
}

TapjoyManager::~TapjoyManager() {
    CC_SAFE_DELETE(gSaredInstance_);
}

TapjoyManager* TapjoyManager::getInstance() {
    if (!gSaredInstance_) {
        gSaredInstance_ = new (std::nothrow)TapjoyManager();
    }
    
    return gSaredInstance_;
}

/**
 *  @brief 初期化
 */
void TapjoyManager::init() {
#if RELEASE_BUILD
    Tapjoy::setDebugEnabled(false);
#else
    Tapjoy::setDebugEnabled(true);
#endif
    
    Tapjoy::connect(TAPJOY_CONNECT_KEY.c_str(), &connectListener_);
}

/**
 *  @brief プレイスメントをリクエストして取得する
 */
void TapjoyManager::requestPlacement(const std::string& _placementKey) {
    placementHandle_ = TJPlacement::create(_placementKey.c_str(), &placementListener_);
    TJPlacement::requestContent(placementHandle_);
}

/**
 *  @brief Tapjoyに接続済かどうか
 */
bool TapjoyManager::checkConnectTapjoy() {
    return Tapjoy::isConnected();
}

/**
 *  @brief アイテム在庫を確認する
 */
void TapjoyManager::getCurrencyBalance() {
    Tapjoy::getCurrencyBalance(&getCurrencyBalanceListener_);
}

/**
 *  @brief アイテム付与したことを通達する
 */
void TapjoyManager::spendCurrency(int32_t _balance) {
    Tapjoy::spendCurrency(_balance, &spendCurrencyListener_);
}

/**
 *  @brief 現在のプレイスメントを切断する
 */
void TapjoyManager::dismissPlacementContent() {
    TJPlacement::dismissContent();
}

/**
 *  @brief 現在のプレイスメントを解放する
 */
void TapjoyManager::releasePlacementContent() {
    TJPlacement::release(placementHandle_);
}

/**
 *  @brief コンテンツの準備が完了したかどうか
 */
bool TapjoyManager::checkContentIsReady() {
    return TJPlacement::isContentReady(placementHandle_);
}

/**
 *  @brief コンテンツが利用できるかどうか
 */
bool TapjoyManager::checkContentIsAvailable() {
    return TJPlacement::isContentAvailable(placementHandle_);
}

void TapjoyManager::setPlacementRequestSuccessedCallback(std::function<void(void)> _callback) {
    placementListener_.setRequestSuccessedCallback(_callback);
}

void TapjoyManager::setPlacementRequestFailedCallback(std::function<void(void)> _callback) {
    placementListener_.setRequestFailedCallback(_callback);
}

void TapjoyManager::setPlacementContentShowCallback(std::function<void(void)> _callback) {
    placementListener_.setContentShowCallback(_callback);
}

void TapjoyManager::setPlacementContentDismissedCallback(std::function<void(void)> _callback) {
    placementListener_.setContentDismissedCallback(_callback);
}

void TapjoyManager::setCurrencyBalanceSuccessedCallback(std::function<void(std::string, int32_t)> _callback) {
    getCurrencyBalanceListener_.setCurrencyResponseSuccessedCallback(_callback);
}

void TapjoyManager::setSpendCurrencySuccessedCallback(std::function<void(std::string, int32_t)> _callback) {
    spendCurrencyListener_.setSpendCurrencyResponseSuccessedCallback(_callback);
}

/**
 *  @ brief 全てのコールバックを初期化
 */
void TapjoyManager::clearAllCallback() {
    setPlacementRequestFailedCallback(nullptr);
    setPlacementRequestFailedCallback(nullptr);
    setPlacementContentShowCallback(nullptr);
    setPlacementContentDismissedCallback(nullptr);
    setCurrencyBalanceSuccessedCallback(nullptr);
    setSpendCurrencySuccessedCallback(nullptr);
}
