/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_PipelineEvent.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/01
 *  
 *  pipeline中的事件处理系统
 *
*/
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>
#include <pipeline/FFL_PipelineEventCallback.hpp>
#include <utils/FFL_Handler.hpp>
#include <utils/FFL_Looper.hpp>


namespace FFL {
	PipelineEvent::PipelineEvent(int32_t eventId) {
		mEventId = eventId;
		setWhat(PipelineEventWhat);
	}
	PipelineEvent::PipelineEvent(PipelineNodeId src, PipelineNodeId dst, int32_t eventId) {
		mSrcNodeId = src;
		mTargetNodeId = dst;
		mEventId = eventId;
	}

	PipelineEvent::PipelineEvent(sp<EventCallback> callback) :mCallback(callback) {
	}
	void PipelineEvent::setEventId(int32_t eventId) {
		mEventId = eventId;
	}
	int32_t PipelineEvent::getEventId() const {
		return mEventId;
	}

	bool PipelineEvent::haveCallback() {
		return !(mCallback.isEmpty());
	}
	//执行回调
	void PipelineEvent::fireCallback() {
		if (!mCallback.isEmpty())
			mCallback->fire(this);
	}

	//
	//
	//
	class PipelineEventLooper::EventHandler : public Handler {
	public:
		EventHandler(PipelineEventLooper* looper) :mLooper(looper) {
		}
	private:
		void onMessageReceived(const sp<Message> &msg) {
			if (msg.isEmpty()) {
				return;
			}

			sp<PipelineEvent> event = ((PipelineEvent*)msg.get());
			if (event->haveCallback()) {
				event->fireCallback();
			}
			else {
				mLooper->onEvent(event);
			}
		}
		PipelineEventLooper* mLooper;
	};

	PipelineEventLooper::PipelineEventLooper(sp<Pipeline> pipeline)
	{
		mPipeline = pipeline;
        mEventHandler = new EventHandler(this);     	
	}
	void PipelineEventLooper::postEvent(const sp<PipelineEvent> &event, int64_t delayUs)
	{
        Looper::handler_id id=mEventHandler->id();        
        if(id==0){
           sp<Pipeline> pipeline = mPipeline.promote();
           if (!pipeline.isEmpty()) {
               id=registerHandler(mEventHandler);
           }
        }
        
		event->setTarget(id);
		sp<Pipeline> pipeline = mPipeline.promote();
		if (!pipeline.isEmpty()) {
			post(event, delayUs);
		}
	}
	void PipelineEventLooper::onEvent(const sp<PipelineEvent> &event)
	{
		sp<Pipeline> pipeline = mPipeline.promote();
		if (!pipeline.isEmpty()) {
			pipeline->onEvent(event);
		}
	}
}