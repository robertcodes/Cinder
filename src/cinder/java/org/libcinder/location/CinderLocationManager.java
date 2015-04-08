package org.libcinder.hardware;

import org.libcinder.app.Platform;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;

/** \class CinderLocationManager
 *
 */
public class CinderLocationManager implements LocationListener {
    private Location mCurrentLocation;
    private DateTime mLastUpdateTime;
    private boolean mRequestingLocationUpdates;

    public CinderLocationManager() {
        mRequestingLocationUpdates = false;
        mLocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        provider = locationManager.getBestProvider(criteria, false);
        
        Log.d(Platform.TAG, "Location Manager created");
    }

    @Override
    public void onLocationChanged(Location location) {
        mCurrentLocation = location;
        mLastUpdateTime = DateFormat.getTimeInstance().format(new Date());
        
        // TODO: Callback to C++?
    }

    public void startLocationUpdates() {

        try {
            //LocationServices.FusedLocationApi.requestLocationUpdates(mGoogleApiClient, mLocationRequest, this);
            mLocationManager.requestLocationUpdates(provider, 200, 1, mylistener);
        }
        catch(Exception e ) {
            Log.e(Platform.TAG, "CinderLocationManager.startLocationUpdates failed: " + e);
        }
    }

    public void stopLocationUpdates() {
        try {
            LocationServices.FusedLocationApi.removeLocationUpdates(mGoogleApiClient, this);
        }
        catch(Exception e ) {
            Log.e(Platform.TAG, "CinderLocationManager.stopLocationUpdates failed: " + e);
        }
    }

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

    public boolean isEnabled() const { return mRequestingLocationUpdates; }

    // TODO: Implement state saving for the activity


    // =============================================================================================
    // Static Methods for C++
    // =============================================================================================

    private static CinderLocationManager sLocationManager = null;

    public static boolean initialize() {
        if(null == sLocationManager) {
            sLocationManager = new CinderLocationManager();
        }
        return true;
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

    public static bool isEnabled() {
        return (null != sLocationManager && sLocationManager.isEnabled());
    }

    public static LocationEvent getMostRecentLocation() { /* return sMostRecentLocation; */ }
}
