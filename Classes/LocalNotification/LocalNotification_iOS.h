//
//  LocalNotification_iOS.h
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/12/17.
//

#ifndef LocalNotification_iOS_h
#define LocalNotification_iOS_h

#ifdef __APPLE__

#include "LocalNotificationBase.hpp"

class LocalNotification_iOS : public LocalNotificationBase {
private:
    bool enabled_{false};
    
public:
    LocalNotification_iOS();
    ~LocalNotification_iOS();
    
    bool init() override;
    void setLocalNotificationSchedule(const std::string& _message, int32_t intervalSeconds, int32_t _tag) override;
    void setEnabled(bool _enabled, bool _jumpToNativeSettings = false) override;
    bool isEnabled() override;
    void cancelLocalNotification(int32_t _tag) override;
    void cancelAllLocalNotification() override;
};

#endif

#endif /* LocalNotification_iOS_h */
