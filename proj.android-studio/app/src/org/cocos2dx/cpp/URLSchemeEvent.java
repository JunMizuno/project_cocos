package org.cocos2dx.cpp;

public class URLSchemeEvent {
    private static String schemeURL = "";

    public static void setSchemeURL(String url) {
        schemeURL = url;
    }

    public static String getSchemeURL() {
        return schemeURL;
    }
}
