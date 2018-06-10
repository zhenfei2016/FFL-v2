/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NodeReader.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/03
*
*  视频文件读写，解复用的
*
*/

#include "NodeReader.hpp"
#include "NodeFFMpegInputFile.hpp"
#include "FFL_Player.hpp"
#include <pipeline/FFL_PipelineInputHandler.hpp>

namespace player {
	NodeReader::NodeReader(){		
	}
	NodeReader::~NodeReader() {
	}
	//
	//  成功创建了node
	//
	void NodeReader::onCreate() {

	}
	//
	//  成功删除了node
	//
	void NodeReader::onDestroy(){

	}

	bool NodeReader::setControlInput(const OutputInterface& output) {
		//
		FFL::sp<FFL::PipelineInputHandler> handler =
			new FFL::ClassMethodPipelineInputHandler<NodeReader>(this, &NodeReader::receivedControl);
		if (!mControlInput.isValid()) {
			mControlInput = createInputInterface(handler, "");
		}
		FFL::sp<FFL::SyncPipelineConnector > conn = new FFL::SyncPipelineConnector();
		getPipeline()->connect(output.mNodeId, output.mId,
			mControlInput.mNodeId, mControlInput.mNodeId, conn);
		return true;
	}
	//
	// 收到控制消息
	//
	void NodeReader::receivedControl(const FFL::sp<FFL::PipelineMessage>& msg) {
		if (!msg.isEmpty()) {
			msg->consume(this);
		}
	}
	//
	//  设置数据消息的输入端口
	//
	bool NodeReader::setDataInput(const OutputInterface& output) {
		//
		FFL::sp<FFL::PipelineInputHandler> handler =
			new FFL::ClassMethodPipelineInputHandler<NodeReader>(this, &NodeReader::receiveData);
		if (!mDataInput.isValid()) {
			mDataInput = createInputInterface(handler, "");
		}
		FFL::sp<FFL::SyncPipelineConnector > conn = new FFL::SyncPipelineConnector();
		getPipeline()->connect(output.mNodeId, output.mId,
			mDataInput.mNodeId, mDataInput.mNodeId, conn);
		return true;
	}
	//
	//  获取数据输出接口
	//
	OutputInterface NodeReader::getDataOutput() {
		if (mDataOutput.isValid()) {
			return  mDataOutput;
		}

		mDataOutput = createOutputInterface();
		return mDataOutput;
	}
	//
	// 收到消息
	//
	void NodeReader::receiveData(const FFL::sp<FFL::PipelineMessage>& msg) {
		postMessage(mDataOutput.mId, msg);
	}
	//
	//  打开
	//
	void NodeReader::play(const char* url) {

		if (1) {
			IStreamManager* mgr = getOwner()->getStreamMgr();
			FFL::sp<NodeFFMpegInputFile> file  = new NodeFFMpegInputFile(mgr);
			file->create(getOwner());

			mImpl = file.get();
			mImpl->play(url);
		}

	}
	//
	//  暂停读，但是不关闭
	//
	void NodeReader::pause() {
		mImpl->pause();
	}
	//
	//  恢复读
	//
	void NodeReader::resume() {
		mImpl->resume();
	}
	//
	// 设置开始读位置
	//
	void NodeReader::seek(int64_t pos) {
		mImpl->seek(pos);
	}
	//
	// 关闭
	//
	void NodeReader::close() {
		mImpl->close();
	}

	//
	// 获取播放时长
	//
	int64_t NodeReader::getDurationUs() {
		return mImpl->getDurationUs();
	}
	//
	// 获取当前的播放位置
	//
	int64_t NodeReader::getCurrentUs() {
		return mImpl->getCurrentUs();
	}
}