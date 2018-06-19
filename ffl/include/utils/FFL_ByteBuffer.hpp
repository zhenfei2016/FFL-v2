/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_ByteBuffer   
*  Created by zhufeifei(34008081@qq.com) on 2018/05/06 
*  �ڴ������
*
*/
#ifndef _FFL_BYTE_BUFFER_HPP_
#define _FFL_BYTE_BUFFER_HPP_

#include <FFL.h>

namespace FFL{
	class ByteStream;
	class ByteBuffer {
	public:
		ByteBuffer();
		ByteBuffer(uint32_t size);
		~ByteBuffer();
		
		//
		// ��������һ�¿ռ�
		//
		uint32_t alloc(uint32_t size);
		//
		//  ����һ���ڴ�ռ�,���sizeС���Ѿ�������򷵻���ǰ�Ĵ�С
		//
		uint32_t realloc(uint32_t size);
		

		uint8_t* data() const;
		uint32_t size() const;
		
		//
		//  ��ȡ����ڴ�Ĳ�����
		//
		ByteStream* getByteStream();
	private:
		uint8_t* mData;
		uint32_t mSize;

		ByteStream* mStream;
	};
}


#endif

