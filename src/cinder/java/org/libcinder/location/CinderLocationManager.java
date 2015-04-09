package org.libcinder.location;

import org.libcinder.app.Platform;

import android.app.Activity;
import android.content.Context;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;

/** \class CinderLocationManager
 *
 * Implemented with the Android android.location API
 * Currently built using reasonable defaults for ongoing location tracking.
 * For future we may build for more use cases to allow things like a single
 * GPS request rather than ongoing location tracking.
 *
 * TODO: Consider using the Google Play
 */
public class CinderLocationManager implements LocationListener {
    private Location mCurrentLocation;
    //private Date mLastUpdateTime;
    private boolean mRequestingLocationUpdates;
    private LocationManager mLocationManager;
    private String mProvider;

    // TODO: Determine how we want to let these be configured, and how much parity we want with the iOS block

    private long mMinTime = 200;
    private float mMinDistance = 1.0f;

    public CinderLocationManager() {
        mRequestingLocationUpdates = false;
        mLocationManager = (LocationManager) Platform.getActivity().getSystemService(Context.LOCATION_SERVICE);

        Criteria criteria = new Criteria();
        criteria.setPowerRequirement(Criteria.POWER_LOW);
        criteria.setAccuracy(Criteria.ACCURACY_COARSE);
        mProvider = mLocationManager.getBestProvider(criteria, false);
        if (mProvider != null) {
            Log.d(Platform.TAG, "LocationManager created");
        } else {
            Log.e(Platform.TAG, "LocationManager could not locate appropriate GPS provider.  Check AndroidManifest.xml for appropriate uses-permissions for location.");
        }
    }

    @Override
    public void onLocationChanged(Location location) {
        Log.d(Platform.TAG, "onLocationChanged " + location.getLatitude() + "," + location.getLongitude());
        mCurrentLocation = location;

        // TODO: Callback to C++?
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        Log.d(Platform.TAG, "onStatusChanged: " + provider + " " + status);
    }

    @Override
    public void onProviderEnabled(String provider) {
        Log.d(Platform.TAG, "onProviderEnabled: " + provider);

    }

    @Override
    public void onProviderDisabled(String provider) {
        Log.d(Platform.TAG, "onProviderDisabled: " + provider);

    }

    public void startLocationUpdates() {
        Log.d(Platform.TAG, "startLocationUpdates: " + mProvider);

        try {
            mLocationManager.requestLocationUpdates(mProvider, mMinTime, mMinDistance, this);
        }
        catch(Exception e ) {
            Log.e("LOCATION", "CinderLocationManager.startLocationUpdates failed: " + e);
        }
    }

    public void stopLocationUpdates() {
        Log.d(Platform.TAG, "stopLocationUpdates");

        try {
            mLocationManager.removeUpdates(this);
        }
        catch(Exception e ) {
            Log.e("LOCATION", "CinderLocationManager.stopLocationUpdates failed: " + e);
        }
    }

    public boolean hasProvider() {
        return mProvider != null;
    }

    /*
    @Override
    public void onResume() {
        super.onResume();
        if (!mRequestingLocationUpdates) {
            startLocationUpdates();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopLocationUpdates();
    }
    */

    public boolean isRequestingUpdates() { return mRequestingLocationUpdates; }

    // TODO: Implement state saving for the activity


    // =============================================================================================
    // Static Methods for C++
    // =============================================================================================

    private static CinderLocationManager sLocationManager = null;

    public static boolean initialize() {
        if(null == sLocationManager) {
            sLocationManager = new CinderLocationManager();
        }
        return sLocationManager.hasProvider();
    }

    //public static void enable( float accuracyInMeters = 20.0f, float distanceFilter = 0.0f, float headingFilter = 0.1f ) {
    public static void enable() {
        if(null == sLocationManager) {
            return;
        }

        sLocationManager.startLocationUpdates();
    }

    public static void disable() {
        if(null == sLocationManager) {
            return;
        }

        sLocationManager.stopLocationUpdates();
    }

    public static boolean isEnabled() {
        return (null != sLocationManager && sLocationManager.isEnabled());
    }

    //public static LocationEvent getMostRecentLocation() { /* return sMostRecentLocation; */ }
}

