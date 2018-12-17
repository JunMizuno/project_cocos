//
//  LocalNotificationBase.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/12/17.
//

#ifndef LocalNotificationBase_hpp
#define LocalNotificationBase_hpp

/**
 *  @brief ローカル通知クラス
 *  @details 呼び出し元となる
 */
class LocalNotificationBase {
public:
    static LocalNotificationBase* gSharedLocalNotification_;
    
public:
    LocalNotificationBase();
    virtual ~LocalNotificationBase() = default;
    
    static LocalNotificationBase* getInstance();
    
    virtual bool init() = 0;
    virtual void setLocalNotificationSchedule(const std::string& _message, int32_t _intervalSeconds, int32_t _tag) = 0;
    virtual void setEnabled(bool _enabled, bool _jumpToNativeSettings = false) = 0;
    virtual bool isEnabled() = 0;
    virtual void cancelLocalNotification(int32_t _tag) = 0;
    virtual void cancelAllLocalNotification() = 0;
};

#endif /* LocalNotificationBase_hpp */
