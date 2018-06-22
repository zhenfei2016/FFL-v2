/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Sample.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ��Ƶ�����ṹ
*
*/
#include "AudioSample.hpp"

namespace player {

	AudioSample::AudioSample() :mAuotoDelete(false) {
		//
		//  ��ʾ��ʱ�䣬������ʱ��
		//
		mPts = -1;
		mDuration = -1;
		//
		//
		// ������ʽ��ͨ��������������,������
		//
		mFormat = AV_SAMPLE_FMT_NONE;
		mChannel = 0;		
		mSampleNum = 0;
		mFreq = 0;

		mLinesize=0;
		mData = NULL;
	}

	AudioSample::~AudioSample() {
		freeData();
	}

	//
	//  �����ڴ棬
	//  channelNum��ͨ����
	//  sampleNum:������
	//
	void AudioSample::allocData(uint32_t channelNum, uint32_t sampleNum) {
		av_samples_alloc_array_and_samples(
			(uint8_t***)&(mData), &(mLinesize),
			channelNum,
			sampleNum,
			(AVSampleFormat)mFormat, 0);
		mChannel = channelNum;
		mAuotoDelete = true;
	}
	//
	//  ɾ��������ڴ�
	//
	void AudioSample::freeData() {
		if (mAuotoDelete) {
			av_freep(mData);
			mAuotoDelete = false;
		}
	}
	//
	//  ������ݣ���allocData��һ����ģʽ������������ݲ���Ҫ�ͷ�
	//
	void AudioSample::fillData(uint8_t **data, int32_t linesize, uint32_t sampleNum) {
		freeData();

		mData = data;
		mLinesize = linesize;
		mSampleNum=sampleNum;
		mAuotoDelete = false;
	}
	//
	//  ��r�е������ƶ�����
	//
	void AudioSample::moveData(AudioSample& r) {
		freeData();

		mData = r.mData;
		r.mData = 0;

		mLinesize = r.mLinesize;
		r.mLinesize = 0;

		mSampleNum = r.mSampleNum;
		r.mSampleNum = 0;

		mAuotoDelete = r.mAuotoDelete;
		r.mAuotoDelete = false;
	}
	//
	//  �����ĸ�ʽ
	//
	void AudioSample::setAudioFormat(const AudioFormat& fmt) {
		mChannel=fmt.mChannelNum ;
		mFormat=fmt.mFormat;
		mFreq=fmt.mFreq;
	}
	void AudioSample::getAudioFormat(AudioFormat& fmt) {
		fmt.mChannelNum = mChannel;
		fmt.mFormat = mFormat;
		fmt.mFreq = mFreq;
	}
}