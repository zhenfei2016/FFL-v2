/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  Render.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  渲染基类
*
*/

#include "Render.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>
#include "ClockUpdater.hpp"

namespace player {	
	Render::Render():mClockUpdater(NULL),
		mClockUpdaterUserdata(NULL){
	}
	Render::~Render() {
	}
	//
	// 设置时钟的更新器
	//
	void Render::setClockUpdater(ClockUpdater* updater, void* uesrdata) {
		mClockUpdater = updater;
		mClockUpdaterUserdata = uesrdata;
	}
	//
	//  开始绘制tm时间戳的帧
	//
	void Render::updateRenderTimestamp(int64_t tm, int32_t streamId){
		if (mClockUpdater) {
			mClockUpdater->updateClcok(tm, streamId, mClockUpdaterUserdata);
		}
	}
	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > Render::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		FFL::PipelineAsyncConnectorFixSize* conn=new FFL::PipelineAsyncConnectorFixSize(3);
		//conn->getLooper()->setDebug(true);
		return conn;
	}
	
}