package io.agora.rtc.cxx.apis.sample;

public class RtcEngineNative {
    static {
        System.loadLibrary("rtc-cxx-api-samples");
        System.loadLibrary("agora-rtc-sdk");
    }

    private static RtcEngineNative instance;

    private RtcEngineNative(String appId) {
        nativeInitialize(appId);
    }

    public synchronized static RtcEngineNative create(String appId) {
        if (instance == null) {
            instance = new RtcEngineNative(appId);
        }
        return instance;
    }

    public void joinChannel(String token, String channelName, int uid) {
        nativeJoinChannel(token, channelName, uid);
    }

    public void leaveChannel() {
        nativeLeaveChannel();
    }

    public synchronized static void destroy() {
        if (instance != null) {
            instance.nativeDestroy();
            instance = null;
        }
    }

    private native void nativeInitialize(String appid);

    private native void nativeJoinChannel(String token, String channelName, int uid);

    private native void nativeLeaveChannel();

    private native void nativeDestroy();
}
