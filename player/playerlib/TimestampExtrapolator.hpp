/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  TimestampExtrapolator   
*  Created by zhufeifei(34008081@qq.com) on 2018/05/26 
*
*  音视频同步的时间戳推算器，推算这个timestamp需要延迟多长时间开始播放
*  
*  主时钟需要一直 update();
*  需要计算延迟时长的  getDelayUsRelativeNow(),返回相对现在需要延迟多长时间
*/
#ifndef _TIMESTAMPEXTRAPOLATOR_HPP_
#define _TIMESTAMPEXTRAPOLATOR_HPP_

#include <FFL.h>
#include <utils/FFL_Clock.hpp>
#include "Stream.hpp"

namespace player {
	class TimestampExtrapolator {
	public:
		//
		//  clock: 依赖这一个时钟，进行时间的推算
		//  可以改变这个clock的速度，这样计算出来的延迟值就相应的改变了
		// 
		TimestampExtrapolator(FFL::sp<FFL::Clock> clock);
		~TimestampExtrapolator();
		//
		// 重置
		//
		void reset();
		//
		//  更新这个推算器的依赖时间点，当前时间戳对应当前的时间点
		//  这应该是一个需要同步到这个上面的主时钟进行的
		//
		void update(int64_t timestamp, FFL::TimeBase units);
		//
		//  获取当前的时间
		//
		int64_t getNowUs();
		//
		// 获取这个时间戳相对当前需要延迟的时长us
		//
		int64_t getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase units);
	private:
		//
		//  时间戳单位转到us值
		//
		int64_t timestampToUs(int64_t timestamp, FFL::TimeBase units);
		//
		//  根据最后一次的时间戳计算相对延迟值
		//
		int64_t getDelayUsTimestamp(int64_t timestamp, FFL::TimeBase units);
	private:
		//
		//  源点的时间点，其他时间对视相对这个的偏移值  ，本地系统的时间轴
		//
		int64_t mOriginLocalTimeUs;
		//
		//  外部输出的时间戳的开始时间点，外部的时间轴
		//
		int64_t mOriginTimestampUs;
		//
		//  最后一次获取delay的时间戳，和计算的clock时间点
		//
		int64_t mLastTimestamp;
		int64_t mLastTimestampLocalTimeUs;
	public:
		FFL::sp<FFL::Clock> mClock;
	};
}

#endif
