package org.cocos2dx.cpp;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.util.Log;
import java.util.Log;
import java.util.Calendar;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.Context;

public class LocalNotification {
    private static final String TAG = LocalNotification.class.getSimpleName();
    private static final String KEY = "enableNotification";
    private static boolean init_ = false;
    private static boolean enabled_ = false;
    private static SharedPreferences pref_;
    private static Context context_;
    private static AlarmManager alarmManager_;
    private static Activity activity_;

    public static void init() {
        if (init_) {
            return;
        }

        init_ = true;
        activity_ = Cocos2dxHelper.getActivity();
        context_ = activity_.getApplicationContext();
        pref_ = context_.getSharedPreferences(TAG, Context.MODE_PRIVATE);
        enabled_ = pref_.getBoolean(KEY, true);
        alarmmanager_ = (AlarmManager)activity_.getSystemService(Context.ALARM_SERVICE);
    }

    public static void setLocalNotificationSchedule(int _intervalSeconds, String _message, int _tag) {
        if (!enabled_) {
            return;
        }

        PendingIntent sender = getPendingIntent(_message, _tag);

        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.add(Calendar.SECOND, _intervalSeconds);
        alarmManager_.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
    }

    public static void setEnabled(boolean _enabled) {
        pref_.edit().putBoolean(KEY, _enabled).commit();
        enabled_ = _enabled;

        if (!enabled_) {
            Intent intent = new Intent(context_, LocalNotificationReceiver.class);
            PendingIntent pendingIntent = PendingIntent.getBroadcast(context_, 0, intent, 0);
            pendingIntent.cancel();
            alarmManager_.cancel(pendingIntent);
        }
    }

    public static boolean isEnabled() {
        return enabled_;
    }

    public static void cancelLocalNotification(int _tag) {
        PendingIntent sender = getPendingIntent(null, _tag);
        alarmManager_.cancel(sender);
    }

    private static PendingIntent getPendingIntent(String _message, int _tag) {
        Activity activity = Cocos2dxHelper.getActivity();
        Intent intent = new Intent(context_, LocalNotificationReceiver.class);
        intent.putExtra("notification_id", _tag);
        intent.putExtra("message", _message);
        PendingIntent sender = PendsingIntent.getBroadcast(activity, _tag, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        return sender;
    }
}
