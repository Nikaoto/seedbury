#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"

#endif

using namespace cocos2d;

class InternetStatus {
public:
    static void toastMessage(const char* message) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        JniMethodInfo methodInfo;
        if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "ge/seedbury/game/Connectivity", "toastMessage", "(Ljava/lang/String;)V")) {
            log("JNI METHOD BRIDGE FAIL");
            return;
        }

        jstring javaText = methodInfo.env->NewStringUTF(message);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, javaText);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
    }

    static bool isConnected() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        JniMethodInfo methodInfo;
        if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "ge/seedbury/game/Connectivity", "isConnectedToInternet", "()Z")) {
            return false;
        }

        bool ans = (bool) methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return ans;
#endif
    }

    static bool isConnectionFast() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        JniMethodInfo methodInfo;
        if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "ge/seedbury/game/Connectivity", "isConnectedToFastInternet", "()Z")) {
            return false;
        }

        bool ans = (bool) methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return ans;
#endif
    }

    static bool isConnectionMobile() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        JniMethodInfo methodInfo;
        if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "ge/seedbury/game/Connectivity", "isConnectedToMobileInternet", "()Z")) {
            return false;
        }

        bool ans = (bool) methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return ans;
#endif
    }
};