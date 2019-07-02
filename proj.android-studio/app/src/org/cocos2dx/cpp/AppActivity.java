/****************************************************************************
Copyright (c) 2015-2017 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;

// Add for LocalNotification
import android.app.Notification;
import android.app.NotificationManager;
import android.app.NotificationChannel;
import android.graphics.Color;

// Add for URLScheme
import android.net.Uri;
import android.util.Log;

public class AppActivity extends Cocos2dxActivity {
    // Add for Utility_NDK
    private static native void initNDK(Context context);

    // Add for Tapjoy
    public static native void setTapjoyActivity(Context context);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        
        // Add for Utility_NDK
        initNDK(this.getApplicationContext());

        // Add for Tapjoy
        setTapjoyActivity(this.getApplicationContext());

        // Add for LocalNotification
        // 設定画面における通知チャンネルの生成(API26以上、Oreo対応)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODE.O) {
            NotificationManager manager = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
            // 引数内訳:内部タグ、表示項目、表示プライオリティー
            NotificationChannel channel = new NotificationChannel("channel_local_notification", "ローカル通知", NotificationManager.IMPORTANCE_DEFAULT);
            channel.enableLights(true);
            channel.setLightColor(Color.WHITE);
            channel.setLockscreenVisiblity(Notification.VISIBLITY_PUBLIC);
            manager.createNotificationChannel(channel);
        }

        // URLスキーム経由で起動された場合
        Intent intent = getIntent();
        String action = intent.getAction();
        if (Intent.ACTION_VIEW.equals(action)) {
            Uri uri = intent.getData();
            String url = uri.toString();
            URLSchemeEvent.setSchemeURL(url);
        }
    }
}
