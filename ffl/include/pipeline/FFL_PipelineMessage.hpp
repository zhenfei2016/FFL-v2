/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineMessage.hpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/23
*
*  pipelineMessage消息
*
*/
#ifndef __FFL_PIPELINE_MESSAGE_HPP_
#define __FFL_PIPELINE_MESSAGE_HPP_


#include <utils/FFL_Message.hpp>

namespace FFL{
	class PipelineAsyncConnector;

	//
	//   PipelineMessage携带的负载信息
	//
	class PipelineMessagePayload {
	public:
        virtual ~PipelineMessagePayload(){}
		//
		//  已经处理完成了，可以回收了		
		//
		virtual void consume()=0;
	};

	class MessageConsumeListener {
	public:
		virtual void onConsume(FFL::PipelineMessage* msg)=0;
	};

	//
	//  pipelinemessage的追溯信息，可以进行消息处理的性能分析
	//
	class PipelineMessageTrackbackId {
	public:
		PipelineMessageTrackbackId();
		~PipelineMessageTrackbackId();
	public:
		void setId(int32_t id);
		void beginTrack();
		void endTrack();
	public:
		virtual void beginTrack(int32_t tid);
		virtual void endTrack(int32_t tid);
		virtual void printf();
	private:
		//
		//  全局唯一的
		//
		int32_t mId;
		//
		//  开始结束时间，2个的差值为处理事件
		//
		int64_t mStartTimeUs;
		int64_t mEndTimeUs;
		//
		// 处理的线程id
		//
		int32_t mThreadId;
	};



	class PipelineMessage : public Serializable {
		friend class PipelineAsyncConnector;		
		friend class PipelineLooper;
	public:
		PipelineMessage();	
		//
		//  消息类型
		//
		PipelineMessage(uint32_t msgType);

        ~PipelineMessage();
		//
		//  消息类型
		//
		uint32_t getType() const;
		void setType(uint32_t type);
		//
		//  这个消息消耗监听器
		//  当消息被消耗完的时候会执行对应listener
		//
		void setConsumeListener(MessageConsumeListener* listener);
		//
		//  这个消息已经被消耗了
        //  消息正常的被handler处理后触发consume的调用
		//  消息cancel也会触发consume
		//
		virtual void consume(void* );
		//
		//   设置负载信息
		//
		void setPayload(PipelineMessagePayload* payload,bool autoDel=true);
		PipelineMessagePayload* getPayload() const;	
		//
		// 设置这个消息带的参数，
		//
		void setParams(int64_t param1, int64_t param2);
		int64_t getParam1() const;
		int64_t getParam2() const;
	private:
		//
		//  消息类型，可以根据这个类型解析出Playload的值
		//
		uint32_t mType;

		//
		//  消息的负载信息
		//
		PipelineMessagePayload* mPayload;
        bool mPayloadAutoDel;
		//
		//  参数
		//
		int64_t mParam1;
		int64_t mParam2;
		//
		//  监听器
		//
		MessageConsumeListener* mConsumeListener;
		//
		//Message::uniqueId() 
		//保存Message的唯一id
		//
		uint32_t mMessageUniqueId;
	public:
		//
		//  获取，这个Message的追溯信息，主要用于调试，分析
		//  因为，一条消息可能经过n多的node进行处理
		//		
		PipelineMessageTrackbackId& getTracebackInfo();
	protected:
		PipelineMessageTrackbackId mTraceBackInfo;
	};
   
	//
	//  自动打印trackback
	//
	class AutoPrintfTrackback {
	public:
		inline AutoPrintfTrackback(PipelineMessageTrackbackId& info):mId(info){			
			mId.beginTrack();
		}
		inline ~AutoPrintfTrackback() {
			mId.endTrack();
			mId.printf();
		}

		PipelineMessageTrackbackId& mId;
	};
	
    //
    //  是否PipelineMessage消息
    //
    bool isPipelineMessage(sp<Message> msg);    
	//
	// 获取pipelineMessage消息
	//
	sp<PipelineMessage> getPipelineMessage(sp<Message> msg);
	//
	//  是否系统消息
	//
	bool isSysPipelineMessage(sp<PipelineMessage> msg);

    //
    //  创建指定类型的系统消息
    //
    sp<PipelineMessage> createSysPipelineMessage(uint32_t type);
    
}


#endif
