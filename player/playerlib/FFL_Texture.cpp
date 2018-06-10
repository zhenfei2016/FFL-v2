/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Texture.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  宣言视频图片结构
*
*/

#include "FFL_Texture.hpp"


FFLTexture::FFLTexture() {
	//
	//  显示的时间，持续的时长
	//
	mPts=-1;
	mDuration=-1;

	mOrginalPts = 0;

	//
	//图片的宽度，高度，格式
	//
	mWidth=0;
	mHeight=0;
	mPixFormat=0;
	//
	//  宽高比  ,可能跟视频宽度，高度不成比例
	//
	//av_guess_sample_aspect_ratio
	mSampleAspectRatioNum=0;
	mSampleAspectRatioDen=0;


	//
	//  图片数据，有几片，对齐大小，起始指针
	//		
	mPlanesCount=0;
	memset(mPitches,0,sizeof(mPitches[0])*4);
	memset(mPixels, 0, sizeof(mPixels[0]) * 4);
}

FFLTexture::~FFLTexture() {
}