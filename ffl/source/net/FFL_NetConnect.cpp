#ifndef _FFL_NETCONNECT_HPP_
#define _FFL_NETCONNECT_HPP_

#include <FFL.h>
#include <ref/FFL_Ref.hpp>

namespace FFL {
	class NetConnectManager;
	class NetConnect{
		friend class NetConnectManager;
	public:
		NetConnect(NetFD fd);
		virtual ~NetConnect();
	public:
		NetFD getFd() const {
			return mFd;
		}
	
		//
		//  启动，停止connect
		//
		status_t start();
		void stop();
	public:
		//
		//  处理fd上数据的循环，返回是否继续处理
		//
		virtual bool process();

	protected:
		NetFD mFd;
		NetConnectManager* mConnMgr;
	};
}

#endif