/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  MessageFFMpegFrame.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/11
*
*  视频，音频解码后的结构包装
*
*/

#include "MessageFFMpegFrame.hpp"

namespace message {
	FFMpegVideoFrame::FFMpegVideoFrame():mFrame(NULL)
	{
        setType(MSG_FFMPEG_VIDEO_FRAME);
		
	}
	FFMpegVideoFrame::~FFMpegVideoFrame()
	{
		if (mFrame) {
			FFL_LOG_WARNING("FFMpegVideoFrame av_frame_unref");
			av_frame_unref(mFrame);
			av_frame_free(&mFrame);
		}
	}

 
	//
	//  已经处理完成了，可以回收了		
	//
	void FFMpegVideoFrame::consume()
	{
		av_frame_unref(mFrame);
		av_frame_free(&mFrame);
	}

	//
	//  填充数据
	//
	void FFMpegVideoFrame::fillAvframe(AVFrame* frame)
	{
		if (mFrame) {
			FFL_LOG_WARNING("FFMpegVideoFrame av_frame_unref");
		}
		mFrame = frame;

		mTexture.mWidth = frame->width;
		mTexture.mHeight = frame->height;
		

		mTexture.mDuration = frame->pkt_duration;
		mTexture.mPts = frame->pts;
		
		switch (frame->format)
		{
		case AV_PIX_FMT_YUV420P:
			mTexture.mPlanesCount = 3;
			break;

		default:
			mTexture.mPlanesCount = 1;
			break;
		}
		mTexture.mPixFormat = frame->format;
		
		for (int i = 0; i < mTexture.mPlanesCount; i++) {
			mTexture.mPitches[i] = frame->linesize[i];
			mTexture.mPixels[i] = frame->data[i];
		}

		//av_guess_sample_aspect_ratio()
		//mTexture.mSampleAspectRatioDen

	}


	FFMpegAudioFrame::FFMpegAudioFrame() :mFrame(NULL)
	{

        setType(MSG_FFMPEG_AUDIO_FRAME);
	}
	FFMpegAudioFrame::~FFMpegAudioFrame()
	{
		if (mFrame) {
			FFL_LOG_WARNING("FFMpegAudioFrame av_frame_unref");
			av_frame_unref(mFrame);
			av_frame_free(&mFrame);
		}
	}


	//
	//  已经处理完成了，可以回收了		
	//
	void FFMpegAudioFrame::consume()
	{
		av_frame_unref(mFrame);
		av_frame_free(&mFrame);
	}

	//
	//  填充数据
	//
	void FFMpegAudioFrame::fillAvframe(AVFrame* frame)
	{
		if (mFrame) {
			FFL_LOG_WARNING("FFMpegVideoFrame av_frame_unref");
		}
		mFrame = frame;

		mSamples.mFreq = 0;
		mSamples.mSampleNum= frame->nb_samples;
		mSamples.mChannel = frame->channels;
		mSamples.mFormat = frame->format;
		mSamples.mChannelLayout=(uint32_t)frame->channel_layout;


		mSamples.mDuration = frame->pkt_duration;
		mSamples.mPts = frame->pts;

		mSamples.mLinesize = frame->linesize[0];
		for (int i = 0; i < 4; i++) {
			
			mSamples.mDataTemp[i] = frame->data[i];
		}
		mSamples.mData = mSamples.mDataTemp;
	}
}
