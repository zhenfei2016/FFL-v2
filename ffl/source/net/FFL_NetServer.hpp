#ifndef _FFL_NET_SERVER_HPP_
#define _FFL_NET_SERVER_HPP_


#include <FFL.h>
#include <ref/FFL_Ref.hpp>

namespace FFL {
	class NetConnectManager;
	class NetServer {
	public:
		NetServer(NetConnectManager* mgr);
		virtual ~NetServer();
		//
		//  ��������
		//
		virtual status_t startServer();
		//
		//  ֹͣ������
		//
		virtual void stopServer();
	protected:
		NetConnectManager* getConnectManager() const {
			return mConnectMgr;
		}
	private:
		NetConnectManager* mConnectMgr;
	};


	class TcpServer {
	public:
		TcpServer();
		TcpServer(NetConnectManager* mgr);
		~TcpServer();

		//
		//  ����ָ���˿ڵ�tcp����
		//
		bool startLoopAddrServer(int32_t port);
		bool startAnyAddrServer(int32_t port);
		bool startServer(const char* ip, int32_t port);
		//
		//  ֹͣtcp������
		//
		void stopServer();
	protected:
		//
		//  ����ѭ���������Ƿ��������
		//
		bool listenLoop();
	private:
		friend class ListenThread;
		FFL::sp<ListenThread> mListenThread;

		int32_t mServerFd;
	};
}

#endif