package io.agora.rtc.cxx.apis.sample;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    RtcEngineNative rtcEngineNative;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    private final static int REQUEST_CODE = 101;

    private void showShort(String s) {
        Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode != REQUEST_CODE) {
            return;
        }
        for (int result : grantResults) {
            if (result != PackageManager.PERMISSION_GRANTED) {
                showShort("No permission for " + Arrays.toString(permissions));
                return;
            }
        }
        joinChannel();
    }


    public void onClickJoin(View view) {
        checkEngineCreated();
        String[] requestPermission = {
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.CAMERA,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
        };

        if (AppUtil.checkAndRequestAppPermission(this, requestPermission, REQUEST_CODE)) {
            joinChannel();
        }
    }

    private void checkEngineCreated() {
        if (rtcEngineNative == null)
            rtcEngineNative = RtcEngineNative.create(getString(R.string.agora_app_id));
    }

    private void joinChannel() {
        rtcEngineNative.joinChannel(null, "cxx_api_channel", 0);
    }

    public void onClickLeave(View view) {
        checkEngineCreated();
        rtcEngineNative.leaveChannel();
    }

}
