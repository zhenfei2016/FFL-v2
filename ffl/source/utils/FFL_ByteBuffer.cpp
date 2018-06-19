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

#include <utils/FFL_ByteBuffer.hpp>

namespace FFL {
	ByteBuffer::ByteBuffer():mData(0),mSize(0), mStream(0){
		mStream = new ByteStream();
		alloc(4096);
	}
	ByteBuffer::~ByteBuffer(){
		FFL_free(mData);		
		mSize = 0;
		FFL_SafeFree(mStream);
	}

	ByteBuffer::ByteBuffer(uint32_t size) :mData(0), mSize(0) {
		alloc(size);
	}
	//
	// ��������һ�¿ռ�
	//
	uint32_t ByteBuffer::alloc(uint32_t size) {
		if (size <= mSize) {
			return mSize;
		}

		uint8_t* data = (uint8_t*)FFL_malloc(size);
		if (!data) {
			FFL_LOG_ERROR("ByteBuffer::alloc fail");
			return mSize;
		}	

		FFL_free(mData);
		mData = data;
		mSize = size;

		mStream->setData(mData, 0, mSize);
		return size;
	}
	//
	//  ����һ���ڴ�ռ�,���sizeС���Ѿ�������򷵻���ǰ�Ĵ�С
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

		FFL_free(mData);
		mData = data;
		mSize = size;

		mStream->setData(mData, 0, mSize);
		return size;
	}

	uint8_t* ByteBuffer::data() const {
		return mData;
	}
	uint32_t ByteBuffer::size() const {
		return mSize;
	}
	//
	//  ��ȡ����ڴ�Ĳ�����
	//
	ByteStream* ByteBuffer::getByteStream() {
		return mStream;
	}
}