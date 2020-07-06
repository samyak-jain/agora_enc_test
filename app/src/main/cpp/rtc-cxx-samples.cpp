#include <jni.h>
#include <string>
#include <android/log.h>

#include "include/IAgoraRtcEngine.h"

#define TAG "AGORA_CXX"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

using namespace agora::rtc;

class MyEventHandler : public IRtcEngineEventHandler {
public:
    MyEventHandler() {
    }

    ~MyEventHandler() {
    }

public:
    virtual void onJoinChannelSuccess(const char *channel, uid_t uid, int elapsed) {
        LOGI("join channel success. channel -> %s, uid -> %u, elapsed -> %d", channel, uid, elapsed);
    }

    virtual void onLeaveChannel(const RtcStats &stats) {
        LOGI("on leave channel executed");
    }

    virtual void onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed) {
        LOGI("rejoin channel success. channel -> %s, uid -> %u, elapsed -> %d", channel, uid, elapsed);
    }

    virtual void onRemoteAudioStateChanged(uid_t uid,
            REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {
        LOGI("remote audio state changed. uid -> %u, state -> %d, reason -> %d, elapsed -> %d", uid, state, reason, elapsed);
    }

    virtual void onConnectionStateChanged(
            CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason) {
        LOGI("connection state changed. state -> %d, reason -> %d", state, reason);
    }

    virtual void onUserJoined(uid_t uid, int elapsed) {
        LOGI("remote user joined. uid -> %u, elapsed -> %d", uid, elapsed);
    }
};

MyEventHandler handler;
IRtcEngine *rtcEngine = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_io_agora_rtc_cxx_apis_sample_RtcEngineNative_nativeInitialize(JNIEnv *env, jobject thiz, jstring appid) {
    if (!rtcEngine && appid) {
        rtcEngine = createAgoraRtcEngine();
        RtcEngineContext ctx;
        const char *str_appid = env->GetStringUTFChars(appid, nullptr);
        ctx.appId = str_appid;
        ctx.eventHandler = &handler;
        LOGI("initialize");
        rtcEngine->initialize(ctx);
        env->ReleaseStringUTFChars(appid, str_appid);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_io_agora_rtc_cxx_apis_sample_RtcEngineNative_nativeJoinChannel(JNIEnv *env, jobject thiz, jstring token,
                                                    jstring channel_name, jint uid) {
    if (rtcEngine && channel_name) {
        const char *str_token = nullptr;
        if (token) {
            str_token = env->GetStringUTFChars(token, nullptr);
        }
        const char *str_channel_name = env->GetStringUTFChars(channel_name, nullptr);
        rtcEngine->joinChannel(str_token, str_channel_name, nullptr, (uid_t) uid);
        LOGI("join channel -> %s", str_channel_name);
        env->ReleaseStringUTFChars(channel_name, str_channel_name);
        if (str_token) {
            env->ReleaseStringUTFChars(token, str_token);
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_io_agora_rtc_cxx_apis_sample_RtcEngineNative_nativeLeaveChannel(JNIEnv *env, jobject thiz) {
    if (rtcEngine) {
        LOGI("leave channel");
        rtcEngine->leaveChannel();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_io_agora_rtc_cxx_apis_sample_RtcEngineNative_nativeDestroy(JNIEnv *env, jobject thiz) {
    if (rtcEngine) {
        rtcEngine->release(true);
        rtcEngine = NULL;
    }
}
