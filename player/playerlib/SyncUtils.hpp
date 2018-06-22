/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  SyncUtils.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/21
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ����ͬ����������
*
*/
#include "SyncClock.hpp"

namespace player {
	enum OptMode {
		//
		//  ����ʹ���������ӳ�
		//
		CORRECT_DELAY = 0,
		//
		//  ���̫�����½���ͬ��
		//
		RESYNC = 1,
		//
		//  ������ǰ��
		//
		DROP_FRAME = 2,
	};
	//
	//  ��Ƶͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ� ��Ƶʱ��
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//  
	//
	int64_t correctVideoDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt);
	//
	//  ��Ƶͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ� ��Ƶʱ��
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//
	int64_t correctAudioDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt);
	//
	//  ͬ����master�ϣ�����һ���ӳ�ֵ
	//  master:ͬ����ʱ��
	//  slave :��ʱ�ӣ�
	//  delay :ԭʼ�ӳ�ֵ
	//  opt :���ؽ���Ĵ���ģʽ��
	//  ������������ӳ�ֵ
	//
	int64_t correctSyncDelay(SyncClock* master, SyncClock* slave, int64_t delay, OptMode* opt);

}