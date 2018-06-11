/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineConnector.cpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/10
*
*  node异步连接器基类
*
*/


#include <pipeline/FFL_PipelineConnector.hpp>
#include <pipeline/FFL_PipelineInput.hpp>
#include <pipeline/FFL_PipelineOutput.hpp>
#include "PipelineLooper.hpp"


namespace FFL {
	PipelineConnector::PipelineConnector():
		mInput(NULL),
		mOutput(NULL)
	{
		FFL_LOG_DEBUG("PipelineConnector::PipelineConnector %p",this);
	}
	PipelineConnector::~PipelineConnector() {
		FFL_LOG_DEBUG("PipelineConnector::~PipelineConnector %p", this);
	}

	bool PipelineConnector::isConnected()
	{
		CMutex::Autolock l(mMutex);
		return (mInput != NULL && mOutput != NULL);
	}
	//
	//  启动，停止这个连接器
	//
	status_t PipelineConnector::startup()
	{
		return FFL_OK;
	}
	status_t PipelineConnector::shutdown()
	{
		return FFL_OK;
	}
	status_t PipelineConnector::requestShutdown() {
		return FFL_OK;
	}
	status_t PipelineConnector::waitShutdown() {
		return FFL_OK;
	}	//
	// 这个connector在哪一个looper中进行处理
	//
	sp<PipelineLooper> PipelineConnector::getLooper()
	{
		return NULL;
	}
	//
	// 转发的消息已经处理了
	//
	void PipelineConnector::consumer(const sp<PipelineMessage> &msg) {
	}
	void PipelineConnector::setInput(PipelineInput* input) {
		CMutex::Autolock l(mMutex);
		mInput = input;
	}
	void PipelineConnector::setOutput(PipelineOutput* output) {
		CMutex::Autolock l(mMutex);
		mOutput = output;
	}
	sp<PipelineInput> PipelineConnector::getInput() {
		CMutex::Autolock l(mMutex);
		return mInput;
	}
	sp<PipelineOutput> PipelineConnector::getOutPut() {
		CMutex::Autolock l(mMutex);
		return mOutput;
	}
    //
    // 开始分派系统消息
    //
    void PipelineConnector::dispathSysMessage(const sp<PipelineMessage>& msg)
    {
        sp<PipelineInput> input = getInput();
        if (!input.isEmpty()) {
            input->dispathSysMessage(msg);
        }        
    }


	SyncPipelineConnector::SyncPipelineConnector() {

	}

	status_t SyncPipelineConnector::tranport(const sp<PipelineMessage> &msg,int64_t delayUs)
	{
		sp<PipelineInput> input = getInput();
		if (!input.isEmpty()) {
            if(isSysPipelineMessage(msg)){
                input->dispathSysMessage(msg);
            }else{
			    input->dispathMessage(msg);
            }
			return FFL_OK;
		}else {
			FFL_LOG_WARNING("SyncPipelineConnector input is null");
			return FFL_ERROR_FAIL;
		}
	}

}
