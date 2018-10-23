//
//  Utility_NDK.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/23.
//

#ifndef Utility_NDK_hpp
#define Utility_NDK_hpp

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "cocos2d.h"

#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initNDK(JNIEnv* _env, jobject _obj, jobject _thiz);
#ifdef __cplusplus
}
#endif

template <class T>
class JObj {
private:
    JNIEnv* pEnv_;
    T pObj_;
    
public:
    JObj(JNIEnv* _env) : pEnv_(_env), pObj_{nullptr}{}
    JObj(JNIEnv* _env, T _obj) : pEnv_(_env), pObj_(_obj){}
    ~JObj() {
        if (pEnv_ != nullptr && pObj_ != nullptr) {
            pEnv_->DeleteLocalRef(pObj_);
        }
    }
    
    inline T get() { return pObj_; }
    
    inline void operator = (T _obj){ pObj_ = _obj; }
    
    inline T operator -> () { return pObj_; }
}

class Utility_NDK {
private:
    JNIEnv* pEnv_{nullptr};
    jobject obj_;
    jobject thiz_;
    
public:
    Utility_NDK();
    ~Utility_NDK();
    
    static Utility_NDK* getInstance();
    
    void setNDKInstance(JNIEnv* _env, jobject _obj, jobject _thiz);
    
    JNIEnv* getJNIEnv();
    jobject getObj();
    jobject getThiz();
    
    void initialize(JNIEnv* _env);
    jclass loadClass(JNIEnv* _env, const char* _classSigneture);
    jclass findClass(const char* _name);
    jmethodId loadMethoad(JNIEnv* _env, jclass _clazz, const char* _methodName, const char* _methodSigneture, bool _isStatic);
}

#endif /* __APPLE__ */
#endif /* Utility_NDK_hpp */
