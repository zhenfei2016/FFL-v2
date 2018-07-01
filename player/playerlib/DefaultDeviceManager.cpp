/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  DefaultDeviceManager.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/07/01
*  https://github.com/zhenfei2016/FFL-v2.git
*  ����Ƶ�豸�����豸�Ĵ򿪣��رգ���ȡ�Ѿ����������Ƶ�豸
*  Ĭ��ʹ�õģ�androidƽ̨��ʹ�� AndroidModule.hpp  ������ƽ̨ʹ��SDL2Module.hpp
*
*/

#include "DefaultDeviceManager.hpp"
#include "SDL2Module.hpp"
#if defined(ANDROID)
#include "android/AndroidModule.hpp"
#endif

namespace player {
	DefaultDeviceManager::DefaultDeviceManager(){
		mSurfaceHandle = NULL;
		//mPlayer = player;
#if defined(ANDROID)
		mDeviceFactory = new android::AndroidModule();
#else
		mDeviceFactory = new SDL2Module();
#endif
	}
	DefaultDeviceManager::~DefaultDeviceManager() {
		if (!mVideoDevice.isEmpty()) {
			mVideoDevice->close();
			mVideoDevice = NULL;
		}

		if (!mAudioDevice.isEmpty()) {
			mAudioDevice->close();
			mAudioDevice = NULL;
		}

		FFL_SafeFree(mDeviceFactory);
	}
	//
	//  ͨ��wnd������Ӧ��
	//
	FFL::sp<VideoDevice> DefaultDeviceManager::openVideoDisplay(SurfaceHandle wnd) {
		FFL::sp<VideoDevice> dev = mDeviceFactory->createVideoDevice(this);
		uint32_t width = 400;
		uint32_t height = 300;	
		dev->open(wnd, width, height);
		if (!mVideoDevice.isEmpty()) {
			FFL_LOG_WARNING("DefaultDeviceManager::openVideoDisplay mVideoDevice isnot null.");
		}
		mVideoDevice = dev;
		return dev;
	}
	//
	//  ���ʽͨ���Ѿ�������SufraceHandle ������VideoDevice�򲻻�ر�������ڵ�
	//
	void DefaultDeviceManager::closeVideoDisplay() {
		if (!mVideoDevice.isEmpty()) {
			mVideoDevice->close();
			mVideoDevice = NULL;
		}
	}
	//
	//  ͨ��fmt�򿪶�Ӧ����Ƶ�豸
	//
	FFL::sp<AudioDevice> DefaultDeviceManager::openAudioDisplay(AudioFormat fmt) {		
		FFL::sp<AudioDevice> dev = mDeviceFactory->createAudioDevice(this);
		//
		// ������Ƶ�豸
		//
		AudioFormat obtainedFmt;
		if (!dev->open(fmt, 1024, obtainedFmt)) {
			return NULL;
		}

		mAudioDevice = dev;
		return dev;
	}
	//
	//  �ر���Ƶ�豸
	//
	void DefaultDeviceManager::closeAudioDisplay() {
		if (!mAudioDevice.isEmpty()) {
			mAudioDevice->close();
			mAudioDevice = NULL;
		}
	}
	//
	//  ��ȡ�Ѿ���������Ƶ�豸
	//
	FFL::sp<VideoDevice> DefaultDeviceManager::getVideoDisplay(FFL::sp<VideoStream> stream) {
		return mVideoDevice;
	}
	//
	// ��ȡ�Ѿ���������Ƶ�豸
	//
	FFL::sp<AudioDevice> DefaultDeviceManager::getAudioDisplay(FFL::sp<AudioStream> stream) {
		return mAudioDevice;
	}
}