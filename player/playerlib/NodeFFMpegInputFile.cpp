/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegInputFile.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  文件读取，提取流
*
*/

#include "NodeFFMpegInputFile.hpp"
#include <pipeline/FFL_PipelineSourceConnector.hpp>
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>
#include <pipeline/FFL_PipelineMessageCache.hpp>

#include "NodeBase.hpp"
#include "MessageFFMpegPacket.hpp"
#include "PlayerEvent.hpp"
#include "PlayerCore.hpp"
#include "PlayerConstant.hpp"
#include "FFMpegStream.hpp"
#include "NodeFFMpegVideoDecoder.hpp"
#include "TimestampUtils.hpp"

namespace player {
	NodeFFMpegInputFile::NodeFFMpegInputFile(IStreamManager* streamMgr) :		
		mEventOpenPending(false),
		mEventClosePending(false),
		mEventPausePending(false),
		mEventSeekPending(false),
		mAVFormatContext(NULL)
	{
		setName("ffmpeg-input");
		mStreamManager = streamMgr;
		mSeekUs = 0; mSerialNumber = 1;
		mLoopPlay = 0;
		mIsBuffering = false;
		mMessageCache = new FFL::PipelineMessageCache(MSG_FFMPEG_AVPACKET);		
	}
	NodeFFMpegInputFile::~NodeFFMpegInputFile()
	{
        mMessageCache.clear();
	}


	void NodeFFMpegInputFile::onCreate() {
		init();
	}

	void NodeFFMpegInputFile::onDestroy() {

	}

	bool NodeFFMpegInputFile::init()
	{
		FFL::sp<FFL::PipelineInputHandler> callback =
			new FFL::ClassMethodVoidInputHandler<NodeFFMpegInputFile>(this, &NodeFFMpegInputFile::readFrame);
		InputInterface input = createInputInterface(callback, "reader-stream");
		if (!input.isValid()) {
			FFL_LOG_ERROR("invalid input id");
			return false;
		}

	
        mInput=input;
		FFL::sp<FFL::PipelineSourceConnector> conn = 0;
		if (1) {
			conn = new FFL::PipelineIdleSourceConnector();
		}
		else {
			conn = new FFL::PipelineTimerSourceConnector(3000);
		}
		getOwner()->getPipeline()->connectSource(input.mNodeId, input.mId, conn);
		return true;
	}
	
	void NodeFFMpegInputFile::readFrame()
	{
		if (mEventPausePending) {
			FFL_sleep(100);
			return;
		}

		if (mEventOpenPending ){
			if (onOpen()) {

			}else {
				FFL_LOG_WARNING("NodeFFMpegInputFile open (%s) fail. ", mUrl.c_str());
				event::postPlayerEvent(this, event::EVENT_ERROR);
				return;
			}
			
		}

		if (mAVFormatContext == NULL)
		{
			FFL_LOG_WARNING("NodeFFMpegInputFile readFrame ctx is null");
			return;
		}

		if (mEventClosePending) {
			onClose();
			return;
		}
        
        if(mEventSeekPending){
            onSeek();
            return ;
        }

		onReadFrame();
	}
	
	void NodeFFMpegInputFile::play(const char* url)
	{
		FFL::CMutex::Autolock l(&mRequestMutex);
		if (mEventOpenPending) {
			return;
		}

		mUrl = url;
		mEventOpenPending = true;
	}

	bool NodeFFMpegInputFile::onOpen()
	{
		bool ret = false;
		if (mAVFormatContext) {
			avformat_close_input(&mAVFormatContext);
			mAVFormatContext = NULL;
		}
		//AVERROR
		
		int err = avformat_open_input(&mAVFormatContext, mUrl.c_str(), 0, 0);
		if (0 == err) {
			ret = true;
		}

		{
			FFL::CMutex::Autolock l(mRequestMutex);
			mEventOpenPending = false;
			mEOFFlag = 0;
		}

		if (ret)
		{			
			//int64_t duration=mAVFormatContext->duration;
			//int64_t startTime=mAVFormatContext->start_time;			
			//av_rescale(mAVFormatContext->duration, 1000, AV_TIME_BASE);
			//int64_t duration1 = mAVFormatContext->streams[0]->duration;
			//int64_t startTime1 = mAVFormatContext->streams[0]->start_time;

			avformat_find_stream_info(mAVFormatContext, NULL);
			openStream(mAVFormatContext->streams, mAVFormatContext->nb_streams);
		}
		return ret;
	}

	void NodeFFMpegInputFile::pause() {
		FFL::CMutex::Autolock l(&mRequestMutex);
		if (mEventPausePending) {
			return;
		}
		mEventPausePending = true;
	}

	void NodeFFMpegInputFile::resume() {
		FFL::CMutex::Autolock l(&mRequestMutex);
		if (mEventPausePending) {
			return;
		}
		mEventPausePending = true;
	}

	void NodeFFMpegInputFile::seek(int64_t us) {

        FFL::CMutex::Autolock l(&mRequestMutex);
        if (mEventSeekPending) {
            return;
        }
        mEventSeekPending = true;
		mSeekUs =us;
	}
    
    
	bool NodeFFMpegInputFile::onSeek() {
		int64_t ts= FFMPegUsToSeekfileTimestamp(mSeekUs);
        int err=avformat_seek_file(mAVFormatContext,-1, INT64_MIN,ts, INT64_MAX,0);
		if (err < 0) {
			FFL_LOG_WARNING("Failed to seek %" lld64,ts);
		}
		mSerialNumber++;

        //
        //  节点丢弃当前没有处理的msg
        //
        FFL::sp<FFL::PipelineMessage> msgDiscardCache = new FFL::PipelineMessage(MSG_CONTROL_DISCARD_MSG);		
		msgDiscardCache->setParams(mSerialNumber, 0);

        for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
            if (!mStreamVector[i].isValid()) {
                continue;
            }

			FFL::sp<FFL::PipelineOutput > output = getOutput(mStreamVector[i].mOutputInterface.mId);
			if (!output.isEmpty()) {
				output->clearMessage();
			}

            if (FFL_OK != postMessageDelay(mStreamVector[i].mOutputInterface.mId, msgDiscardCache,-1)) {
            }			
        }
        
		{
			FFL::CMutex::Autolock l(mRequestMutex);
			mEventSeekPending = false;
			mIsBuffering = true;
			
		}

        event::postPlayerEvent(this,event::EVENT_SEEK_END);
        event::postPlayerEvent(this,event::EVENT_BUFFERING_START);
		return true;
	}

	
	void NodeFFMpegInputFile::close()
	{
		FFL::CMutex::Autolock l(&mRequestMutex);
		if (mEventClosePending) {
			return;
		}
		mEventClosePending = true;
	}
	//
	//  获取视频的播放时长us
	//
	int64_t NodeFFMpegInputFile::getDurationUs() {
		if (!mAVFormatContext) {
			return -1;
		}

		//
		//  us
		//
		int64_t durationUs=av_rescale((mAVFormatContext->duration), 1000000, AV_TIME_BASE);		
		return durationUs;
	}
	//
	//  获取当前的位置
	//
	int64_t NodeFFMpegInputFile::getCurrentUs() {
		if (!mAVFormatContext) {
			return -1;
		}
		return timestampToUs(mCurrentPts, mCurrentTb);
	}
	bool NodeFFMpegInputFile::onClose(){
		bool ret = false;
		if (mAVFormatContext) {
			avformat_close_input(&mAVFormatContext);
			mAVFormatContext = NULL;
		}


		{
			FFL::CMutex::Autolock l(mRequestMutex);
			mEventClosePending = false;
		}
		return ret;
	}

	void NodeFFMpegInputFile::onReadFrame(){
		if (mEOFFlag) {
			FFL_sleep(10);
			FFL_LOG_WARNING(" NodeFFMpegInputFile::onReadFrame eof");
			return;
		}

		message::FFMpegPacket* packet = NULL;
		FFL::sp<FFL::PipelineMessage> msg = message::createMessageFromCache(mMessageCache, &packet, MSG_FFMPEG_AVPACKET);
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
			} else {
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
	
		if (mIsBuffering) {
			//packet->mPacket->flags
		}

		//
		// 这个值是不准确的，先这样啊
		//
		int64_t pts = packet->mPacket->pts;
		if (pts == AV_NOPTS_VALUE) {
			pts = packet->mPacket->dts;
		}
		mCurrentPts = pts;
		stream.mStream->getTimebase(mCurrentTb);
		if (FFL_OK != postMessage(stream.mOutputInterface.mId, msg))
		{
			msg->consume(this);
        }else{
			FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "read %s pts=%" lld64 " streamIndex=%d timebase=%d:%d",
				(stream.mStream->getStreamType() == STREAM_TYPE_VIDEO ? "video " : "audio"),
				pts,
				stream.mStream->getIndex(),
				mCurrentTb.mNum, mCurrentTb.mDen);
        }
	}

	void NodeFFMpegInputFile::handleEof() {
		//
		//  关闭当前的输入接口
		//		
		mEOFFlag = 1;
		FFL::sp<FFL::PipelineMessage> msgEof = new FFL::PipelineMessage(MSG_CONTROL_READER_EOF);
		for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
			if (!mStreamVector[i].isValid()) {
			    continue;
			}
			if (FFL_OK != postMessage(mStreamVector[i].mOutputInterface.mId, msgEof)) {
			}
		}	

		//
		//  关闭当前的输入
		//
        FFL::sp<FFL::PipelineInput> input=getInput(mInput.mId);
        if(!input.isEmpty()){
            input->requestShutdown();
        }        
	}
#if 1	
	void NodeFFMpegInputFile::openStream(AVStream** streams, uint32_t count){
		FFL::sp<FFL::Pipeline> pipeline = getPipeline();
		AVStream* stream = 0;

		int streamIndexVec[3] = {-1};
		streamIndexVec[0] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
		streamIndexVec[1] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
		streamIndexVec[2] = -1;
		//streamIndexVec[2] = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_SUBTITLE,-1, -1, NULL, 0);
				
		for ( int i=0;i< FFL_ARRAY_ELEMS(streamIndexVec); i++){
			uint32_t index = streamIndexVec[i];
			if(index==-1) continue;
			stream = streams[index];
			
			FFL::sp<Stream> streamInfo ;
			switch (avcodec_get_type(stream->codecpar->codec_id)) {
			case AVMEDIA_TYPE_VIDEO:
				{
					streamInfo = new FFMpegVideoStream(stream);				
					FFL::sp<event::PlayerEvent> event = new event::PlayerEvent(event::EVENT_VIDEO_SIZE_CAHNGED);
					event->mInt32Parma1 = stream->codecpar->width;
					event->mInt32Parma2 = stream->codecpar->height;
					event->mInt64Param1 = stream->codecpar->sample_aspect_ratio.num;
					event->mInt64Param2 = stream->codecpar->sample_aspect_ratio.den;
					event::postPlayerEvent(this, event);
				}
				break;
			case AVMEDIA_TYPE_AUDIO:
				{
					streamInfo = new FFMpegAudioStream(stream);
				}
				break;
			default:
				break;
			}				
			if (streamInfo.isEmpty()) {
				FFL_LOG_WARNING("stream %d not support ", stream->index);
				continue;
			}

			FFL::sp<Decoder> decoder = streamInfo->createDecoder();
			if (decoder.isEmpty()) {
				mStreamVector[index].mStream = NULL;
				FFL_LOG_WARNING("Failed to createDecoder (index=%d). ", stream->index);
				continue;
			}
			
			StreamEntry& entry = mStreamVector[index];
			entry.mStream = streamInfo;
			entry.mOutputInterface= createOutputInterface();			
			entry.mStream->mSource = entry.mOutputInterface;
			FFL::formatString(entry.mStream->mSource.mName,
				"decoder-%d-%d", 
				streamInfo->getIndex(),
				streamInfo->getStreamType());
			
			//
			// 启动这个流的处理
			//
			InputInterface input;			
			decoder->create(getOwner());
			decoder->connectSource(entry.mOutputInterface, entry.mStream->mSource.mName.c_str(), input, 0);
			mStreamManager->addStream(streamInfo);			
		}
	}
#else
	void NodeFFMpegInputFile::openStream(AVStream** streams, uint32_t count)
	{
		FFL::sp<FFL::Pipeline> pipeline = getPipeline();
		AVStream* stream = 0;
		int64_t startTime = -1;
		//bool haveAudio = false;

		//AVCodecContext* codec = NULL;
		//av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_AUDIO, 1, 1, &codec, 0);
		//	st_index[AVMEDIA_TYPE_SUBTITLE],
		//	(st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
		//		st_index[AVMEDIA_TYPE_AUDIO] :
		//		st_index[AVMEDIA_TYPE_VIDEO]),
		//	NULL, 0);

		for (uint32_t index = 0; index < FFL_MIN(count, 4); index++)
		{
			stream = streams[index];

			//
			//  获取第一帧的时间
			//
			if (stream->start_time != AV_NOPTS_VALUE) {
				startTime = FFL_MIN(startTime, stream->start_time);
			}

			AVCodecContext* codec = openCodec(stream);
			if (!codec) {
				continue;
			}

			FFL::sp<FFMpegStream> streamInfo = new FFMpegStream();
			if (streamInfo.isEmpty()) {
				FFL_LOG_WARNING("stream %d not support ", stream->index);
				continue;
			}

			streamInfo->setStreamIndex(stream->index);
			streamInfo->mFFMpegStream = stream;
			streamInfo->mFFMpegCodecCtx = codec;
			if (codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				streamInfo->setStreamType(STREAM_TYPE_VIDEO);

				FFL::sp<event::PlayerEvent> event = new event::PlayerEvent(event::EVENT_VIDEO_SIZE_CAHNGED);
				event->mInt32Parma1 = stream->codecpar->width;
				event->mInt32Parma2 = stream->codecpar->height;
				event->mInt64Param1 = stream->codecpar->sample_aspect_ratio.num;
				event->mInt64Param2 = stream->codecpar->sample_aspect_ratio.den;
				event::postPlayerEvent(this, event);
			}

			else if (codec->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				//if (!haveAudio) {
				//	mStreamVector[index] = NULL;
				//	avcodec_free_context(&codec);
				//	haveAudio = true;
				//	continue;
				//}
				//else {					
				streamInfo->setStreamType(STREAM_TYPE_AUDIO);
				//}
			}
			//
			//  
			//
			OutputInterface output = createOutputInterface();
			streamInfo->mDataSource = output;
			FFL::sp<Decoder> decoder = streamInfo->createDecoder();
			if (!decoder.isEmpty()) {
				mStreamVector[index] = streamInfo;
				mStreamManager->addStream(streamInfo);
				decoder->create(getOwner());
				streamInfo->build();
			}
			else {
				mStreamVector[index] = NULL;
			}
		}
	}
#endif
	
}

