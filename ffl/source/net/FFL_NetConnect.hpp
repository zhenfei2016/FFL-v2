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
	//  ������ֹͣconnect
	//
	status_t NetConnect::start() {
		return FFL_OK;
	}
	void NetConnect::stop() {

	}
	//
	//  ����fd�����ݵ�ѭ���������Ƿ��������
	//
	bool NetConnect::process() {
		return false;
	}
}