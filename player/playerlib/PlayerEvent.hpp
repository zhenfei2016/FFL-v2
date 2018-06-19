/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PlayerEvent.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/11
*
*  播放器事件
*
*/
#pragma once
#include <pipeline/FFL_PipelineEventCallback.hpp>
#include <pipeline/FFL_Pipeline.hpp>

namespace player {
	class NodeBase;
}

namespace event{
   enum EventId{
	   //
	   //  未知的错误
	   //
	   EVENT_ERROR =1,

	   EVENT_PREPARED,
	   //
	   //  视频大小改变了
	   //
	   EVENT_VIDEO_SIZE_CAHNGED,
	   //
	   // 定位到指定位置
	   //
	   EVENT_SEEK_START,
	   EVENT_SEEK_END,
	   //
	   // 开始，结束缓冲中
	   //
	   EVENT_BUFFERING_START,
	   EVENT_BUFFERING_END,
	   //
	   //  开始渲染视频第一，最后一帧
	   //
	   EVENT_VIDEO_RENDER_FIRST_FRAME,
	   EVENT_VIDEO_RENDER_LAST_FRAME,
	   //
	   //  开始渲染音频第一，最后一帧
	   //
	   EVENT_AUDIO_RENDER_FIRST_FRAME,
	   EVENT_AUDIO_RENDER_LAST_FRAME,
   };

   class PlayerEvent : public FFL::PipelineEvent{
   public:
	   PlayerEvent(EventId eventid) :FFL::PipelineEvent((int32_t)eventid){
	   }

	   int32_t mInt32Parma1;
	   int32_t mInt32Parma2;

	   int64_t mInt64Param1;
	   int64_t mInt64Param2;

   };
   //
   //  发送一些播放器事件
   //   
   void postPlayerEvent(FFL::sp<player::NodeBase> node, EventId eventid);
   void postPlayerEvent(FFL::sp<player::NodeBase> node, const FFL::sp<PlayerEvent>& event);
}