/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Serializable.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  �������л��Ľӿ�
*/

#include <utils/FFL_Serializable.hpp>
#include <utils/FFL_ByteStream.hpp>

namespace FFL{
	Serializable::Serializable(){}
	Serializable::~Serializable(){}

	void Serializable::serialization(ByteStream& stream) {
	}	
	status_t Serializable::deserialization(ByteStream& stream)
	{
		return FFL_NOT_IMPLEMENT;
	}
}
