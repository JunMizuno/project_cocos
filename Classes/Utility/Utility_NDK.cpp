//
//  Utility_NDK.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/23.
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "Utility_NDK.hpp"
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <sys/stat.h>
#include <vector>
#include <utility>
#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initNDK(JNIEnv* _env, jobject _obj, jobject _thiz) {
    Utility_NDK::getInstance()->setNDKInstance(_env, _obj, _thiz);
}

static Utility_NDK* gSaredInstance_ = nullptr;
static JavaVM* gJavaVM_ = nullptr;
static jobject gClassLoader_ = nullptr;
static jmethodId gFindClassMethod = nullptr;

Utility_NDK* Utility_NDK::getInstance() {
    if (!gSaredInstance_) {
        gSaredInstance_ = new Utility_NDK();
    }
    
    return gSaredInstance_;
}

Utility_NDK::Utility_NDK() {
    
}

Utility_NDK::~Utility_NDK() {
    CC_SAFE_DELETE(gSaredInstance_);
}

void Utility_NDK::setNDKInstance(JNIEnv* _env, jobject _obj, jobject _thiz) {
    env_ = _env;
    obj_ = _obj;
    thiz_ = _thiz;
    initialize(env_);
}

JNIEnv* Utility_NDK::getJNIEnv() {
    return env_;
}

jobject Utility_NDK::getObj() {
    return obj_;
}

jobject Utility_NDK::getThiz() {
    return thiz_;
}

void Utility_NDK::initialize(JNIEnv* _env) {
    _env->GetJavaVM(&gJavaVM_);
    
    auto randomClass = _env->FindClass("org/cocos2dx/cpp/AppActivity");
    jclass classClass = _env->GetObjectClass(randomClass);
    auto classLoaderClass = _env->FindClass("java/lang/ClassLoader");
    auto getClassLoaderMethod = _env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader");
    jobject classLoader = _env->CallObjectMethod(randomClass, getClassLoaderMethod);
    gFindClassMethod = _env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    gClassLoader_ = _env->NewGlobalRef(classLoader);
}

jclass Utility_NDK::loadClass(JNIEnv* _env, const char* _classSigneture) {
    //jclass result = _env->FindClass(_classSigneture);
    jclass result = findClass(_classSigneture);
    
    if (!result) {
        CCLOG("error findClass(%s)", _classSigneture);
    }
    assert(result != nullptr);
    
    return result;
}

jclass Utility_NDK::findClass(const char* _name) {
    JObj<jstring> nameStr(env_);
    nameStr = env_->NewStringUTF(_name);
    //return static_cast<jclass>(env_->CallObjectMethod(gClassLoader, gFindClassMethod, _name));
    return static_cast<jclass>(env_->CallObjectMethod(gClassLoader, gFindClassMethod, nameStr.get());
}

jmethodId Utility_NDK::loadMethod(JNIEnv* _env, jclass _clazz, const char* _methodName, const char* _methodSigneture, bool _isStatic) {
    jmethodId result = nullptr;
    
    if (isStatic) {
        result = _env->GetStaticMethodID(_clazz, _methodName, _methodSigneture);
    }
    else {
        result = _env->GetMethodID(_clazz, _methodName, _methodSigneture);
    }
    
    if (!result) {
        CCLOG("error GetMethod(%s(%s))", _methodName, _methodSigneture);
    }
    assert(result != nullptr);
    
    return result;
}

#endif
