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
		//  ������ֹͣconnect
		//
		status_t start();
		void stop();
	public:
		//
		//  ����fd�����ݵ�ѭ���������Ƿ��������
		//
		virtual bool process();

	protected:
		NetFD mFd;
		NetConnectManager* mConnMgr;
	};
}

#endif