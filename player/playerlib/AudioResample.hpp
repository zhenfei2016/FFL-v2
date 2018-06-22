/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioResample.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  �������ز���
*
*/
#ifndef _AUDIO_RESAMPLE_HPP_
#define _AUDIO_RESAMPLE_HPP_

#include "AudioSample.hpp"
#include "FFMpeg.hpp"
#include "AudioFormat.hpp"

namespace player {
	class AudioResample  {	
	public:
		AudioResample();
		~AudioResample();
	protected:
		//
		//  �����ز���������
		//
		bool createSwr(const AudioFormat* src,const AudioFormat* dst,uint32_t speed);
		//
		//  ɾ���ز���������
		//
		void destroySwr();
	public:
		//
		//  speed  :��ǰ��Ƶ�Ĳ����ٶ�100�����ٶ�
		//   in:    ����Ĳ�������
		//   out:   ������ز�������
		//
		bool resample(const AudioSample* in , AudioSample* out,uint32_t speed);		
	protected:
		//
		//  ffmpeg���ز�����
		//
		SwrContext *mSwrCtx;		
		//
		//  ��ǰ��Դ��ʽ��Ŀ���ʽ
		//
		AudioFormat mSrc;
		AudioFormat mDst;
		//
		// �����ٶ�,
		// �ٶȸı������ mSampleNum������ ����ӦCompensationNum������
		//
		uint32_t mSpeed;

		uint32_t mSampleNum;
		int32_t  mCompensationNum;
	};
}

#endif