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
#pragma once
#include <ref/FFL_Ref.hpp>
class FFLWindow : public FFL::RefBase
{
public:
	FFLWindow();
	~FFLWindow();

	//
	//  设置窗口的宽度，高度
	//
	virtual void setWindowSize(int32_t widht, int32_t height);
};