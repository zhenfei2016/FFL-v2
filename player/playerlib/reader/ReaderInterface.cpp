#include "ReaderInterface.hpp"
#include "StreamManagerInterface.hpp"
namespace reader {



	ReaderInterface::ReaderInterface():mStreamMgr(&gDefaultStreamMgr){
	}
	ReaderInterface::~ReaderInterface(){
	}
	//
	//  ������������,��open�ɹ��󣬶�ȡ��������������Ϣ��Ȼ���֪ͨIStreamManager
	//
	void ReaderInterface::setStreamManager(StreamMgrInterface* streamMgr) {
		if (streamMgr) {
			mStreamMgr = streamMgr;
		}
	}
	//
	//  ��ȡ������
	//
	StreamMgrInterface* ReaderInterface::getStreamManager() {
		return mStreamMgr;
	}
}