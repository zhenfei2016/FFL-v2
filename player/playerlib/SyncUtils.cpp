
/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  SyncUtils.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/21
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ����ͬ����������
*
*/
#ifndef _SYNC_UTILS_HPP_
#define _SYNC_UTILS_HPP_

#include <FFL.h>
#include <utils/FFL_Clock.hpp>
#include "Stream.hpp"
#include "SyncUtils.hpp"
#include "PlayerConstant.hpp"

namespace player {
	//
	//  ��֡����ֵus������5s���������ͬ��
	//
#define SYNC_RESYNC_THRESHOLD_US 5000000
	//
	//  ��Ҫ����ͬ������ֵ��40ms�� 25֡�������ʱ��ֵ
	//  �������ʱ��������40ms����Ҫ����ͬ��
	//  >SYNC_MIN_THRESHOLD_US ����Ҫͬ��
	//  >SYNC_MAX_THRESHOLD_US ��֡
	//  SYNC_MIN_THRESHOLD_US -  SYNC_MAX_THRESHOLD_US ֮��ʹ��ͬ������
	//
#define SYNC_MIN_THRESHOLD_US 40000
#define SYNC_MAX_THRESHOLD_US 40000*3

	//
	//  ��Ƶͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ� ��Ƶʱ��
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//
	int64_t correctVideoDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt) {
		return correctSyncDelay(master,slave,delay,opt);
	}
	//
	//  ��Ƶͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ� ��Ƶʱ��
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//
	int64_t correctAudioDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt) {
		return delay;
	}
	//
	//  ͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ�
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//
	int64_t correctSyncDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt) {
		if (opt) {
			*opt = CORRECT_DELAY;
		}

		if (master == NULL || slave == NULL) {
			return delay;
		}

		int64_t diff = diffClock(master, slave);
		if (diff == 0) {
			return delay;
		}

		//
		//  �Ƿ���Ƶ�����
		//
		bool isFast = diff < 0;
		{
			//
			//  �Ƿ���Ҫ��֡������Ƶ��ͬ�����̫����
			//
			bool dropFrame = false;
			if (isFast && diff <= -SYNC_RESYNC_THRESHOLD_US) {
				delay = delay * 2;
				dropFrame = true;
			}
			else if (!isFast && diff >= SYNC_RESYNC_THRESHOLD_US) {
				delay = 0;
				dropFrame = true;
			}

			if (dropFrame) {				
				FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP,
					"sync: resync diff=%" lld64 " fast=%d",
					diff, isFast);

				if (opt)
					*opt = RESYNC;
				return delay;
			}
		}

		//
		//  ����Ҫͬ��
		//
		int64_t threshold = FFL_MIN(SYNC_MIN_THRESHOLD_US, delay);
		if ((isFast && diff < -threshold) ||
			(!isFast && diff > threshold)) {
			FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP,
				"sync: not adjust diff=%" lld64 " fast=%d",
				diff, isFast);
			return delay;
		}


		{
			//
			//  �Ƿ�֡���
			//
			bool correct = false;
			if (isFast && diff < -SYNC_MAX_THRESHOLD_US) {
				delay = delay*2;
				correct = true;
			}
			else if (!isFast && diff > SYNC_MAX_THRESHOLD_US) {
				delay = 0;
				correct = true;
			}

			if (correct) {
				FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP,
					"sync: dropframe diff=%" lld64 " fast=%d",
					diff,isFast);
				if (opt)
					*opt = DROP_FRAME;
				return delay;
			}
		}

		//
		//  ���߲�ͬ���������ӳ�ʱ��
		//		
		if (isFast) {
			delay = FFL_MIN(delay * 2, delay - diff);			
		}else{
			delay = FFL_MIN(delay / 2, delay - diff);
			delay = FFL_MAX(0, delay);			
		}		
		FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP,
			"sync: crorect diff=%" lld64 " fast=%d",
			diff, isFast);
		return delay;
	}
}

#endif
