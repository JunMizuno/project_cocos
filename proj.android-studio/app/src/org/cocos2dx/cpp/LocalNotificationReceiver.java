package org.cocos2dx.cpp;

import android.os.Bundle;
import android.util.Log;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmapfactory;
import android.support.v4.app.NotificationCompat;

public class LocalNotificationReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context _context, Intent _intent) {
        int notificationId = _intent.getIntExtra("notification_id", 0);
        String message = _intent.getStringExtra("message");

        Intent launchIntent = _context.getPackageManager().getLaunchIntentForPackage(_context.getPackageName());
        launchIntent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        PendingIntent pendingIntent = PendingIntent.getActivity(_context, 0, launchIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        Resources resources = _context.getResources();
        String packageName = _context.getPackageName();
        String appName = _context.getString(resources.getIdentifier("app_name", "string", packageName));

        // @memo. アイコン画像はres->mipmapに格納してAndroidManifestで設定する
        int smallIcon = resources.getIdentifier("icon", "mipmap", packageName);
        int icon = resources.getIdentifier("icon", "mipmap", packageName);
        if (smallIcon == 0) {
            smallIcon = icon;
        }
        Bitmap largeIcon = BitmapFactory.decodeResources(resources, icon);

        NotificationCompat.Builder builder = null;
        // Android Oreo(API26)以上であるかどうかで処理わけ
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
            // タグはチャンネル設定時のものに合わせる
            builder = new NotificationCompat.Builder(_context, "channel_local_notification");
        }
        else {
            builder = new NotificationCompat.Builder(_context);
        }

        builder.setContentTitle(appNme);
        builder.setContentText(message);
        // @memo. アイコン設定が正しくないと届かないとのこと
        builder.setSmallIcon(smallIcon);
        builder.setLargeIcon(largeIcon);
        builder.setTicker(message);
        builder.setAutoCancel(true);
        builder.setDefaults(Notification.DEFAULT_ALL);
        builder.setTicker(pendingIntent);

        NotificationManager manager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
        manafer.notify(notificationId, builder.build());
    }
}
