#ifndef _STREAM_MANAGER_INTERFACE_HPP_
#define _STREAM_MANAGER_INTERFACE_HPP_
#include <ref/FFL_Ref.hpp>
#include "Stream.hpp"

namespace reader{
	class ReaderBase;
	class ReaderStreamManager {
	public:
		//
		//  ���reader�µ�һ��stream�������У����ص�ǰ���stream��id
		//
		virtual int32_t  addStream(ReaderBase* reader, StreamPtr stream) = 0;
		//
		//  ���reader�µ�����stream�������У����سɹ�����˼�����
		//  
		//
		virtual uint32_t addStreamVec(ReaderBase* reader, FFL::Vector < StreamPtr > streamVec) = 0;
		//
		//  ������id��ȡһ����ʵ��
		//
		virtual StreamPtr getStream(int32_t id) = 0;
		//
		//  ��ȡ���reader�µ�������
		//
		virtual void getStreamVec(ReaderBase* reader, FFL::Vector < StreamPtr >& streamVec) = 0;
	};
}

#endif