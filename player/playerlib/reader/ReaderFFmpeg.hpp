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
		// 获取播放时长us
		//
		virtual int64_t getDuration() { return 0; }
		//
		// 获取当前的播放位置 us
		//
		virtual int64_t getCurrentPosition() { return 0; }
	protected:
		//
		//  读取一帧数据
		//
		virtual void onReadFrame();
		//
		// open函数，具体实现
		//
		virtual void onOpen(const char* url);

		//
		// close函数，具体实现
		//
		virtual void onClose();

		//
		// pause函数，resume 具体实现
		//
		virtual void onPause();
		virtual void onResume();

		//
		// seek函数，具体实现
		//	
		virtual void onSeek(int64_t pos);	
	private:
		//
		//  打开这几个流
		//
		void openStream(AVStream** streams, uint32_t count);
		//
		//  文件结束
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
		// 每次seek后会更改序列号
		//
		int64_t mSerialNumber;
		//
		//  是否eof
		//
		uint32_t mEOFFlag;
		//
		//  解复用FFMpeg上下文
		//
		AVFormatContext* mAVFormatContext;
		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
	};
}
#endif
