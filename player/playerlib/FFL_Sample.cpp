/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Sample.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  �ز����ṹ
*
*/
#include "FFL_Sample.hpp"


FFLSample::FFLSample() {
	//
	//  ��ʾ��ʱ�䣬������ʱ��
	//
	mPts=-1;
	mDuration=-1;
	mOrginalPts = 0;

	//
	//
	// ������ʽ��ͨ��������������,������
	//
	mFormat=0;
	mChannel=0;
	mChannelLayout = 0;
	mSampleNum=0;
	mFreq=0;


	//
	//  ͼƬ���ݣ��м�Ƭ�������С����ʼָ��
	//			
}

FFLSample::~FFLSample() {
}