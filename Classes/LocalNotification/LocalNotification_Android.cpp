//
//  LocalNotification_Android.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/12/18.
//

#ifndef __APPLE__

#include "LocalNotification_Android.hpp"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

static const std::string helperClassName = "org/cocos2dx/cpp/LocalNotification";

LocalNotificationBase* LocalNotificationBase::getInstance() {
    if (!gSharedLocalNotification_) {
        gSharedLocalNotification_ = new (std::nothrow) LocalNotification_Android();
        if (!gSharedLocalNotification_->init()) {
            delete gSharedLocalNotification_;
            gSharedLocalNotification_ = nullptr;
            return nullptr;
        }
    }
    
    return gSharedLocalNotification_;
}

LocalNotification_Android::LocalNotification_Android() {
    
}

LocalNotification_Android::~LocalNotification_Android() {
    CC_SAFE_DELETE(gSharedLocalNotification_);
}

bool LocalNotification_Android::init() {
    JniHelper::callStaticVoidMethod(helperClassName.c_str(), "init");
    
    return true;
}

void LocalNotification_Android::setLocalNotificationSchedule(const std::string& _message, int32_t _intervalSeconds, int32_t _tag) {
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "setLocalNotificationSchedule", "(ILjava/lang/String;I)V")) {
        JNIEnv* env = JniHelper::getEnv();
        jstring jmessage = env->NewStringUTF(_message.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, _intervalSeconds, jmessage, _tag);
        env->DeleteLocalRef(jmessage);
    }
}

void LocalNotification_Android::setEnabled(bool _enabled, bool _jumpToNativeSettings) {
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "setEnabled", "(Z)V")) {
        JNIEnv* env = JniHelper::getEnv();
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, _enabled);
    }
}

bool LocalNotification_Android::isEnabled() {
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "isEnabled", "()Z")) {
        JNIEnv* env = JniHelper::getEnv();
        jboolean jbool = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return (jbool == JNI_TRUE);
    }
    else {
        return false;
    }
}

void LocalNotification_Android::cancelLocalNotification(int32_t _tag) {
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, helperClassName.c_str(), "cancelLocalNotification", "(I)V")) {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, _tag);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void LocalNotification_Android::cancelAllLocalNotification() {
    
}

#endif
