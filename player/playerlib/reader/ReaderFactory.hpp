#include "ReaderBase.hpp"
#include <utils/FFL_Singleton.hpp>
namespace reader {
	class ReaderFactory : public FFL::Singleton<ReaderFactory> {
		friend class FFL::Singleton<ReaderFactory>;
	private:
		ReaderFactory();
		~ReaderFactory();

	public:
		//
		//  ����url����һ�����ʵ�ReaderInterface�������֧�ֻ᷵��NULL
		//
		FFL::sp<ReaderBase> createReader(const char* url,void* userdata);
		//
		// �ͷ����reader
		//
		void destroyReader(FFL::sp<ReaderBase> reader);
	private:
		FFL::sp<ReaderBase> mReader;
	private:
		DISABLE_COPY_CONSTRUCTORS(ReaderFactory);
	};
}