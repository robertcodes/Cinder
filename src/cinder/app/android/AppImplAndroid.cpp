/*
 Copyright (c) 2012, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

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

#include "cinder/app/android/AppImplAndroid.h"
#include "cinder/app/android/AppAndroid.h"
#include "cinder/app/AppBase.h"

namespace cinder { namespace app {

AppImplAndroid::AppImplAndroid( AppAndroid *aApp, const AppAndroid::Settings &settings )
	: mApp( aApp ), mSetupHasBeenCalled( false ), mActive( true )
{
}

AppImplAndroid::~AppImplAndroid()
{
}

AppAndroid* AppImplAndroid::getApp() 
{ 
	return mApp; 
}

void AppImplAndroid::run()
{
}

WindowRef AppImplAndroid::createWindow( Window::Format format )
{
	if( ! format.getRenderer() )
		format.setRenderer( mApp->getDefaultRenderer()->clone() );

	mWindows.push_back( new WindowImplAndroid( format, nullptr/*mApp->findSharedRenderer( format.getRenderer() )*/, this ) );

	// emit initial resize if we have fired setup
	if( mSetupHasBeenCalled )
		mWindows.back()->getWindow()->emitResize();

	return mWindows.back()->getWindow();
}

void AppImplAndroid::quit()
{
	// Close all windows, forcing the application to quit.
	while( ! mWindows.empty() )
		mWindows.back()->close();

	// Always quit, even if ! isQuitOnLastWindowCloseEnabled()
	mShouldQuit = true;
}

void AppImplAndroid::sleep( double seconds )
{
    throw (std::string(__FUNCTION__) + " not implemented yet").c_str();
}

bool AppImplAndroid::setupHasBeenCalled() const 
{ 
	return mSetupHasBeenCalled; 
}

float AppImplAndroid::getFrameRate() const 
{ 
	return mFrameRate; 
}

void AppImplAndroid::setFrameRate( float frameRate )
{
	mFrameRate = frameRate;
	mFrameRateEnabled = true;
	mNextFrameTime = mApp->getElapsedSeconds();
}

void AppImplAndroid::disableFrameRate()
{
	mFrameRateEnabled = false;
}

bool AppImplAndroid::isFrameRateEnabled() const
{
	return mFrameRateEnabled;
}

RendererRef AppImplAndroid::findSharedRenderer( const RendererRef &searchRenderer )
{
	if( ! searchRenderer )
		return RendererRef();

	for( const auto &win : mWindows ) {
		RendererRef renderer = win->getRenderer();
		if( renderer && ( typeid(*renderer) == typeid(*searchRenderer) ) )
			return renderer;
	}

	return RendererRef();	
}

WindowRef AppImplAndroid::getWindow() const 
{ 
	return mActiveWindow; 
}

void AppImplAndroid::setWindow( WindowRef window ) 
{ 
	mActiveWindow = window; 
}

size_t AppImplAndroid::getNumWindows() const
{
	return mWindows.size();
}

WindowRef AppImplAndroid::getWindowIndex( size_t index ) const
{
	if( index >= mWindows.size() )
		return cinder::app::WindowRef();
	
	auto winIt = mWindows.begin();
	std::advance( winIt, index );
	return (*winIt)->mWindowRef;
}

WindowRef AppImplAndroid::getForegroundWindow() const
{
	return mForegroundWindow;
}

void AppImplAndroid::setForegroundWindow( WindowRef window )
{
	mForegroundWindow = window;
}

void AppImplAndroid::closeWindow( WindowImplAndroid *windowImpl )
{
	auto winIt = find( mWindows.begin(), mWindows.end(), windowImpl );
	if( winIt != mWindows.end() ) {
		windowImpl->getWindow()->emitClose();
		windowImpl->privateClose();
		delete windowImpl; // this corresponds to winIt
		mWindows.erase( winIt );
	}

	if( mWindows.empty() && mQuitOnLastWindowClosed )
		mShouldQuit = true;
}

void AppImplAndroid::hideCursor()
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();
}

void AppImplAndroid::showCursor()
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();
}

ivec2 AppImplAndroid::getMousePos() const
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();

	return ivec2( -1, -1 );	
}

fs::path AppImplAndroid::getAppPath()
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();

	return fs::path();
}

fs::path AppImplAndroid::getOpenFilePath( const fs::path &initialPath, std::vector<std::string> extensions )
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();

	return fs::path();
}

fs::path AppImplAndroid::getSaveFilePath( const fs::path &initialPath, std::vector<std::string> extensions )
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();

	return fs::path();
}

fs::path AppImplAndroid::getFolderPath( const fs::path &initialPath )
{
	throw (std::string(__FUNCTION__) + " not implemented yet").c_str();

	return fs::path();
}

///////////////////////////////////////////////////////////////////////////////
// WindowImplAndroid
WindowImplAndroid::WindowImplAndroid( const Window::Format &format, RendererRef sharedRenderer, AppImplAndroid *appImpl )
{
    mPos = ivec2( 0, 0 );
    mNativeWindow = nullptr;
}

WindowImplAndroid::~WindowImplAndroid()
{
}

void WindowImplAndroid::setFullScreen( bool fullScreen, const app::FullScreenOptions &options )
{
}

void WindowImplAndroid::setSize( const ivec2 &size )
{
}

void WindowImplAndroid::setPos( const ivec2 &pos )
{
}

void WindowImplAndroid::close()
{
}

void WindowImplAndroid::setTitle( const std::string &title )
{
}

void WindowImplAndroid::hide()
{
}

void WindowImplAndroid::show()
{
}

void WindowImplAndroid::setBorderless( bool borderless )
{
}

void WindowImplAndroid::setAlwaysOnTop( bool alwaysOnTop )
{
}

void WindowImplAndroid::draw()
{
	mAppImpl->setWindow( mWindowRef );
	mRenderer->startDraw();
	mWindowRef->emitDraw();
	mRenderer->finishDraw();
}

void WindowImplAndroid::resize()
{
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitResize();
}

void WindowImplAndroid::redraw()
{
}

void WindowImplAndroid::privateClose()
{
/*
	mRenderer->kill();

	if( mDC )
		::ReleaseDC( mWnd, mDC );

	if( mWnd )
		::DestroyWindow( mWnd );

	mWnd = 0;
*/
}

void WindowImplAndroid::keyDown( const KeyEvent &event )
{
	KeyEvent localEvent( event );
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitKeyDown( &localEvent );
}

} } // namespace cinder::app