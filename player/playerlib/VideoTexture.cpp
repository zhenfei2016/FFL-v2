/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoTexture.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/13
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ����ȾͼƬ�Ļ�����Ϣ
*/

#include "VideoTexture.hpp"


VideoTexture::VideoTexture() {
	//
	//  ��ʾ��ʱ�䣬������ʱ��
	//
	mPts=-1;
	mDuration=-1;



	//
	//ͼƬ�Ŀ�ȣ��߶ȣ���ʽ
	//
	mWidth=0;
	mHeight=0;
	mPixFormat=0;
	//
	//  ��߱�  ,���ܸ���Ƶ��ȣ��߶Ȳ��ɱ���
	//
	//av_guess_sample_aspect_ratio
	mSampleAspectRatioNum=0;
	mSampleAspectRatioDen=0;


	//
	//  ͼƬ���ݣ��м�Ƭ�������С����ʼָ��
	//		
	mPlanesCount=0;
	memset(mPitches,0,sizeof(mPitches[0])*4);
	memset(mPixels, 0, sizeof(mPixels[0]) * 4);
}

VideoTexture::~VideoTexture() {
}