/*
 Copyright (c) 2015, The Cinder Project

 This code is intended to be used with the Cinder C++ library, http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "cinder/android/JniHelper.h"

namespace cinder { namespace app {

class PlatformAndroid;

}} // namespace cinder::app

namespace cinder { namespace android {

class CinderLocationManager {
public:

	CinderLocationManager();
	virtual ~CinderLocationManager();

	bool 				initialize();
	void 				enable();
	void 				disable();
	bool 				isEnabled() const { return mRequestingLocationUpdates; }

	//! Returns the altitude in meters of the location event
	//float			getAltitude() const { return mAltitude; }
	//! Returns the coordinate of the location event as a Vec2f where x is latitude and y is longitude
	//cinder::vec2	getCoordinate() const { return mCoordinate; }
	//! Returns the latitude coordinate of the location event
	//float			getLatitude() const { return mCoordinate.x; }
	//! Returns the latitude coordinate of the location event
	//float			getLongitude() const { return mCoordinate.y; }
	//! Returns speed in meters of location event
	//float			getSpeed() const { return mSpeed; }
	//! Returns the horizontal accuracy of location event -- invalid when negative
	//float			getHorizontalAccuracy() const { return mHorizontalAccuracy; }
	//! Returns the vertical accuracy of location event -- invalid when negative
	//float			getVerticalAccuracy() const { return mVerticalAccuracy; }

private:
	static void 		cacheJni();
	static void 		destroyJni();

	static jclass		sCinderLocationManagerClass;
	static jmethodID	sInitializeMethodId;
	static jmethodID 	sStartLocationUpdatesMethodId;
	static jmethodID 	sStopLocationUpdatesMethodId;

	bool				mInitialized = false;
	bool 				mRequestingLocationUpdates = false;

	friend class cinder::app::PlatformAndroid;
};

}} // namespace cinder::android