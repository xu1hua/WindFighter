package org.cocos2dx.cpp;

import android.util.Log;

import java.util.UUID;

/**
 * Created by hua.xu on 2016/5/31.
 */
public class AppUtility {
    public static String CreateGuidString()
    {
        UUID uuid = UUID.randomUUID();
        Log.d("AppUtility:","CreateGuidString" + uuid.toString());
        return uuid.toString();
    }
}
