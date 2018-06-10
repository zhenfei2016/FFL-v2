/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  StreamManager.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/04/11 
 *  
 *  Á÷¹ÜÀíÆ÷
 *
*/
#pragma once

#include "Stream.hpp"
namespace player {
	class IStreamManager {
	public:
		virtual bool addStream(FFL::sp<Stream> stream)=0;
		virtual FFL::sp<Stream> removeStream(uint32_t index)=0;
		virtual FFL::sp<Stream> getStream(uint32_t index)=0;
	};
}
