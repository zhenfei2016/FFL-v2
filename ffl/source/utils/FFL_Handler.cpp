/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Handler.cpp
*  Created by zhufeifei(34008081@qq.com) on 2017/11/25
*  �ο� Android Open Source Project �����޸ĵ�
*  ������Ϣ�Ļص�����
*
*/

#include <utils/FFL_Handler.hpp>


namespace FFL {	
	Handler::Handler(): mID(0) {
	}

	Looper::handler_id Handler::id() const{
		return mID;
	}

	IdleHandler::IdleHandler() {
	}
	IdleHandler::~IdleHandler() {
	}


}
