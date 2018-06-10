/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  Stream.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/01
*
*  流信息
*
*/

#include "Stream.hpp"
#include "Decoder.hpp"
#include "Composer.hpp"
#include "FFL_Player.hpp"
#include <pipeline/FFL_Pipeline.hpp>

namespace player {
	Stream::Stream():
		mStreamindex(-1),
		mStreamType(0){

	}
	Stream::Stream(uint32_t index) :mStreamindex(index), mStreamType(0){

	}
	Stream::~Stream() {

	}
	//
	// 获取流索引
	//
	uint32_t Stream::getIndex() const {
		return mStreamindex;
	}
	void Stream::setStreamIndex(uint32_t index) {
		mStreamindex = index;
	}
	uint32_t Stream::getStreamType() const {
		return mStreamType;
	}
	void Stream::setStreamType(StreamType type) {
		mStreamType = type;
	}

	void Stream::getTimebase(FFL::TimeBase& units) {
		units = mUnits;
	}
	//
	//  获取音频采样率
	//
	uint32_t Stream::getAudioSamples() {
		return 0;
	}
	//
	//  设置解码后数据合成器
	//
	void Stream::setComposer(FFL::sp<Composer> composer) {
		mComposer = composer;
	}
	FFL::sp<Composer> Stream::getComposer() const{
		return mComposer;
	}


	FFL::sp<Decoder> Stream::getDecoder() const {
		return mDecoder;
	}
	//
	//  创建这个流需要的解码器
	//
	FFL::sp<Decoder> Stream::createDecoder() {
		return 0;
	}
	//
	//  搭建解码流水线,
	//
	status_t Stream::build() {
		FFL::String name;
		name += ":input";

		InputInterface input;		
		mDecoder->connectSource(mDataSource,name.c_str(),input,NULL);
		mDecoder->setOutputComposer(mComposer);
		
		FFL::sp<FFL::Pipeline> pipeline=mDecoder->getOwner()->getPipeline();
		pipeline->startup(mDecoder->getNodeId());
		pipeline->startup(mComposer->getNodeId());
		return FFL_OK;
	}

}
