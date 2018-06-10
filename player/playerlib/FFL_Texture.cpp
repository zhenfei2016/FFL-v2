/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Texture.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  ������ƵͼƬ�ṹ
*
*/

#include "FFL_Texture.hpp"


FFLTexture::FFLTexture() {
	//
	//  ��ʾ��ʱ�䣬������ʱ��
	//
	mPts=-1;
	mDuration=-1;

	mOrginalPts = 0;

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

FFLTexture::~FFLTexture() {
}