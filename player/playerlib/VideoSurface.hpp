/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Window.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  渲染窗口
*
*/
#ifndef _VIDEO_SURFACE_HPP_
#define _VIDEO_SURFACE_HPP_

#if WIN32
#include <windows.h>
typedef HWND  SurfaceHandle;
#else
typedef void*  SurfaceHandle;
#endif

#include <ref/FFL_Ref.hpp>
namespace player {
	class VideoSurface : public FFL::RefBase{
	public:
		VideoSurface();
		~VideoSurface();
		//
		//  设置 获取窗口句柄
		//
		virtual void setHandle(SurfaceHandle handle);
		virtual SurfaceHandle getHandle();
		//
		//  设置窗口的宽度，高度
		//
		virtual void setWindowSize(int32_t widht, int32_t height);
	private:
		SurfaceHandle mHandle;
	};	
}
#endif
