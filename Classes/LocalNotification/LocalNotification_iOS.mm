//
//  LocalNotification_iOS.mm
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/12/17.
//

#ifdef __APPLE__

#import "LocalNotification_iOS.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

USING_NS_CC;

static NSString* const KEY = @"enableNotification";

LocalNotificationBase* LocalNotificationBase::getInstance() {
    if (!gSharedLocalNotification_) {
        gSharedLocalNotification_ = new (std::nothrow) LocalNotification_iOS();
        
        if (!gSharedLocalNotification_->init()) {
            delete gSharedLocalNotification_;
            gSharedLocalNotification_ = nullptr;
            return nullptr;
        }
    }
    
    return gSharedLocalNotification_;
}

LocalNotification_iOS::LocalNotification_iOS() {
    
}

LocalNotification_iOS::~LocalNotification_iOS() {
    CC_SAFE_DELETE(gSharedLocalNotification_);
}

bool LocalNotification_iOS::init() {
    UIUserNotificationType types = UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert;
    UIUserNotificationSettings* settings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
    [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    enabled_ = [[NSUserDefaults standardUserDefaults] boolForKey:KEY];
    
    return true;
}

void LocalNotification_iOS::setLocalNotificationSchedule(const std::string& _message, int32_t intervalSeconds, int32_t _tag) {
    if (!isEnabled()) {
        return;
    }
    
    UILocalNotification* notification = [[UILocalNotification alloc] init];
    notification.fireDate = [[NSDate date] dateByAddingTimeInterval:intervalSeconds];
    notification.timeZone = [NSTimeZone defaultTimeZone];
    notification.alertBody = [NSString stringWithUTF8String:_message.c_str()];
    notification.alertAction = @"Open";
    notification.soundName = UILocalNotificationDefaultSoundName;
    NSNumber* nsTag = [NSNumber numberWithInteger:_tag];
    NSDictionary* infoDictionary = [NSDictionary dictionaryWithObject:nsTag forKey:@"ID"];
    notification.userInfo = infoDictionary;
    
    // 通知を登録
    // @memo. 当然ながらアプリ立ち上げ中や通知オフにしている場合は有効にならない(蓄積はされる模様)
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
}

void LocalNotification_iOS::setEnabled(bool _enabled, bool _jumpToNativeSettings) {
    [[NSUserDefaults standardUserDefaults] setBool:_enabled forKey:KEY];
    enabled_ = _enabled;
    
    if (!enabled_) {
        cancelAllLocalNotification();
    }
    
    // OSの設定画面を開く場合
    // @memo. [[UIApplication sharedApplication] isRegisteredForRemotoNotifications]はAppDelegateで呼ばないと正しい値が返らないらしい
    if (enabled_ && _jumpToNativeSettings) {
        NSURL* url = [NSURL URLWithString:UIApplicationOpenSettingsURLString];
        [[UIApplication sharedApplication] openURL:url];
    }
}

bool LocalNotification_iOS::isEnabled() {
    return enabled_;
}

void LocalNotification_iOS::cancelLocalNotification(int32_t _tag) {
    for (UILocalNotification* notification in [[UIApplication sharedApplication] scheduledLocalNotifications]) {
        if ([[notification.userInfo objectForKey:@"ID"] integerValue] == _tag) {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

void LocalNotification_iOS::cancelAllLocalNotification() {
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

#endif
