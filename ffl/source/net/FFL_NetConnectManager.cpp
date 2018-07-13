#include "FFL_netConnectManager.hpp"

namespace FFL {
	NetConnectManager::NetConnectManager(){
	}
	NetConnectManager::~NetConnectManager(){
	}
	NetConnect* NetConnectManager::createConnect(NetFD fd, NetServer* srv) {
		return NULL;
	}
	void NetConnectManager::destroyConnect(NetFD fd) {

	}

}