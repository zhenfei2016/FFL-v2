/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_PipelineInputList.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2017/12/16 
 *  
 *  ��������һ��ӿڵİ�װ����
 *
*/
#include <pipeline/FFL_PipelineNode.hpp>
#include <pipeline/FFL_PipelineInput.hpp>
#include "PipelineInputList.hpp"

namespace FFL {
	PipelineInputList::PipelineInputList(PipelineNode* node):mNextInputId(1)
	{
		mPipelineNode = node;

		InputInfo info = {};		
		mInputs.push_back(info);
		mNextInputId= mInputs.size();
	}
	PipelineInputList::~PipelineInputList(){}


	//
	//  ����һ���µ�input
	//
	PipelineInputId PipelineInputList::createInput(const char* name, sp<PipelineInputHandler> handler){

		PipelineInputId id = mNextInputId;
		PipelineInput* input = new PipelineInput(mPipelineNode, handler, id);
		input->setName(name);

		InputInfo info = {};
		info.input = input;
		mInputs.push_back(info);
		mNextInputId = mInputs.size();
		return input->getId();
	}
	//
	//  ɾ��һ��input
	//
	void PipelineInputList::destroyInput(PipelineInputId id) {
		if (!isValidId(id)) {
			FFL_LOG_WARNING("destroyInput fail. ");
			return;
		}

		InputInfo& info = mInputs[id];
		if (!info.input.isEmpty()) {
			if (info.input->isStarted()) {
				info.input->shutdown();			
			}
			
			info.input->disconnect();
			info.input = NULL;			
		}	
	}
	//
	//  ͨ��id����һ��input
	//
	sp<PipelineInput> PipelineInputList::getInput(PipelineInputId id){
		if (!isValidId(id)) {
			FFL_LOG_WARNING("destroyInput fail. ");
			return NULL;
		}

		InputInfo& info = mInputs[id];
		if (info.input.isEmpty()) {				
			return NULL;
		    
		}
		return info.input;
	}
	//
	//  ��ȡ���е���Ч��input
	//
	void PipelineInputList::getInputVector(Vector< sp<PipelineInput> >& list, InutStatus status ) {
		for (Vector<InputInfo>::iterator it = mInputs.begin(); it != mInputs.end(); it++) {
			InputInfo& info = *it;
			if (info.input.isEmpty()) {
				continue;
			}

			if (status == ALL) {
				list.push_back(info.input);
			}else if (status == STARTED) {
				if(info.input->isStarted())
				   list.push_back(info.input);
			}
			else if (status == STOPED) {
				if (!info.input->isStarted())
					list.push_back(info.input);
			}			
		}
	}

	bool PipelineInputList::isValidId(PipelineInputId id) {
		if (mInputs.size() <= (size_t)id) {
			FFL_LOG_WARNING("invalid input id. ");
			return false;
		}

		return true;
	}
}