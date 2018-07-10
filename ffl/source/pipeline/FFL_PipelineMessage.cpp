/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineMessage.cpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/23
*
*  pipelineMessage消息
*
*/

#include <pipeline/FFL_PipelineMessage.hpp>
#include <utils/FFL_Message.hpp>

#define ENABLE_TRACKBACK 

namespace FFL {
	PipelineMessageTrackbackId::PipelineMessageTrackbackId() {
		mId = 0;
		mStartTimeUs=0;
		mEndTimeUs=0;
		//
		// 处理的线程id
		//
		mThreadId=0;
	}
	PipelineMessageTrackbackId::~PipelineMessageTrackbackId() {

	}
	void PipelineMessageTrackbackId::setId(int32_t id) {
		mId = id;
	}

	void PipelineMessageTrackbackId::beginTrack() {
		beginTrack(FFL_CurrentThreadID());
	}
	void PipelineMessageTrackbackId::endTrack() {
		endTrack(FFL_CurrentThreadID());
	}
	void PipelineMessageTrackbackId::beginTrack(int32_t tid) {
		mStartTimeUs = FFL_getNowUs();
		mThreadId = tid;
	}
	void PipelineMessageTrackbackId::endTrack(int32_t tid) {
		mEndTimeUs = FFL_getNowUs();
	}	
	void PipelineMessageTrackbackId::printf() {
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PipelineMessage::PipelineMessage() :
		mType(0),
		mPayload(NULL),
		mConsumeListener(NULL)
	{
		mMessageUniqueId = 0;
        mPayloadAutoDel=false;	
		mParam1 = 0;
		mParam2 = 0;
	}
	//
	//  消息类型
	//
	PipelineMessage::PipelineMessage(uint32_t msgType) 	:
		mType(msgType),
		mPayload(0) ,
        mConsumeListener(NULL)
	{
        mPayloadAutoDel=false;
        FFL_LOG_DEBUG("PipelineMessage::PipelineMessage(%d) %p",mType,this);
	}
    
    PipelineMessage::~PipelineMessage(){
         FFL_LOG_DEBUG("PipelineMessage::~PipelineMessage(%d) %p",mType,this);
        if(mPayloadAutoDel && mPayload){
            delete mPayload;
            mPayload=NULL;
        }
    }
	//
	//  消息类型
	//
	uint32_t PipelineMessage::getType() const 
	{
		return mType;
	}
	void PipelineMessage::setType(uint32_t type)
	{
		mType = type;
	}
	//
	//  这个消息消耗监听器
	//  当消息被消耗完的时候会执行对应listener
	//
	void PipelineMessage::setConsumeListener(MessageConsumeListener* listener)
	{
		mConsumeListener = listener;
	}
	//
	//  这个消息已经被消耗了
	//  消息正常的被handler处理后触发consume的调用
	//  消息cancel也会触发consume
	//
	void PipelineMessage::consume(void*)
	{
		mMessageUniqueId = 0;		
		if (mPayload) {			
			mPayload->consume();
		}

		//
		//  通知监听器
		//
		if (mConsumeListener) {
			mConsumeListener->onConsume(this);
		}
	}      

	//
	//   设置负载信息
	//
	void PipelineMessage::setPayload(PipelineMessagePayload* payload,bool autoDel) {
        
        if(mPayloadAutoDel && mPayload && mPayload !=payload){
            delete mPayload;
            mPayload=NULL;
            
        }
		mPayload = payload;
        mPayloadAutoDel=autoDel;
	}
	PipelineMessagePayload* PipelineMessage::getPayload() const
	{
		return mPayload;
	}
	//
	// 设置这个消息带的参数，
	//
	void PipelineMessage::setParams(int64_t param1, int64_t param2) {
		mParam1 = param1;
		mParam2 = param2;
	}
	int64_t PipelineMessage::getParam1() const {
		return mParam1;
	}
	int64_t PipelineMessage::getParam2() const {
		return mParam2;
	}
	//
	//  获取，这个Message的追溯信息，主要用于调试，分析
	//  因为，一条消息可能经过n多的node进行处理
	//
	PipelineMessageTrackbackId&  PipelineMessage::getTracebackInfo() {
		return mTraceBackInfo ;
	}
   

    //
    //  是否系统消息
    //
    bool isPipelineMessage(sp<Message> msg){
        uint32_t what = msg->what();
        return (what ==MSGT_PIPELINE);
    }
	//
	// 获取pipelineMessage消息
	//
	sp<PipelineMessage> getPipelineMessage(sp<Message> msg) {
		if (msg.isEmpty()) {
			return NULL;
		}

		uint32_t what = msg->what();
		if (what == MSGT_PIPELINE || (what >= MSGT_SYS_BEGIN && what <= MSGT_SYS_END)) {
			sp<PipelineMessage> message = (PipelineMessage*)(msg->mObj.get());
			return message;
		}		
		return NULL;
	}
	//
	//  是否系统消息
	//
	bool isSysPipelineMessage(sp<PipelineMessage> msg) {
		uint32_t what = msg->getType();
		return (what >= MSGT_SYS_BEGIN && what <= MSGT_SYS_END);
	}
    //
    //  创建指定类型的系统消息
    //
    sp<PipelineMessage> createSysPipelineMessage(uint32_t type){
        sp<PipelineMessage> msg=new PipelineMessage(type);
        msg->setType(type);
        return  msg;
    }

}
