#include "FFL_netConnect.hpp"
#include "FFL_netConnectManager.hpp"

namespace FFL {
	NetConnect::NetConnect(NetFD fd):mFd(fd){
		mConnMgr = NULL;
	}
	NetConnect::~NetConnect() {
		mConnMgr->destroyConnect(mFd);
		if (mFd>=0) {
			FFL_socketClose(mFd);
			mFd = -1;
		}
	}	
	//
	//  启动，停止connect
	//
	status_t NetConnect::start() {
		return FFL_OK;
	}
	void NetConnect::stop() {

	}
	//
	//  处理fd上数据的循环，返回是否继续处理
	//
	bool NetConnect::process() {
		return false;
	}
}