/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_File.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/20 
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  �ļ�������
 *
*/
#ifndef _FFL_FILE_HPP_
#define _FFL_FILE_HPP_

#include <utils/FFL_String.hpp>
#include <utils/FFL_Io.hpp>

namespace FFL {

	class File : public IOReader ,public IOWriter {
	public:
		File();
		virtual ~File();
	public:
		//
		//  ���ļ���FFL_OK�ɹ�
		//  path:�ļ�����·��
		//
		status_t open(const String& path);
		//
		//  ׷��ģʽ���ļ���FFL_OK�ɹ�
		//  path:�ļ�����·��
		//
		status_t openAppend(const String& path);
		//
		// �����ļ�,�ļ��Ѿ����ڵ�����¸���ԭ�ļ�
		//
		status_t create(const String& path);
		//
		// �ر��ļ�
		//
		void close();	
		bool isOpened() const;		
	private:
		//
		//  ���ļ���FFL_OK�ɹ�
		//  path:�ļ�����·��
		//
		status_t open(const char* path,int mode);
	public:
		//
		//  д���ݵ��ļ���
		//  buf:��������ַ
		//  count:��������С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t write(void* buf, size_t count, size_t* pWrite);
		//
		//  д���ݵ��ļ���
		//  bufVec:��������ַ,����
		//  count:�����С
		//  pWrite:ʵ����д�˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t writeVec(const BufferVec* bufVec, int count, size_t* pWrite);
		//
		//  �����ݵ�������
		//  buf:��������ַ
		//  count:��Ҫ���Ĵ�С
		//  pReaded:ʵ���϶��˶�������
		//  ���ش�����  �� FFL_OK��ʾ�ɹ�
		//
		virtual status_t read(uint8_t* buf, size_t count, size_t* pReaded);
	private:
		//
		//  �ļ�·�����ļ����
		//
		String mPath;	
		void* mFd;
	};

	//
	//  �ļ��Ƿ񴴽���
	//
	bool fileIsExist(const char* path);
}

#endif