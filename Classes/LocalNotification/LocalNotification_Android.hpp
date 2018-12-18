//
//  LocalNotification_Android.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/12/18.
//

#ifndef LocalNotification_Android_hpp
#define LocalNotification_Android_hpp

#ifndef __APPLE__

#include "LocalNotificationBase.hpp"
#include "cocos2d.h"

class LocalNotification_Android : public LocalNotificationBase {
public:
    LocalNotification_Android();
    ~LocalNotification_Android();
    
    bool init() override;
    void setLocalNotificationSchedule(const std::string& _message, int32_t _intervalSeconds, int32_t _tag) override;
    void setEnabled(bool _enabled, bool _jumpToNativeSettings) override;
    bool isEnabled() override;
    void cancelLocalNotification(int32_t _tag) override;
    void cancelAllLocalNotification() override;
};

#endif

#endif /* LocalNotification_Android_hpp */
