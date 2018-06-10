/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  TimestampExtrapolator
*  Created by zhufeifei(34008081@qq.com) on 2018/05/26
*  时间戳推算
*/
#include "TimestampExtrapolator.hpp"

namespace player {
	TimestampExtrapolator::TimestampExtrapolator(FFL::sp<FFL::Clock> clock) :
		mOriginLocalTimeUs(0),
		mOriginTimestampUs(0),
		mClock(clock){
		mLastTimestamp = 0;
		mLastTimestampLocalTimeUs = 0;
	}

	TimestampExtrapolator::~TimestampExtrapolator() {
	}
	//
	// 重置
	//
	void TimestampExtrapolator::reset() {
		mOriginLocalTimeUs = 0;
		mOriginTimestampUs = 0;
		mLastTimestamp = 0;
		mLastTimestampLocalTimeUs = 0;
	}
	//
	//  重置一下时钟，当前时间戳对应当前的时间点
	//
	void TimestampExtrapolator::update(int64_t timestamp, FFL::TimeBase units) {
   		mOriginTimestampUs = timestampToUs(timestamp, units);
        if(mOriginTimestampUs<0){
            int i=0;
            i++;
        }
		mOriginLocalTimeUs = mClock->nowUs();
	}
	//
	//  获取当前的时间
	//
	int64_t TimestampExtrapolator::getNowUs() {
		return mClock->nowUs();
	}
	//
	// 获取这个时间戳到现在的相对时间
	//
	int64_t TimestampExtrapolator::getDelayUsRelativeNow(int64_t timestamp, FFL::TimeBase units) {
		int64_t d1 = 0;
		int64_t d2 = 0;
		int64_t now = 0;
		if (mOriginLocalTimeUs != 0 && mOriginTimestampUs != 0) {
			int64_t t2 = timestampToUs(timestamp, units);
			int64_t t1 = now= mClock->nowUs();
			//
			//  返回相对的差值
			//	
			d1 = t1 - mOriginLocalTimeUs;
			d2 = t2 - mOriginTimestampUs;
			if (d2 < 0 || d1 < 0) {
                FFL_LOG_WARNING("Extrapolator: d2<0 || d1<0  d2=%" lld64 " d1=%" lld64,
                                 d2,d1);
				d2 = 0;
				d1 = 0;
                
			}
		}

		FFL_LOG_CRIT("Extrapolator::getDelay ts=%" lld64 " d2=%" lld64 " d1=%" lld64 " delay=%" lld64,
			timestamp,
			d2,
			d1,
			d2-d1);

		int64_t delay = d2 - d1;
        {
            int64_t timestampLocalTimeUs= now + delay;
            //
            //  支持的最大偏差 , 2s
            //
            int64_t maxDeviation=2 * 1000 * 1000;
            int64_t diff=(timestampLocalTimeUs>mLastTimestampLocalTimeUs)?
                         (timestampLocalTimeUs-mLastTimestampLocalTimeUs):
                         (mLastTimestampLocalTimeUs-timestampLocalTimeUs);
            if (diff > maxDeviation) {
                //
                //   2桢数据相差2s以上了，这个有点异常了 ,todo需要通知丢帧
                //
                delay=d1 = d2 = 0;
                reset();
                update(timestamp, units);
                FFL_LOG_CRIT("reset TimestampExtrapolator");
            }
        }

		if (mLastTimestampLocalTimeUs != 0 && mLastTimestamp != 0) {
			//
			// 跟时间戳计算的延迟比较一下，看有多少的偏差
            // 延迟是否大于15毫秒了
			//
            int64_t maxDeviation= 1000 * 15;
			int64_t diff = getDelayUsTimestamp(timestamp, units);
			diff -= (now - mLastTimestampLocalTimeUs);
			if (abs(diff - delay) < maxDeviation) {
				delay = diff;
			}else {
				//
				//  这个需要怎么处理，音视频相对主时钟的时间比较长了
				//
				FFL_LOG_CRIT("TimestampExtrapolator diff> 15ms =%" lld64 , diff - delay);
				
			}
		}
		
		mLastTimestamp = timestamp;
		mLastTimestampLocalTimeUs = now + delay;
		return delay;
	}

	int64_t TimestampExtrapolator::timestampToUs(int64_t timestamp, FFL::TimeBase units) {
		if (units.mDen == 0) {
			return timestamp;
		}

		//
		//  时间戳一个刻度对应多少毫秒
		//
		double dus = ((double)(1000 * 1000 * units.mNum)) / units.mDen;
		return (int64_t)(timestamp* dus);
	}
	//
	//  根据最后一次的时间戳计算相对延迟值
	//
	int64_t TimestampExtrapolator::getDelayUsTimestamp(int64_t timestamp, FFL::TimeBase units) {
		if (mLastTimestamp == 0 || mLastTimestampLocalTimeUs == 0) {
			return 0;
		}

		if (timestamp == mLastTimestamp) {
			return 0;
		}

		int64_t diffUs=timestampToUs(timestamp - mLastTimestamp, units);

		return diffUs;
	}
		

}
