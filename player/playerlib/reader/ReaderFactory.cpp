#include "ReaderFactory.hpp"
#include "ReaderFFmpeg.hpp"

FFL_IMPLEMENT_SINGLETON(reader::ReaderFactory);
namespace reader {
	ReaderFactory::ReaderFactory():mReader(NULL){
	}
	ReaderFactory::~ReaderFactory() {
	}
	//
	//  ����url����һ�����ʵ�ReaderInterface�������֧�ֻ᷵��NULL
	//
	FFL::sp<ReaderBase> ReaderFactory::createReader(const char* url, void* userdata){
		mReader = new FFMPegReader();
		return mReader;
	}
	//
	// �ͷ����reader
	//
	void ReaderFactory::destroyReader(FFL::sp<ReaderBase> reader) {

	}
}