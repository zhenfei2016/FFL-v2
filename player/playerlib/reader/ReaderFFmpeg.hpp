#ifndef _READER_FFMPEG_HPP_
#define _READER_FFMPEG_HPP_
#include "ReaderBase.hpp"
#include "Stream.hpp"
#include "../FFMpeg.hpp"
#include <pipeline/FFL_PipelineMessageCache.hpp>
namespace reader {
	class FFMPegReader : public ReaderBase {
	public:
		FFMPegReader();
		~FFMPegReader();
	public:
		//
		// ��ȡ����ʱ��us
		//
		virtual int64_t getDuration() { return 0; }
		//
		// ��ȡ��ǰ�Ĳ���λ�� us
		//
		virtual int64_t getCurrentPosition() { return 0; }
	protected:
		//
		//  ��ȡһ֡����
		//
		virtual void onReadFrame();
		//
		// open����������ʵ��
		//
		virtual void onOpen(const char* url);

		//
		// close����������ʵ��
		//
		virtual void onClose();

		//
		// pause������resume ����ʵ��
		//
		virtual void onPause();
		virtual void onResume();

		//
		// seek����������ʵ��
		//	
		virtual void onSeek(int64_t pos);	
	private:
		//
		//  ���⼸����
		//
		void openStream(AVStream** streams, uint32_t count);
		//
		//  �ļ�����
		//
		void handleEof();
	private:
		//InputInterface mInput;
		enum {
			SUPORT_STREAM_NUM = 4,
		};
		struct StreamEntry {
			StreamPtr mStream;
			//OutputInterface mOutputInterface;

			bool isValid() {
				return !mStream.isEmpty();
			}

			FFL::PipelineOutputId getOutputId() {
				return mStream->getSource().mId;
			}
		};
		StreamEntry mStreamVector[SUPORT_STREAM_NUM];
		//
		// ÿ��seek���������к�
		//
		int64_t mSerialNumber;
		//
		//  �Ƿ�eof
		//
		uint32_t mEOFFlag;
		//
		//  �⸴��FFMpeg������
		//
		AVFormatContext* mAVFormatContext;
		//
		// ��������ݰ�
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
	};
}
#endif
