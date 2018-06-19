/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  Decoder.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  解码基类
*
*/
#include "Decoder.hpp"
#include "Composer.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>

namespace player {
	Decoder::Decoder(){
	}
	Decoder::~Decoder() {
	}	
	//
	//  获取这个节点处理的延迟
	//
	int64_t Decoder::getDelayUs() {
		return 0;
	}
	//
	//  设置这个解码器的输出到那个一个合成器中
	//
	void Decoder::setOutputComposer(FFL::sp<Composer> composer) {
		mComposer = composer;
		if (!mComposer.isEmpty()) {
			mFrameOutput = createOutputInterface();
			InputInterface input;
            FFL::String name;
            getName(name);
			mComposer->connectSource(mFrameOutput,name.c_str(),input,0);
		}
	}
	
	FFL::sp<Composer>  Decoder::getComposer() {
		return mComposer;
	}

	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > Decoder::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		return new FFL::PipelineAsyncConnectorFixSize(2);
	}

}
