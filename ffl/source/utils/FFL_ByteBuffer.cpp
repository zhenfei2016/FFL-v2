/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_ByteBuffer
*  Created by zhufeifei(34008081@qq.com) on 2018/05/06
*  内存管理类
*
*/

#include <utils/FFL_ByteBuffer.hpp>

namespace FFL {
	ByteBuffer::ByteBuffer():mData(0),mSize(0){
		realloc(4096);
	}
	ByteBuffer::~ByteBuffer(){
		FFL_SafeFreeA(mData);
		mSize = 0;
	}

	ByteBuffer::ByteBuffer(uint32_t size) :mData(0), mSize(0) {
		realloc(size);
	}
	//
	//  扩大一下内存空间,如果size小于已经申请的则返回以前的大小
	//
	uint32_t ByteBuffer::realloc(uint32_t size) {
		if (size <= mSize) {
			return mSize;
		}

		uint8_t* data=(uint8_t*)FFL_malloc(size);
		if(!data){
			FFL_LOG_ERROR("ByteBuffer::realloc fail");
			return mSize;
		}

		if (mSize) {
			memcpy(data, mData, mSize);
		}

		mData = data;
		mSize = size;
		return size;
	}

	uint8_t* ByteBuffer::data() const {
		return mData;
	}
	uint32_t ByteBuffer::size() const {
		return mSize;
	}
}