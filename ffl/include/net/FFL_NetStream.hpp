/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_NetStream.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/17 
*  https://github.com/zhenfei2016/FFL-v2.git
*  ����������ȡ�ӿڣ��ڲ��������ݣ��������parser
*
*/
#ifndef _FFL_NET_STREAM_READER_HPP_
#define _FFL_NET_STREAM_READER_HPP_

#include <utils/FFL_ByteReader.hpp>
#include <net/FFL_NetSocket.hpp>

namespace FFL {
	class ByteBuffer;

	class NetStreamReader : public ByteReader {
	public:
		NetStreamReader(CSocket* socket );
		~NetStreamReader();
	public:
		//
		//  ��ȡ����ָ��
		//
		uint8_t* getData() const;
		//
		//  ��ȡ�洢�����ݴ�С
		//
		uint32_t getSize() const;
		//
		//  ��ȡ���ݵĿ�ʼΪZHi
		//
		uint32_t getPosition() const;
		//
		//  ������ô���ȵ�����
		//
		void skip(int32_t length);
		//
		//  socket��ȡ�������
		//
		status_t pull(int32_t expect);
		//
		//  ByteReader ��д
		//
		bool read1Bytes(int8_t& val);
		bool read2Bytes(int16_t& val);
		bool read3Bytes(int32_t& val);
		bool read4Bytes(int32_t& val);
		bool read8Bytes(int64_t& val);
		bool readString(String& val, uint32_t len);
		bool readBytes(int8_t* data, uint32_t size);
	private:
		ByteBuffer* mBuffer;
		uint32_t mPosition;
		uint32_t mSize;

		CSocket* mSocket;
	};
}

#endif