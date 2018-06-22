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
#ifndef _AUDIOSAMPLEFORMAT_HPP_
#define _AUDIOSAMPLEFORMAT_HPP_

#include "FFMpeg.hpp"
#define DEBUG_AUDIO 1
#if DEBUG_AUDIO
#include <utils/FFL_String.hpp>
#endif
namespace player {
	//
	//  ��Ƶ������ʽ��Ϣ
	//
	class AudioFormat {
	public:
		AudioFormat();
		//
		// ����һ�����е���ֵ
		//
		void reset();
		//
		//  �Ƚ��Ƿ����
		//
		bool equal(const AudioFormat& r) const;	
		bool operator==(const AudioFormat& r) const;
		//
		// ��ֵ����
		//
		AudioFormat& operator=(const AudioFormat& r);
	public:
		//
		//  ��ȡͨ����
		//
		uint32_t getChannelNum() const {
			return mChannelNum;
		}
		//
		//  ��ȡ��������
		//
		int64_t getChannelLayout();
		//
		// ��ȡһ��������Ĵ�С
		//
		uint32_t getPerFrameSize();
		//
		//  �����ǰ��������Ϣ
		//
		const char* dump();
	public:
		//
		// �����ʣ�������ȣ����������ٶ�
		//
		uint32_t mFreq;			
		AVSampleFormat mFormat;
		uint32_t mChannelNum;

#if DEBUG_AUDIO
		FFL::String mDumpInfo;
#endif
	};

	//
	//  ��ȡ�����ٶȸı����Ҫ�����Ĳ����������ܱ���С
	//  sampleNum:ԭʼ��������µĲ�����
	//  speed :�����ٶȣ� �����ٶ�Ϊ100 , <100���� , >100���
	//
	//
	//  �����Ĳ����ٶ�
	//
    #define SPEED_NORAML 100
	int32_t getCompensationSampleNum(uint32_t sampleNum, uint32_t speed);
}

#endif