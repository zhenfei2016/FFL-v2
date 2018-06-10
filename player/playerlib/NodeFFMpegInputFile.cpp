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
#include "FFL_PlayerEvent.hpp"
#include "FFL_Player.hpp"
#include "FFMpegStream.hpp"
#include "NodeFFMpegVideoDecoder.hpp"

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
		mLoopPlay = 0;
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
			//avformat_find_stream_info(mAVFormatContext, NULL);
			//av_rescale(mAVFormatContext->duration, 1000, AV_TIME_BASE);
			//int64_t duration1 = mAVFormatContext->streams[0]->duration;
			//int64_t startTime1 = mAVFormatContext->streams[0]->start_time;

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

	void NodeFFMpegInputFile::seek(int64_t pos) {

        FFL::CMutex::Autolock l(&mRequestMutex);
        if (mEventSeekPending) {
            return;
        }
        mEventSeekPending = true;
        mSeekPos=pos;
	}
    
    
	bool NodeFFMpegInputFile::onSeek() {
		//av_seek_frame(mAVFormatContext,)

        //int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);
        
        //int streamIndex=-1;
       
        
//        int64_t SEEK_JITTER=1000;
		AVRational tb;
		tb.den = mCurrentTb.mDen;
		tb.num = mCurrentTb.mNum;
		int64_t ts=FFMPegUsToTimestamp( mSeekPos, tb);
//        int64_t minTs=ts - SEEK_JITTER;
//        int64_t maxTs=ts +SEEK_JITTER ;
       // int err=avformat_seek_file(mAVFormatContext,streamIndex,minTs,ts,maxTs,0);
		//if (err < 0) {
		//	FFL_LOG_WARNING("Failed to seek %" lld64,ts);
		//}

		{
			int defaultStreamIndex = av_find_default_stream_index(mAVFormatContext);
			FFL::sp<FFMpegStream> stream = mStreamVector[defaultStreamIndex];
			if(!stream.isEmpty())
			{

				int64_t seekTime = mAVFormatContext->streams[defaultStreamIndex]->start_time +
					ts;
				int ret;
				if (seekTime > mCurrentPts)
				{
					ret = av_seek_frame(mAVFormatContext, defaultStreamIndex, seekTime, AVSEEK_FLAG_ANY);
				}
				else
				{
					ret = av_seek_frame(mAVFormatContext, defaultStreamIndex, seekTime, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD);
				}
			}
		}
        {
            FFL::CMutex::Autolock l(mRequestMutex);
            mEventSeekPending = false;
        }
        
        //
        //  节点丢弃当前没有处理的msg
        //
        FFL::sp<FFL::PipelineMessage> msgDiscardCache = new FFL::PipelineMessage(MSG_CONTROL_DISCARD_CACHE);
        for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
            if (mStreamVector[i].isEmpty()) {
                continue;
            }
            if (FFL_OK != postMessage(mStreamVector[i]->mDataSource.mId, msgDiscardCache)) {
            }
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

		AVRational tb;
		tb.den = mCurrentTb.mDen;
		tb.num = mCurrentTb.mNum;
		return FFMPegTimestampToUs(mCurrentPts,tb);
	}
	bool NodeFFMpegInputFile::onClose()
	{
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

	void NodeFFMpegInputFile::onReadFrame()
	{
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
		FFL::sp<FFMpegStream> stream = mStreamVector[streamIndex];				

		
		if (stream.isEmpty()) {
			msg->consume(this);
			return;
		} 
		

		//
		// 这个值是不准确的，先这样啊
		//
		int64_t pts = packet->mPacket->pts;
		if (pts == AV_NOPTS_VALUE) {
			pts = packet->mPacket->dts;
		}
		mCurrentPts = pts;
		stream->getTimebase(mCurrentTb);

		if (FFL_OK != postMessage(stream->mDataSource.mId, msg))
		{
			msg->consume(this);
        }else{
            
        }
	}

	void NodeFFMpegInputFile::handleEof() {
		//
		//  关闭当前的输入接口
		//		
		mEOFFlag = 1;


		FFL::sp<FFL::PipelineMessage> msgEof = new FFL::PipelineMessage(MSG_CONTROL_READER_EOF);
		for (int i = 0; i < SUPORT_STREAM_NUM; i++) {
			if (mStreamVector[i].isEmpty()) {
			    continue;
			}
			if (FFL_OK != postMessage(mStreamVector[i]->mDataSource.mId, msgEof)) {
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
#if 0	
	void NodeFFMpegInputFile::openStream(AVStream** streams, uint32_t count)
	{
		FFL::sp<FFL::Pipeline> pipeline = getPipeline();
		AVStream* stream = 0;
		int64_t startTime = -1;
		bool haveAudio = false;

		//av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
		//	st_index[AVMEDIA_TYPE_SUBTITLE],
		//	(st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
		//		st_index[AVMEDIA_TYPE_AUDIO] :
		//		st_index[AVMEDIA_TYPE_VIDEO]),
		//	NULL, 0);

		for (uint32_t index = 0; index < FFL_MIN(count,4); index++)
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

			else if (codec->codec_type == AVMEDIA_TYPE_AUDIO )
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

	AVCodecContext* NodeFFMpegInputFile::openCodec(AVStream* stream)
	{
		AVCodec* codec = NULL;
		AVCodecContext* codecCtx = NULL;
		//AVCodec* codec = avcodec_find_decoder(stream->codec->codec_id);
		codec = avcodec_find_decoder(stream->codecpar->codec_id);
		if (!codec)
		{
			FFL_LOG_WARNING("failed to find codec\n");
			return NULL;
		}

		codecCtx = avcodec_alloc_context3(codec);
		if (!codecCtx)
		{
			FFL_LOG_WARNING("Failed to allocate the codec context\n");
			goto fail;
		}

		if (avcodec_parameters_to_context(codecCtx, stream->codecpar) < 0) {
			FFL_LOG_WARNING("Failed to copy codec parameters to decoder context\n");
			goto fail;
		}

		if (avcodec_open2(codecCtx, codec, NULL) < 0)
		{
			FFL_LOG_WARNING("Failed to open codec\n");
			goto fail;
		}

		return codecCtx;
	fail:

		if (codecCtx)
			avcodec_free_context(&codecCtx);
		return  NULL;
	}

}

