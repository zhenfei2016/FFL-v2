/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioFormat.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/13
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ��Ƶ�ɼ��ĸ�ʽ��Ϣ
*
*/

#include "AudioFormat.hpp"
namespace player {
	AudioFormat::AudioFormat() {
		reset();
	}	
	void  AudioFormat::reset() {		
		mFormat = AV_SAMPLE_FMT_NONE;
		mFreq = 0;
		mChannelNum = 0;
	}

	bool AudioFormat::equal(const AudioFormat& r)  const {
		return (mFormat == r.mFormat)&&
			(mFreq == r.mFreq) &&
			(mChannelNum == r.mChannelNum);
	}
	bool AudioFormat::operator==(const AudioFormat& r) const {
		return equal(r);
	}
	AudioFormat& AudioFormat::operator=(const AudioFormat& r) {
		mFormat = r.mFormat;
		mFreq = r.mFreq;
		mChannelNum = r.mChannelNum;
		return *this;
	}
	//
	//  ��ȡ��������
	//
	int64_t AudioFormat::getChannelLayout() {		
		return av_get_default_channel_layout(mChannelNum);
	}
	//
	// ��ȡһ��������Ĵ�С
	//
	uint32_t AudioFormat::getPerFrameSize() {
		return av_get_bytes_per_sample(mFormat) * mChannelNum;
	}
	//
	//  �����ǰ��������Ϣ
	//
	const char* AudioFormat::dump() {
#if DEBUG_AUDIO		
		//mDumpInfo.format("freq:%d ,format=%d,channel=%d" , mFreq, mFormat, mChannelNum);
		return mDumpInfo.c_str();
#else
		return "AudioFormat::dump";
#endif		
	}


	//
	//  ��ȡ�����ٶȸı����Ҫ�����Ĳ����������ܱ���С
	//  sampleNum:ԭʼ��������µĲ�����
	//  speed :�����ٶȣ� �����ٶ�Ϊ100 , <100���� , >100���
	//
	int32_t getCompensationSampleNum(uint32_t sampleNum,uint32_t speed){
		if (speed == SPEED_NORAML || speed  ==0 || sampleNum == 0) {
			return 0;
		}

		//
		//  ��������ٶ��������Ҫ�Ĳ�����
		//
		int32_t wantSampleNum=(int32_t)((double)(sampleNum * SPEED_NORAML) / speed);
		
		int32_t diffSampleNum = 0;
		if (speed < SPEED_NORAML) {
			diffSampleNum = wantSampleNum - sampleNum;
		}
		else {
			diffSampleNum = sampleNum- wantSampleNum;
		}	

		return diffSampleNum;
	}
}
