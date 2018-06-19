/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PlayerEvent.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/11
*
*  �������¼�
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
	   //  δ֪�Ĵ���
	   //
	   EVENT_ERROR =1,

	   EVENT_PREPARED,
	   //
	   //  ��Ƶ��С�ı���
	   //
	   EVENT_VIDEO_SIZE_CAHNGED,
	   //
	   // ��λ��ָ��λ��
	   //
	   EVENT_SEEK_START,
	   EVENT_SEEK_END,
	   //
	   // ��ʼ������������
	   //
	   EVENT_BUFFERING_START,
	   EVENT_BUFFERING_END,
	   //
	   //  ��ʼ��Ⱦ��Ƶ��һ�����һ֡
	   //
	   EVENT_VIDEO_RENDER_FIRST_FRAME,
	   EVENT_VIDEO_RENDER_LAST_FRAME,
	   //
	   //  ��ʼ��Ⱦ��Ƶ��һ�����һ֡
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
   //  ����һЩ�������¼�
   //   
   void postPlayerEvent(FFL::sp<player::NodeBase> node, EventId eventid);
   void postPlayerEvent(FFL::sp<player::NodeBase> node, const FFL::sp<PlayerEvent>& event);
}