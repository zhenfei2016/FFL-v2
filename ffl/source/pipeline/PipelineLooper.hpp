/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineLooper.cpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/16
*
*  pipeline消息系统的looper,处理pipelineMessage
*
*/

#ifndef __FFL_PIPELINE_MESSAGE_LOOPER_HPP_
#define __FFL_PIPELINE_MESSAGE_LOOPER_HPP_

#include <utils/FFL_Looper.hpp>

namespace FFL{	
	class PipelineMessage;
	class PipelineLooper : public Looper { 		
	public:
		PipelineLooper(sp<Clock> clock);

		//
		// 发送一个消息
		//
		bool postMessage(const sp<PipelineMessage> &msg, Looper::handler_id target,int64_t delayUs=0);
		//
		// 发送退出looper消息
		//
		void postQuitMessage();
		//
		// 清空等待执行的消息
		//
		void flushMessage();
		//
		// 移除这个消息，返回是否成功
		//
		bool removeMessage(const sp<PipelineMessage> &msg);

		void clearMessageQueue();
	
	};
}


#endif
