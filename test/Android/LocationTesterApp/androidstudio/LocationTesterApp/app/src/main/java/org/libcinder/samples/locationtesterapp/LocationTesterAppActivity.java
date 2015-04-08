package org.libcinder.samples.locationtesterapp;

import android.app.NativeActivity;
import android.os.Bundle;

import org.libcinder.app.Platform;

public class LocationTesterAppActivity extends NativeActivity {
    static final String TAG = "LocationTesterAppActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Platform.setActivity(this);
    }
}
