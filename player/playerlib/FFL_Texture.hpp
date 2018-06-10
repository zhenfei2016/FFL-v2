/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Texture.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  宣言视频图片结构
*
*/

#pragma once
#include <FFL.h>

class FFLTexture{
public:
	FFLTexture();
	~FFLTexture();

public:
	uint32_t mStreamId;	
	//
	// 原始的pts,使用流中的时基
	//
	int64_t mOrginalPts;

	int64_t mRenderus;

	//
	//  显示的时间，持续的时长
	//
	int64_t mPts;
	int64_t mDuration;

	//
	//图片的宽度，高度，格式
	//
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mPixFormat;
	//
	//  宽高比  ,可能跟视频宽度，高度不成比例
	//
	//av_guess_sample_aspect_ratio
	uint32_t mSampleAspectRatioNum;
	uint32_t mSampleAspectRatioDen;


	//
	//  图片数据，有几片，对齐大小，起始指针
	//		
	uint16_t mPlanesCount;
	uint16_t mPitches[4];
	uint8_t *mPixels[4];
};
