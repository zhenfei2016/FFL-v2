#include "ReaderFFmpeg.hpp"
#include "Stream.hpp"
#include "ReaderStreamManager.hpp"
#include "../MessageFFMpegPacket.hpp"
#include "../PlayerEvent.hpp"
#include "../FFMpegStream.hpp"

namespace reader {
	FFMPegReader::FFMPegReader() :mAVFormatContext(NULL){
		setName("reader");
		mSerialNumber = 1;
		mEOFFlag = 0;
		mMessageCache = new FFL::PipelineMessageCache(MSG_FFMPEG_AVPACKET);
	}
	FFMPegReader::~FFMPegReader() {
		mMessageCache->clear();
	}
	//
	// ��ȡ����ʱ��us
	//
	int64_t FFMPegReader::getDuration() {
		return mAVFormatContext->duration;
	}
	//
	// ��ȡ��ǰ�Ĳ���λ�� us
	//
	int64_t FFMPegReader::getCurrentPosition() {
		return 0;
	}

	//
	//  ��ȡһ֡����
	//
	void FFMPegReader::onReadFrame() {
		if (mAVFormatContext == NULL) {			
			FFL_LOG_WARNING(" FFMPegReader::onReadFrame context is null.");
			FFL_sleep(10);
			return;
		}
		if (mEOFFlag) {
			FFL_sleep(10);
			FFL_LOG_WARNING(" FFMPegReader::onReadFrame eof");	
			return;
		}

		message::FFMpegPacket* packet = NULL;
		FFL::sp<FFL::PipelineMessage> msg = message::createMessageFromCache(
			mMessageCache, &packet, MSG_FFMPEG_AVPACKET);
		FFL_ASSERT_LOG(packet, "FFMpegPacket is NULL.\n")

		int err = av_read_frame(mAVFormatContext, packet->mPacket);
		if (0 != err) {
			if (err == AVERROR_EOF) {
				msg->consume(this);
				handleEof();
			}
			FFL_LOG_WARNING("Failed to read frame.");
		}
		else {

		}

		int streamIndex = packet->mPacket->stream_index;
		StreamEntry& stream = mStreamVector[streamIndex];
		if (!stream.isValid()) {
			msg->consume(this);
			return;
		}

		packet->mSerialNumber = mSerialNumber;
		packet->mPacketType = stream.mStream->getStreamType();
		switch (packet->mPacketType)
		{
		case STREAM_TYPE_VIDEO:
			if (packet->mPacket->flags &AV_PKT_FLAG_KEY) {
				packet->mIFrame = 1;
			}
			else {
				packet->mIFrame = 0;
			}
			break;
		case STREAM_TYPE_AUDIO:
			packet->mIFrame = 0;
			break;
		default:
			packet->mIFrame = 0;
			break;
		}
	

		if (FFL_OK != postMessage(stream.mStream->getSource().mId, msg))
		{
			msg->consume(this);
		}
		else {
			//FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "read %s pts=%" lld64 " streamIndex=%d timebase=%d:%d",
			//	(stream.mStream->getStreamType() == STREAM_TYPE_VIDEO ? "video " : "audio"),
			//	pts,
			//	stream.mStream->getIndex(),
			//	mCurrentTb.mNum, mCurrentTb.mDen);
		}
	}
	//
	// open����������ʵ��
	//
	void FFMPegReader::onOpen(const char* url) {		
		if (mAVFormatContext) {
			avformat_close_input(&mAVFormatContext);
			mAVFormatContext = NULL;
		}

		//AVERROR
		int err = avformat_open_input(&mAVFormatContext, url, 0, 0);
		if (0 == err) {			
			avformat_find_stream_info(mAVFormatContext, NULL);
			openStream(mAVFormatContext->streams, mAVFormatContext->nb_streams);
		}		
	}

	//
	// close����������ʵ��
	//
	void FFMPegReader::onClose() {
		if (mAVFormatContext) {
			avformat_close_input(&mAVFormatContext);
			mAVFormatContext = NULL;
		}
	}

	//
	// pause������resume ����ʵ��
	//
	void FFMPegReader::onPause() {

	}
	void FFMPegReader::onResume() {

	}

	//
	// seek����������ʵ��
	//	
	void FFMPegReader::onSeek(int64_t pos) {
		int64_t ts = FFMPegUsToSeekfileTimestamp(pos);
		int err = avformat_seek_file(mAVFormatContext, -1, INT64_MIN, ts, INT64_MAX, 0);
		if (err < 0) {
			FFL_LOG_WARNING("Failed to seek %" lld64, ts);
		}
		mSerialNumber++;

		//
		//  �ڵ㶪����ǰû�д����msg
		//
		FFL::sp<FFL::PipelineMessage> msgDiscardCache = new FFL::PipelineMessage(MSG_CONTROL_DISCARD_MSG);
		msgDiscardCache->setParams(mSerialNumber, 0);

		for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
			StreamEntry& entry = mStreamVector[i];
			if (!entry.isValid()) {
				continue;
			}

			FFL::sp<FFL::PipelineOutput > output = getOutput(entry.getOutputId());
			if (!output.isEmpty()) {
				output->clearMessage();
			}

			if (FFL_OK != postMessageDelay(entry.getOutputId(), msgDiscardCache, -1)) {
			}
		}	

		event::postPlayerEvent(this, event::EVENT_SEEK_END);
		event::postPlayerEvent(this, event::EVENT_BUFFERING_START);		
	}

	void FFMPegReader::openStream(AVStream** streams, uint32_t count) {
		FFL::sp<FFL::Pipeline> pipeline = getPipeline();
		AVStream* stream = 0;

		int streamIndexVec[3] = { -1 };
		streamIndexVec[0] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
		streamIndexVec[1] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);		
		streamIndexVec[2] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_SUBTITLE,-1, -1, NULL, 0);
		streamIndexVec[2] = -1;

		for (int i = 0; i < FFL_ARRAY_ELEMS(streamIndexVec); i++) {
			uint32_t index = streamIndexVec[i];
			if (index == -1) continue;
			stream = streams[index];

			FFL::String name;
			StreamPtr streamInfo;
			switch (avcodec_get_type(stream->codecpar->codec_id)) {
			case AVMEDIA_TYPE_VIDEO:
			{
				name = "video";
				streamInfo = new player::FFMpegVideoStream(stream);				
				fillMetaData(streamInfo, stream);
			}
			break;
			case AVMEDIA_TYPE_AUDIO:
			{
				name = "audio";
				streamInfo = new player::FFMpegAudioStream(stream);
			}
			break;
			default:
				break;
			}
			if (streamInfo.isEmpty()) {
				FFL_LOG_WARNING("stream %d not support ", stream->index);
				continue;
			}

			player::OutputInterface output = createOutputInterface();			
			FFL::formatString(output.mName, "%s-decoder-%s-%d", getName(),name.c_str(), index);
			streamInfo->setSource(output);	

			StreamEntry& entry = mStreamVector[index];
			entry.mStream = streamInfo;			
			getStreamManager()->addStream(this,streamInfo);
		}

		getStreamManager()->addStream(this, NULL);
	}
	void FFMPegReader::fillMetaData(StreamPtr stream, AVStream* avstream) {
		FFL::Dictionary* dic = stream->getDictionary();
	}
	void FFMPegReader::handleEof() {
		//
		//  �رյ�ǰ������ӿ�
		//		
		mEOFFlag = 1;
		FFL::sp<FFL::PipelineMessage> msgEof = new FFL::PipelineMessage(MSG_CONTROL_READER_EOF);
		for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
			StreamEntry& entry = mStreamVector[i];
			if (!entry.isValid()) {
				continue;
			}
			if (FFL_OK != postMessage(entry.getOutputId(), msgEof)) {
			}
		}

		//
		//  �رյ�ǰ������
		//
		pauseLooper();
	}
}