//
//  URLSchemeEvent.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2019/07/02.
//

#include "URLSchemeEvent.hpp"

static URLSchemeEvent* gSharedInstance_ = nullptr;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
static const std::string helperClassName = "org/cocos2dx/cpp/URLSchemeEvent";
#endif

USING_NS_CC;

URLSchemeEvent::URLSchemeEvent() {
    
}

URLSchemeEvent::~URLSchemeEvent() {
    CC_SAFE_DELETE(gSharedInstance_);
}

URLSchemeEvent* URLSchemeEvent::getInstance() {
    if (!gSharedInstance_) {
        gSharedInstance_ = new URLSchemeEvent();
    }
    
    return gSharedInstance_;
}

/**
 *  @brief URLスキームのアドレスを設定
 */
void URLSchemeEvent::setSchemeURL(const std::string& url) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "setSchemeURL", "(Ljava/lang/String;)V")) {
        JNIEnv* env = JniHelper::getEnv();
        jstring jmessage = env->NewStringUTF(url.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jmessage);
        env->DeleteLocalRef(jmessage);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    return;
#endif
    
    schemeURL_ = url;
}

/**
 *  @brief URLスキームのアドレスを取得
 */
std::string URLSchemeEvent::getSchemeURL() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "getSchemeURL", "()Ljava/lang/String;")) {
        JNIEnv* env = JniHelper::getEnv();
        jobject objResult = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        schemeURL_ = JniHelper::jstring2string((jstring)objResult);
        env->DeleteLocalRef(objResult);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
#endif
    
    return schemeURL_;
}
