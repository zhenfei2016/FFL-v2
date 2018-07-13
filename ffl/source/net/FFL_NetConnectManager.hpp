#ifndef _FFL_NETCONNECT_MANAGER_HPP_
#define _FFL_NETCONNECT_MANAGER_HPP_

#include <FFL.h>
#include <ref/FFL_Ref.hpp>

namespace FFL {
	class NetConnect;
	class NetServer;

	class NetConnectManager {
	public:
		NetConnectManager();
		~NetConnectManager();	
	public:
		virtual NetConnect* createConnect(NetFD fd, NetServer* srv);
		virtual void destroyConnect(NetFD fd);

	protected:
		List<NetConnect*> mConnects;
	};
}

#endif