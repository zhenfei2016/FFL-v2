/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Sample.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  重采样结构
*
*/
#pragma once
#include <FFL.h>

//
//  音频的采样
//
class FFLSample{
public:
	FFLSample();
	~FFLSample();

public:
	uint32_t mStreamId;
	//
	// 原始的pts,使用流中的时基
	//
	int64_t mOrginalPts;


	//
	//  显示的时间，持续的时长
	//
	int64_t mPts;
	int64_t mDuration;
	//
	// 采样格式（深度），通道数，通道布局，采样个数,采样率
	//
	uint32_t mFormat;
	uint32_t mChannel;
	uint32_t mChannelLayout;
	uint32_t mSampleNum;
	uint32_t mFreq;

	//
	//  采样的数据，片大小，pcm数据
	//			
	int mLinesize;
	uint8_t **mData;


	uint8_t* mDataTemp[4];
};
