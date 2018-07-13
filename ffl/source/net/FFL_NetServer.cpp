#include "FFL_netServer.hpp"
#include <net/FFL_socket.h>
#include <thread/FFL_Thread.hpp>
namespace FFL {
	NetServer::NetServer(NetConnectManager* mgr) {

	}
	NetServer::~NetServer() {

	}

	status_t NetServer::startServer() {
		return FFL_OK;
	}
	//
	//  ֹͣtcp������
	//
	void NetServer::stopServer() {

	}

	class ListenThread : public Thread{
	public:
		ListenThread(TcpServer* server):mServer(server){
		}
		virtual bool threadLoop() {
			return mServer->listenLoop();
		}

		TcpServer* mServer;
	};
	TcpServer::TcpServer():mServerFd(-1){
		mListenThread = new ListenThread(this);
	}
	TcpServer::TcpServer(NetConnectManager* mgr) {

	}
	TcpServer::~TcpServer() {
		mListenThread = NULL;
	}

	//
	//  ����ָ���˿ڵ�tcp����
	//
	bool TcpServer::startLoopAddrServer(int32_t port) {
		return false;
	}
	bool TcpServer::startAnyAddrServer(int32_t port) {
		return false;
	}
	bool TcpServer::startServer(const char* ip, int32_t port) {
		if (mServerFd >= 0) {
			return false;
		}

		if (FFL_socketAnyAddrTcpServer(port, &mServerFd)!=FFL_SOCKET_OK) {
			FFL_LOG_WARNING("Failed to create tcp server. %s:%d",ip,port);
		}		
		mListenThread->run("tcpserver");
		return true;
	}
	//
	//  ֹͣtcp������
	//
	void TcpServer::stopServer() {
		if (mServerFd >= 0) {
			mListenThread->requestExit();
			FFL_socketClose(mServerFd);
			mServerFd = -1;
			mListenThread->requestExitAndWait();			
		}
	}

	//
	//  ����ѭ���������Ƿ��������
	//
	bool TcpServer::listenLoop() {
		if (mServerFd < 0) {
			return false;
		}
		
		int clientFd=0;
		if (FFL_socketAccept(mServerFd, &clientFd) != FFL_SOCKET_OK){
			return false;
		}

		return true;
	}
}