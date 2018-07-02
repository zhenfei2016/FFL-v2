/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AndroidVideoDevice.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android播放音频接口
*
*/
#include "AndroidVideoDevice.hpp"
#include "render/RenderManager.hpp"
#include "VideoFormat.hpp"

namespace android {
	class ANativeSurface : public  player::VideoSurface{
	public:
        ANativeSurface(){
            mVideoFormat.mFormat=player::VideoFormat::PIXEL_FORMAT_RGBA_8888;
        }
        void setHandle(SurfaceHandle handle){
            player::VideoSurface::setHandle(handle);
        }

		bool getWindowSize(int32_t& width, int32_t& height){
			ANativeWindow* win=getHandle();
			if(win) {
				width = ANativeWindow_getWidth(win);
				height = ANativeWindow_getHeight(win);
				return true;
			}
			return  false;
		}
	};
	AndroidVideoDevice::AndroidVideoDevice(){
		mSurfaceHandle=NULL;
		mVideoSurface=new ANativeSurface();
		mRenderManager=new RenderManager();
		mRenderManager->registerRenders();
	}
	AndroidVideoDevice::~AndroidVideoDevice(){
		mRenderManager->unRegisterRenders();
		FFL_SafeFree(mRenderManager);
	}

	// 获取绘制窗口
	//		
	FFL::sp<player::VideoSurface> AndroidVideoDevice::getSurface() {
		return mVideoSurface;
	}
	//
	// 更新绘制的目标窗口
	//
	void AndroidVideoDevice::setSurface(SurfaceHandle surface) {
		if(mSurfaceHandle==surface){
			return;
		}

		if(surface) {
			ANativeWindow_acquire(surface);
		}

		if(mSurfaceHandle){
			ANativeWindow_release(mSurfaceHandle);
		}
		mSurfaceHandle=surface;
		mVideoSurface->setHandle(surface);
	}
	//
	//  打开关闭视频设备
	//
	bool AndroidVideoDevice::onOpen(SurfaceHandle surface, int32_t widht, int32_t height) {
		setSurface(surface);
		return true;
	}
	void AndroidVideoDevice::onClose() {
		mVideoSurface->setHandle(NULL);
		if(mSurfaceHandle){
			ANativeWindow_release(mSurfaceHandle);
			mSurfaceHandle=NULL;
		}
	}
	//
	//  清空cache的数据
	//
	void AndroidVideoDevice::clearCache() {

	}
	//
	//  写一帧数据
	//		
	bool AndroidVideoDevice::showTexture(player::VideoTexture* texture) {
		RenderInterface* render=mRenderManager->getRender(texture->getVideoFormat(),NULL);
		render->draw(mVideoSurface,texture);
		return false;
	}
}