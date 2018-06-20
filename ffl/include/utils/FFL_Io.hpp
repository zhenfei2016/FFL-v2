/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Io.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/20
*  https://github.com/zhenfei2016/FFL-v2.git
*  io��д�ӿ�
*
*/

#ifndef _FFL_IO_HPP_
#define _FFL_IO_HPP_

#include <FFL.h>
namespace FFL {
	//
	//  ������buffer
	//
	struct BufferVec {
		void  *data;
		size_t size;
	};

	class IOReader {
	public:
		//
		//  �����ݵ�������
		//  buf:��������ַ
		//  count:��Ҫ���Ĵ�С
		//  pReaded:ʵ���϶��˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t read(uint8_t* buf, size_t count, size_t* pReaded)=0;
	};

	class IOWriter {
	public:
		//
		//  д���ݵ��ļ���
		//  buf:��������ַ
		//  count:��������С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t write(void* buf, size_t count, size_t* pWrite)=0;
		//
		//  д���ݵ��ļ���
		//  bufVec:��������ַ,����
		//  count:�����С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t writeVec(const BufferVec* bufVec, int count, size_t* pWrite)=0;		
	};
}

#endif