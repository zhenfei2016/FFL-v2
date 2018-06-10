/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Sample.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  重采样结构
*
*/
#include "FFL_Sample.hpp"


FFLSample::FFLSample() {
	//
	//  显示的时间，持续的时长
	//
	mPts=-1;
	mDuration=-1;
	mOrginalPts = 0;

	//
	//
	// 采样格式，通道数，采样个数,采样率
	//
	mFormat=0;
	mChannel=0;
	mChannelLayout = 0;
	mSampleNum=0;
	mFreq=0;


	//
	//  图片数据，有几片，对齐大小，起始指针
	//			
}

FFLSample::~FFLSample() {
}