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
	class PlayerCore;
}

namespace event{
   enum EventId{
	   //
	   //  δ֪�Ĵ���
	   //
	   EVENT_ERROR =1,

	   //
	   //  prepare�����  
	   //  getParam1() = 1  :�ɹ�
	   //  getParam1() = 0  :�쳣��ʧ����
	   //
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
	   PlayerEvent(EventId eventid,int64_t param1,int64_t param2) 
		   :FFL::PipelineEvent((int32_t)eventid),
		   mParam1(param1),
		   mParam2(param2){
	   }	      

	   inline int64_t getParam1() const {
		   return mParam1;
	   }
	   inline int64_t getParam2() const {
		   return mParam2;
	   }

	   int64_t mParam1;
	   int64_t mParam2;

   };
   //
   //  ����һЩ�������¼�
   //   
   void postPlayerEvent(FFL::sp<player::NodeBase> node, EventId eventid);
   void postPlayerEvent(FFL::sp<player::NodeBase> node, const FFL::sp<PlayerEvent>& event);

   //
   //  ���Ͳ������¼�
   //
   void postPlayerEvent(player::PlayerCore* core, EventId eventid, int64_t param1, int64_t param2);
}