/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_HttpConnect.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/15 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  һ��http��������
*/
#ifndef _FFL_NETCONNECT_HTTP_HPP_
#define _FFL_NETCONNECT_HTTP_HPP_

#include <FFL.h>
#include <net/http/FFL_HttpConnHandle.hpp>
#include <net/FFL_NetConnect.hpp>
#include <net/FFL_NetStream.hpp>
#include <net/FFL_NetSocket.hpp>

namespace FFL {
	//
	//  http����
	//
	class HttpConnect : public NetConnect {
	public:
		HttpConnect(NetFD fd, sp<HttpConnectHandler> handler);
		~HttpConnect();
	public:
		//
		//  ��ȡ��д�ӿ�
		//
		IOReader* getReader();
		IOWriter* getWriter();

		void close();
		bool isClosed();
	public:
		//
		//  ����fd�����ݵ�ѭ���������Ƿ��������
		//
		virtual bool process();
	private:
		NetStreamReader* mStreamReader;
		wp<HttpConnectHandler> mHandler;

		CSocket* mSocket;
		bool mClosed;
	};
}

#endif