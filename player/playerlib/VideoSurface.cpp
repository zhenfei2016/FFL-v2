/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Window.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  ��Ⱦ����
*
*/

#include "VideoSurface.hpp"

namespace player {
	VideoSurface::VideoSurface():mHandle(NULL){
	}

	VideoSurface::~VideoSurface() {
	}

	void VideoSurface::setHandle(void* handle) {
		mHandle = handle;
	}

	void* VideoSurface::getHandle() {
		return mHandle;
	}
	//
	//  ���ô��ڵĿ�ȣ��߶�
	//
	void VideoSurface::setWindowSize(int32_t widht, int32_t height) {
	}
}