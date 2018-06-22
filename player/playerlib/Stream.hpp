/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  Stream.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/04/01 
 *  
 *  流信息
 *
*/
#pragma once

#include <ref/FFL_Ref.hpp>
#include <utils/FFL_Clock.hpp>
#include "NodeBase.hpp"
#include "SyncClock.hpp"

namespace player {

	class Decoder;
	enum StreamType{
		STREAM_TYPE_VIDEO = 1,
		STREAM_TYPE_AUDIO = 2,
		STREAM_TYPE_SUBTITLE=3,
		STREAM_TYPE_OTHER = 4,
	};

	class Stream : public FFL::RefBase {
	public:		
		Stream();
		virtual ~Stream();
	public:
		//
		// 获取流索引
		//
		virtual uint32_t getIndex() const=0;
		//
		// 获取流类型
		//
		virtual uint32_t getStreamType() const=0;
		//
		// 获取流开始的时间戳，以getTimebase为单位的
		//
		virtual int64_t getStartTime() const =0;
		//
		//  获取时基单位
		//
		virtual void getTimebase(FFL::TimeBase& tm) const  =0;
		//
		//  创建这个流需要的解码器
		//
		virtual FFL::sp<Decoder> createDecoder()=0;	
	public:
		//
		//  获取同步时钟
		//
		SyncClock* getSyncClock();		
	protected:
		//
		//  进行同步的时钟
		//
		SyncClock* mSyncClock;
	public:
		//
		//  这个流数据的输入源
		//
		OutputInterface mSource;
	};
}