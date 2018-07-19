/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_NetSocket.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/14
*  https://github.com/zhenfei2016/FFL-v2.git
*  socket��д
*
*/
#ifndef _FFL_SOCKET_HPP_
#define _FFL_SOCKET_HPP_

#include <utils/FFL_Io.hpp>

namespace FFL {
	class CSocket : public IOReader,public IOWriter {
	public:
		CSocket(NetFD fd);
		virtual ~CSocket();

		NetFD getFd() const		{
			return mFd;
		}
		//
		//  �����ݵ�������
		//  buf:��������ַ
		//  count:��Ҫ���Ĵ�С
		//  pReaded:ʵ���϶��˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t read(uint8_t* buf, size_t count, size_t* pReaded);
		//
		//  д���ݵ��ļ���
		//  buf:��������ַ
		//  count:��������С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t write(void* buf, size_t count, size_t* pWrite) ;
		//
		//  д���ݵ��ļ���
		//  bufVec:��������ַ,����
		//  count:�����С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t writeVec(const BufferVec* bufVec, int count, size_t* pWrite) ;
	protected:
		NetFD mFd;
	};
}
#endif
