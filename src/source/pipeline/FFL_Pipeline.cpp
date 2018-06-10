/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_Pipeline.cpp 
 *  Created by zhufeifei(34008081@qq.com) on 2017/12/02 
 *  
 *  ��ˮ��ʵ�������԰���n��Ľڵ� ��Ȼ��������������ˮ��
 *
*/
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>
#include <pipeline/FFL_PipelineEventCallback.hpp>
#include "FFL_PipelineNodeManager.hpp"
#include <pipeline/FFL_PipelineConnector.hpp>

namespace FFL{
	Pipeline::Pipeline(const char* scrip)
	{
		FFL_LOG_DEBUG("Pipeline::Pipeline %p", this);
		mNodeManager = new PipelineNodeManager(this);		
		mEventStartup = new PipelineEvent(new ClassMethodCallback<Pipeline>(this, &Pipeline::handleStartup));
		mEventStartupPending = false;

		mEventShutdown = new PipelineEvent(new ClassMethodCallback<Pipeline>(this, &Pipeline::handleShutdown));
		mEventShutdownPending = false;
		mEventFilter = NULL;		
		
	}	
	Pipeline::~Pipeline()
	{	
		if (mFlags.getFlags() != 0) {
			FFL_LOG_WARNING("delete Pipeline flag!=0  (%d)",mFlags.getFlags());
		}
	
		if (mNodeManager) {			
			delete mNodeManager;
			mNodeManager = NULL;
		}	
		FFL_LOG_DEBUG("Pipeline::~Pipeline %p", this);
	}
	void Pipeline::onFirstRef() {
        sp<Pipeline> pipeline=this;
		mEventLooper = new PipelineEventLooper(pipeline);
		mEventLooper->setName("pipelineEvent");
		mEventLooper->start();
	}

	void Pipeline::onLastStrongRef(const void* id) {
		if (!mEventLooper.isEmpty()) {
			mEventLooper->stop();
			mEventLooper.clear();
		}
	}
	//
	//  ����pipeline, ��Ҫ������PipelineNodeManager
	//  �첽��
	//  �ɹ����� FFL_OK
	//
	status_t Pipeline::startup()
	{
		CMutex::Autolock l(mEventMutex);
		if (mEventStartupPending || mFlags.getFlags() !=0 ) {
			return FFL_INVALID_OPERATION;
		}

		mEventStartupPending = true;
		postEvent(mEventStartup);
		return FFL_OK;
	}
	//
	//  ��������һ��node��ֻ��������pipeline��������ܵ�������
	//  �첽��
	//  �ɹ����� FFL_OK		
	//

	class SingleNodeStartupEvent : public PipelineEvent {
	public:
		SingleNodeStartupEvent(Pipeline* p, PipelineNodeId nodeId):
			PipelineEvent(new ClassMethodCallback<Pipeline>(p, &Pipeline::handleSingleNodeStartup)),
			mNodeId(nodeId)
		{
		}

		PipelineNodeId mNodeId;
	};
	status_t Pipeline::startup(PipelineNodeId nodeId) {
		CMutex::Autolock l(mEventMutex);
		if (mEventStartupPending || (mFlags.hasFlags(SHUDOWNING))) {
			return FFL_INVALID_OPERATION;
		}		
		//postEvent(new SingleNodeStartupEvent(this, nodeId));
        
        handleSingleNodeStartup(new SingleNodeStartupEvent(this, nodeId));
		return FFL_OK;
	}
	//
	//  ֹͣpipeline,�첽��
	//
	status_t Pipeline::shutdown()
	{
		CMutex::Autolock l(mEventMutex);
		if (mEventShutdownPending ||isLooping()==false ) {
			return FFL_INVALID_OPERATION;
		}

		mEventShutdownPending = true;
		postEvent(mEventShutdown);
		return FFL_OK;
	}
	//
	//  ֹͣpipeline,���ȴ��ɹ�
	//
	status_t Pipeline::shutdownAndWait()
	{
		status_t status=shutdown();
		if(status==FFL_OK)
		{
			sp<Pipeline> pipeline = this;
			CMutex::Autolock l(mShutdownMutex);
			mShutdownCond.wait(mShutdownMutex);
		}
		return status;
	}
	//
	//  ��������Ժ����pipeline �Ͳ���ʹ����
	//
	void Pipeline::exit() {
		shutdownAndWait();
		mEventLooper->stop();
		mEventLooper.clear();
	}
	//
	//  �����Ĵ����߼���������¼��߳���ִ��
	//
	void Pipeline::handleStartup(const sp<PipelineEvent>& event)
	{
		mFlags.modifyFlags(STARTUPING,0);
		uint32_t count = mNodeManager->getCount();
		for (uint32_t i = 0; i < count; i++) {			
			PipelineNode* node=mNodeManager->getNode(i);
			node->startup();
		}
		mFlags.modifyFlags(LOOPING,STARTUPING);
		{
			CMutex::Autolock l(mEventMutex);
			mEventStartupPending = false;
		}
	}
	//
	//  ����node�����Ĵ����߼���
	//
	void Pipeline::handleSingleNodeStartup(const sp<PipelineEvent>& event){

		{
			CMutex::Autolock l(mEventMutex);
			if (mFlags.hasFlags(SHUDOWNING)) {			
				return;
			}
		}

		PipelineNodeId nodeId = Pipeline_INVALID_Id;
		SingleNodeStartupEvent* evt = (SingleNodeStartupEvent*)(event.get());
		if (evt != NULL) {
			nodeId = evt->mNodeId;
		}

		//
		//  ���������ڵ�
		//
		if (nodeId != Pipeline_INVALID_Id) {			
			PipelineNode* node = mNodeManager->findNode(nodeId);
			if(node)
			   node->startup();
		}
	}
	//
	//  ֹͣ�Ĵ����߼���������¼��߳���ִ��
	//
	void Pipeline::handleShutdown(const sp<PipelineEvent>& event)
	{		
		mFlags.modifyFlags(SHUDOWNING, LOOPING);
		uint32_t count = mNodeManager->getCount();
		for (uint32_t i = 0; i < count; i++) {
			PipelineNode* node = mNodeManager->getNode(i);
			node->shutdown();
		}
		mFlags.resetFlags(0);

		{
			CMutex::Autolock l(mEventMutex);
			mEventShutdownPending = false;
		}

		mShutdownCond.signal();
	}
	//
	// �����¼����¼�ϵͳ�У� �첽��
	//
	status_t Pipeline::postEvent(const sp<PipelineEvent>& msg)
	{
		if (!msg.isEmpty()) 
		{			
			mEventLooper->postEvent(msg, 0);
		}
		return FFL_OK;
	}
	//
	// �����¼�������
	//
	void Pipeline::setEventFilter(PipelineEventFilter* filter)
	{
		mEventFilter = filter;
	}

	void Pipeline::onEvent(const sp<PipelineEvent> &event)
	{		
		if (mEventFilter && mEventFilter->onPrepareDispatch(event)==false) {
			return;
		}

		for (uint32_t i = 0; i < getNodeManager()->getCount(); i++) {
			PipelineNode* node = getNodeManager()->getNode(i);
			if (node) {
				node->onEvent(event);
			}
		}
	}
	//
	// ��ȡ�ڵ���������������Pipeline���е�PipelineNode
	// �������������Pipelineһ�µ�
	//
	PipelineNodeManager*  Pipeline::getNodeManager() {
		return mNodeManager;
	}

	//
	// ����һ��Node�ڵ�
	// handler�� Ϊ�ڵ�Ĵ����߼����
	// ����Node�ڵ�� id  , PipelineNode::INVALID_NodeId ��ʾ��Чֵ
	//
	PipelineNodeId  Pipeline::createNode(sp<PipelineNodeHandler> handler) {
		return getNodeManager()->createNode(handler);		
	}
	//
	// ����һ������ӿ�
	//
	PipelineInputId Pipeline::createInput(PipelineNodeId nodeId, sp<PipelineInputHandler> handler, const char* name)
	{
		PipelineNode* node=getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("createInput fail . not find node (%d)", nodeId);
			return Pipeline_INVALID_Id;
		}
		return node->createInput(name,handler);
	}
	//
	// ɾ��һ������ӿ�
	//
	void Pipeline::destroyInput(PipelineNodeId nodeId, PipelineInputId id)
	{
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("destroyInput fail . not find node (%d)", nodeId);
			return;
		}
		node->destroyInput(id);		
	}
	//
	// ����һ������ӿ�
	//
	PipelineOutputId Pipeline::createOutput(PipelineNodeId nodeId){
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("createOutput fail . not find node (%d)", nodeId);
			return Pipeline_INVALID_Id;
		}
		return node->createOutput();
	}
	//
	// ɾ��һ������ӿ�
	//
	void Pipeline::destroyOutput(PipelineNodeId nodeId, PipelineOutputId id)
	{
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("destroyOutput fail . not find node (%d)", nodeId);
			return;
		}
		node->destroyOutput(id);
	}
	//
	//ɾ��һ��Node�ڵ� ���ڵ�ɾ��ʱ�����е����룬����ӿڶ��ϵ�
	//
	void Pipeline::destroyNode(PipelineNodeId nodeId) {
		getNodeManager()->destroyNode(nodeId);
	}
	//
	//����һ��Node������ڵ���һ���������
	//conn :��������
	//
	status_t Pipeline::connect(
		PipelineNodeId srcNodeId, PipelineOutputId srcOutputId,
		PipelineNodeId dstNodeId, PipelineInputId dstInputId,
		sp<PipelineConnector> conn) {
		if (conn.is_empty()) {
			return FFL_INVALID_PARAMS;
		}

		PipelineNode* srcNode =getNodeManager()->findNode(srcNodeId);
		if (!srcNode) {
			FFL_LOG_WARNING("Pipeline::connect fail . not find node (%d)", srcNodeId);
			return FFL_INVALID_PARAMS;
		}

		sp<PipelineOutput> srcOutput = srcNode->getOutput(srcOutputId);
		if (srcOutput.isEmpty()) {
			FFL_LOG_WARNING("Pipeline::connect fail . not find node (%d)", srcNodeId);
			return FFL_INVALID_PARAMS;
		}

		PipelineNode* dstNode = getNodeManager()->findNode(dstNodeId);
		if (!dstNode) {
			FFL_LOG_WARNING("Pipeline::connect fail . not find dest node (%d)", dstNodeId);
			return FFL_INVALID_PARAMS;
		}

		sp<PipelineInput> dstInput = dstNode->getInput(dstInputId);
		if (dstInput.isEmpty()) {
			FFL_LOG_WARNING("Pipeline::connect fail . not find dest node (%d)", dstNodeId);
			return FFL_INVALID_PARAMS;
		}		

		srcOutput->connect(conn);
		dstInput->connect(conn);
		return FFL_OK;
	}

	//
	//  ����һ��Դ�ڵ�
	//
	status_t Pipeline::connectSource(PipelineNodeId nodeId, PipelineInputId inputId,
		sp<PipelineSourceConnector> conn) {	    	
			if (conn.is_empty()) {
				return FFL_INVALID_PARAMS;
			}

			PipelineNode* dstNode = getNodeManager()->findNode(nodeId);
			if (!dstNode) {
				FFL_LOG_WARNING("Pipeline::connect fail . not find node (%d)", nodeId);
				return FFL_INVALID_PARAMS;
			}

			sp<PipelineInput> dstInput = dstNode->getInput(inputId);
			if (dstInput.isEmpty()) {
				FFL_LOG_WARNING("Pipeline::connect fail . not find dest node (%d)", inputId);
				return FFL_INVALID_PARAMS;
			}

			dstInput->connect(conn);
			return FFL_OK;
	}
	//
	//�Ͽ���������������
	//
	status_t Pipeline::disconnect(PipelineNodeId nodeId, PipelineOutputId srcOutputId) {
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("destroyOutput fail . not find node (%d)", nodeId);
			return FFL_ERROR_FAIL;
		}

		sp<PipelineOutput> output = node->getOutput(srcOutputId);
		if (!output.isEmpty())
			output->disconnect();
		
		return FFL_OK;
	}
	//
	//�Ͽ�nodeid�����е�����ӿ�
	//
	status_t Pipeline::disconnectInput(PipelineNodeId nodeId) {
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("disconnectInput fail . not find node (%d)", nodeId);
			return FFL_ERROR_FAIL;
		}

		Vector< sp<PipelineInput> > list;
		node->getStartedInputVector(list);

		uint32_t count = list.size();
		for (uint32_t i = 0; i < count; i++) {
			sp<PipelineInput> input = list[i];
			if (!input.isEmpty())
			   input->disconnect();
		}

		return FFL_OK;
	}
	//
	//�Ͽ�nodeid�����е�����ӿ�
	//
	status_t Pipeline::disconnectOutput(PipelineNodeId nodeId) {
		PipelineNode* node = getNodeManager()->findNode(nodeId);
		if (!node) {
			FFL_LOG_WARNING("disconnectInput fail . not find node (%d)", nodeId);
			return FFL_ERROR_FAIL;
		}

		Vector< sp<PipelineOutput> > list;
		node->getOutputVector(list);
		uint32_t count = list.size();

		for (uint32_t i = 0; i < count; i++) {
			sp<PipelineOutput> output = list[i];
			if (!output.isEmpty())
			   output->disconnect();
		}

		return FFL_OK;
	}
	//
	//  �Ƿ�����
	//
	bool Pipeline::isLooping()
	{
		return mFlags.hasFlags(LOOPING);
	}
}
