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

namespace player {

	class Decoder;
	class Composer;
	enum StreamType{
		STREAM_TYPE_VIDEO =1,
		STREAM_TYPE_AUDIO = 2,
	};


	class Stream : public FFL::RefBase {
	public:
		Stream(uint32_t index);
		Stream();
		virtual ~Stream();

	public:
		//
		// 获取流索引
		//
		uint32_t getIndex() const;
		void setStreamIndex(uint32_t index);

		uint32_t getStreamType() const;
		void setStreamType(StreamType type);

		virtual void getTimebase(FFL::TimeBase& units);
		
		//
		//  获取音频采样率
		//
		virtual uint32_t getAudioSamples();
		//
		//  设置解码后数据合成器
		//
		void setComposer(FFL::sp<Composer> composer);
		FFL::sp<Composer> getComposer() const;

		//
		//  获取这个流的解码节点
		//
		FFL::sp<Decoder> getDecoder() const;
	public:
		//
		//  创建这个流需要的解码器
		//
		virtual  FFL::sp<Decoder> createDecoder();

		//
		//  搭建解码流水线,并且启动
		//
		virtual status_t build();
	protected:
		uint32_t mStreamindex;

		uint32_t mStreamType;

		FFL::TimeBase mUnits;
		//
		//  流的时钟
		//
		FFL::sp<FFL::Clock> mClock;
		//
		// 这个流对应的解码器，解码后的合成器
		//
		FFL::sp<Decoder> mDecoder;
		FFL::sp<Composer> mComposer;
	public:
		//
		//  解码后的数据，数据源接口
		//
		OutputInterface mDataSource;
	};
}