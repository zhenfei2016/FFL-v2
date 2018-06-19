/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PlayerEvent.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/11
*
*  ²¥·ÅÆ÷ÊÂ¼þ
*
*/
#include "PlayerEvent.hpp"
#include "NodeBase.hpp"

namespace event{
	void postPlayerEvent(FFL::sp<player::NodeBase> node, EventId eventid) {
		node->postEvent(new PlayerEvent(eventid));
	}

	void postPlayerEvent(FFL::sp<player::NodeBase> node, const FFL::sp<PlayerEvent>& event) {
		node->postEvent(event);
	}
}