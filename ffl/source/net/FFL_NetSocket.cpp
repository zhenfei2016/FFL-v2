/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_NetSocket.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/07/14
*  https://github.com/zhenfei2016/FFL-v2.git
*  socket��д
*
*/

#include <net/base/FFL_Net.h>
#include <net/FFL_NetSocket.hpp>


namespace FFL {
	CSocket::CSocket(NetFD fd):mFd(fd){
	}
	CSocket::~CSocket() {
	}
	//
	//  �����ݵ�������
	//  buf:��������ַ
	//  count:��Ҫ���Ĵ�С
	//  pReaded:ʵ���϶��˶�������
	//  ���ش�����  �� FFL_OK��ʾ�ɹ�
	//
	status_t CSocket::read(uint8_t* buf, size_t count, size_t* pReaded) {
		return FFL_socketRead(mFd, buf, count, pReaded);
	}
	//
	//  д���ݵ��ļ���
	//  buf:��������ַ
	//  count:��������С
	//  pWrite:ʵ����д�˶�������
	//  ���ش�����  �� FFL_OK��ʾ�ɹ�
	//
	status_t CSocket::write(void* buf, size_t count, size_t* pWrite) {
		return FFL_socketWrite(mFd, buf, count, pWrite);
	}
	//
	//  д���ݵ��ļ���
	//  bufVec:��������ַ,����
	//  count:�����С
	//  pWrite:ʵ����д�˶�������
	//  ���ش�����  �� FFL_OK��ʾ�ɹ�
	//
	status_t CSocket::writeVec(const BufferVec* bufVec, int count, size_t* pWrite) {
		status_t ret = FFL_OK;
		size_t nWriteAll = 0;
		size_t nWrite;
		for (int32_t i = 0; i < count; i++) {
			const BufferVec* pBuf = bufVec + i;

			if (FFL_OK !=
				 (ret=FFL_socketWrite(mFd, pBuf->data, count, &nWrite))
				){				
				break;
			}
			nWriteAll += nWrite;
		}
		
		if (pWrite) {
			*pWrite = nWriteAll;
		}
		return ret;
	}
}