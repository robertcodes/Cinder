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

#include "cinder/android/CinderLocationManager.h"

#include <sstream>
#include <iomanip>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "cinder", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "cinder", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"cinder", __VA_ARGS__))

#define FN_LOG( __FNREF__ ) 							\
 	if( NULL != __FNREF__ ) {							\
 		LOGI( #__FNREF__ " successfully obtained" );	\
 	}

namespace cinder { namespace android {

jclass		CinderLocationManager::sCinderLocationManagerClass		= nullptr;
jmethodID	CinderLocationManager::sInitializeMethodId 				= nullptr;
jmethodID 	CinderLocationManager::sStartLocationUpdatesMethodId	= nullptr;
jmethodID 	CinderLocationManager::sStopLocationUpdatesMethodId 	= nullptr;

CinderLocationManager::CinderLocationManager()
{
}

CinderLocationManager::~CinderLocationManager()
{
	if( mRequestingLocationUpdates ) {
		disable();
	}
}

void CinderLocationManager::cacheJni()
{
	const std::string className = "org/libcinder/location/CinderLocationManager";

	if( JniHelper::Get()->AttachCurrentThread() ) {
		jclass cinderLocationManagerClass = JniHelper::Get()->RetrieveClass( className );
		if( nullptr != cinderLocationManagerClass ) {
			CinderLocationManager::sCinderLocationManagerClass = reinterpret_cast<jclass>( JniHelper::Get()->NewGlobalRef( cinderLocationManagerClass ) );
		} else {
			LOGE( "Unable to locate the LocationManager class" );
		}

		if( nullptr != CinderLocationManager::sCinderLocationManagerClass ) {
			CinderLocationManager::sInitializeMethodId 		= JniHelper::Get()->GetStaticMethodId( CinderLocationManager::sCinderLocationManagerClass, "initialize", "()Z" );
			CinderLocationManager::sStartLocationUpdatesMethodId	= JniHelper::Get()->GetStaticMethodId( CinderLocationManager::sCinderLocationManagerClass, "enable", "()V" );
			CinderLocationManager::sStopLocationUpdatesMethodId 	= JniHelper::Get()->GetStaticMethodId( CinderLocationManager::sCinderLocationManagerClass, "disable", "()V" );

			FN_LOG( CinderLocationManager::sInitializeMethodId );	
			FN_LOG( CinderLocationManager::sStartLocationUpdatesMethodId );	
			FN_LOG( CinderLocationManager::sStopLocationUpdatesMethodId );	
		}
		JniHelper::Get()->DeatchCurrentThread();
	}
}

void CinderLocationManager::destroyJni()
{
	if( JniHelper::Get()->AttachCurrentThread() ) {
		
		JniHelper::Get()->DeleteGlobalRef( CinderLocationManager::sCinderLocationManagerClass );

		CinderLocationManager::sCinderLocationManagerClass		= nullptr;
		CinderLocationManager::sInitializeMethodId 		= nullptr;
		CinderLocationManager::sStartLocationUpdatesMethodId	= nullptr;
		CinderLocationManager::sStopLocationUpdatesMethodId 	= nullptr;

		JniHelper::Get()->DeatchCurrentThread();
	}
}

// TODO: Determine need for this, probably do appropriate setup in the java methods
bool CinderLocationManager::initialize()
{
	mInitialized = false;

	if( JniHelper::Get()->AttachCurrentThread() ) {
		mInitialized = (bool)JniHelper::Get()->CallStaticBooleanMethod( CinderLocationManager::sCinderLocationManagerClass, CinderLocationManager::sInitializeMethodId );
		JniHelper::Get()->DeatchCurrentThread();
	}

	return mInitialized;	
}

void CinderLocationManager::enable()
{
	if( ! mInitialized ) {
		return;
	}

	if( JniHelper::Get()->AttachCurrentThread() ) {
		JniHelper::Get()->CallStaticVoidMethod( CinderLocationManager::sCinderLocationManagerClass, CinderLocationManager::sStartLocationUpdatesMethodId );
		
		mRequestingLocationUpdates = true;

		JniHelper::Get()->DeatchCurrentThread();
	}	
}

void CinderLocationManager::disable()
{
	if( ! mInitialized ) {
		return;
	}

	if( JniHelper::Get()->AttachCurrentThread() ) {
		JniHelper::Get()->CallStaticVoidMethod( CinderLocationManager::sCinderLocationManagerClass, CinderLocationManager::sStopLocationUpdatesMethodId );

		mRequestingLocationUpdates = false;

		JniHelper::Get()->DeatchCurrentThread();
	}	
}

}} // namespace cinder::android
