/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  Render.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  渲染基类
*
*/
#pragma once
#include "NodeBase.hpp"
namespace player {
	class Render  : public NodeBase{
	public:
		Render();
		~Render();	

		//
		//  获取这个节点处理的延迟
		//
		int64_t getDelayUs() { return 0; }
		//
		//  停止render
		//
		virtual void stop();
	protected:
		//
		//   外部setDataInput时候调用此函数，创建对应conn
		//
		virtual FFL::sp<FFL::PipelineConnector > onCreateConnector(const OutputInterface& output,
			const InputInterface& input,void* userdata);
		
	
	};
}
