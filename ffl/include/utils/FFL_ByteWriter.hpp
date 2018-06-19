/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_ByteWriter
*  Created by zhufeifei(34008081@qq.com) on 2018/05/01
*  �ֽ���д�ӿ�
*
*/
#ifndef _FFL_BYTE_WRITER_HPP_
#define _FFL_BYTE_WRITER_HPP_

#include <FFL.h>

namespace FFL{ 
	class ByteWriter {
	public:
		//
		//  �ֽ���д�ӿ�
		//
		virtual bool write1Bytes(int8_t val)=0;
		virtual bool write2Bytes(int16_t val) = 0;
		virtual bool write3Bytes(int32_t val) = 0;
		virtual bool write4Bytes(int32_t val) = 0;
		virtual bool write8Bytes(int64_t val) = 0;
		virtual bool writeString(const std::string& val, uint32_t len) = 0;
		virtual bool writeBytes(const int8_t* data, uint32_t size) = 0;
	};
}


#endif
