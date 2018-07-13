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
		//  启动服务
		//
		virtual status_t startServer();
		//
		//  停止服务器
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
		//  启动指定端口的tcp服务
		//
		bool startLoopAddrServer(int32_t port);
		bool startAnyAddrServer(int32_t port);
		bool startServer(const char* ip, int32_t port);
		//
		//  停止tcp服务器
		//
		void stopServer();
	protected:
		//
		//  监听循环，返回是否继续监听
		//
		bool listenLoop();
	private:
		friend class ListenThread;
		FFL::sp<ListenThread> mListenThread;

		int32_t mServerFd;
	};
}

#endif