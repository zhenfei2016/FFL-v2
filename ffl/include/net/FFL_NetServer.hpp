/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_NetServer.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/14 
*  https://github.com/zhenfei2016/FFL-v2.git
*  ��·����������
*
*/
#ifndef _FFL_NET_SERVER_HPP_
#define _FFL_NET_SERVER_HPP_

#include <FFL.h>
#include <ref/FFL_Ref.hpp>

namespace FFL {
	class NetConnect;
	class NetConnectManager;
	class NetServer {
	public:
		NetServer();
		virtual ~NetServer();
		//
		//  ���ӹ���
		//
		void setConnectManager(NetConnectManager* mgr);
		NetConnectManager* getConnectManager() const;
		//
		//  ����,ֹͣ����,��������ǰ��ҪsetConnectManager
		//		
		status_t start();
		void stop();
	protected:		
		//
		//  ����,ֹͣ����ʵ��
		//		
		virtual status_t onStart()=0;
		virtual void onStop()=0;
	protected:
	    //
		//  ������ɾ��һ����������
		//
		NetConnect* createConnect(NetFD fd);
		void destroyConnect(NetFD fd);
	private:
		NetConnectManager* mConnectMgr;	
	};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class TcpServer : public NetServer {
	public:
		TcpServer(const char* ip,int32_t port);		
		~TcpServer();
		//
		//  ����,ֹͣ����ʵ��
		//		
		virtual status_t onStart();
		virtual void onStop();
	protected:
		//
		//  ����ѭ���������Ƿ��������
		//
		bool listenLoop() ;
	private:	
		const char* mIp;
		int32_t mPort;
		int32_t mServerFd;

		friend class ListenThread;
		FFL::sp<ListenThread> mListenThread;

	};
}

#endif