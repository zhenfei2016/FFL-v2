/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeFFMpegInputFile.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10
*
*  文件读取，提取流
*
*/

#pragma once
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineMessageCache.hpp>
#include <string>
#include "NodeBase.hpp"
#include "FFMpeg.hpp"
#include "MessageFFMpegPacket.hpp"
#include "StreamManager.hpp"
#include "IReader.hpp"

namespace player {
	class PlayerCore;	
	class NodeFFMpegInputFile : public NodeBase, public IReader
	{
	public:
		NodeFFMpegInputFile(IStreamManager* streamMgr);
		~NodeFFMpegInputFile();
	protected:
		//
		//  成功创建了node
		//
		virtual void onCreate();
		//
		//  成功删除了node
		//
		virtual void onDestroy();
	public:
		//
		//  打开
		//
		void play(const char* url);
		//
		//  暂停读，但是不关闭
		//
		void pause();
		//
		//  恢复读
		//
		void resume();
		//
		// 设置开始读位置
		//
		void seek(int64_t us);
		//
		// 关闭
		//
		void close();
	public:
		//
		//  获取时长
		//
		int64_t getDurationUs();
		//
		//  获取当前的位置
		//
		int64_t getCurrentUs();
	public:
		//
		// 读取一帧数据,这个会定时触发的
		//
		void readFrame();
	private:
		//
		//初始化pipeline节点，和输入输出接口
		//
		bool init();
	protected:
		std::string mUrl;

		FFL::CMutex mRequestMutex;
		volatile bool mEventOpenPending;
		volatile bool mEventClosePending;
		volatile bool mEventPausePending;
		volatile bool mEventSeekPending;
		volatile bool mIsBuffering;


		bool onOpen();
		bool onSeek();
		bool onClose();
		void onReadFrame();
	private:
		void handleEof();
	private:
		//
		// 打开所有的流
		//
		void openStream(AVStream** streams, uint32_t count);
	private:
        InputInterface mInput;
		enum {
			SUPORT_STREAM_NUM=4,
		};
		struct StreamEntry{
			FFL::sp<Stream> mStream;
			OutputInterface mOutputInterface;

			bool isValid() {
				return !mStream.isEmpty();
			}
		};
		StreamEntry mStreamVector[SUPORT_STREAM_NUM];
		IStreamManager* mStreamManager;
		//
		//  是否循环播放
		//
		uint32_t mLoopPlay;

		uint32_t mEOFFlag;
		//
		//  解复用FFMpeg上下文
		//
		AVFormatContext* mAVFormatContext;
		//
		// 缓存的数据包
		//
		FFL::sp<FFL::PipelineMessageCache> mMessageCache;
        
        //
		// seek到那个地方，每一次seek都会改变mSerialNumber
		//
        int64_t mSeekUs;
		int32_t mSerialNumber;

		//
		// 当前读取到哪一个pts
		//
		int64_t mCurrentPts;
		FFL::TimeBase mCurrentTb;
	};

}
