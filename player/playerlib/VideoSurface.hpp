/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Window.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  ��Ⱦ����
*
*/
#pragma once
#include <ref/FFL_Ref.hpp>
namespace player {
	class VideoSurface : public FFL::RefBase
	{
	public:
		VideoSurface();
		~VideoSurface();

		//
		//  ���� ��ȡ���ھ��
		//
		virtual void setHandle(void* handle);
		virtual void* getHandle();

		//
		//  ���ô��ڵĿ��ȣ��߶�
		//
		virtual void setWindowSize(int32_t widht, int32_t height);

	private:
		void* mHandle;
	};
	
}