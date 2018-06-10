/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineOutputList.cpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/16
*
*  node������ӿڹ���
*
*/

#include <pipeline/FFL_PipelineNode.hpp>
#include <pipeline/FFL_PipelineOutput.hpp>
#include "PipelineOutputList.hpp"

namespace FFL {
	PipelineOutputList::PipelineOutputList(PipelineNode* node):mNextOutputId(1)
	{
		mPipelineNode = node;

		OutputInfo info = {};		
		mOutputs.push_back(info);
		mNextOutputId= mOutputs.size();
	}
	PipelineOutputList::~PipelineOutputList(){
	}


	//
	//  ����һ���µ�input
	//
	PipelineOutputId PipelineOutputList::createOutput(){

		PipelineOutputId id = mNextOutputId;
		PipelineOutput* output = new PipelineOutput(mPipelineNode, id);

		OutputInfo info = {};
		info.output = output;
		mOutputs.push_back(info);
		mNextOutputId = mOutputs.size();
		return id;
	}
	//
	//  ɾ��һ��input
	//
	void PipelineOutputList::destroyOutput(PipelineOutputId id){
		if (!isValidId(id)) {
			FFL_LOG_WARNING("destroyInput fail. ");
			return;
		}
		
		OutputInfo& info = mOutputs[id];		
		if (!info.output.isEmpty()) {			
			info.output->disconnect();
			info.output = NULL;
		} 
	}
	//
	//  ͨ��id����һ��input
	//
	sp<PipelineOutput> PipelineOutputList::getOutput(PipelineOutputId id){
		if (!isValidId(id)) {
			FFL_LOG_WARNING("destroyInput fail. ");
			return NULL;
		}

		OutputInfo& info = mOutputs[id];
		if (!info.output.isEmpty()) {				
		    return info.output;
		}
		return NULL;
	}
	//
	//  ��ȡ���е���Ч��input
	//
	void PipelineOutputList::getOutputVector(Vector< sp<PipelineOutput> >& list) {
		for (Vector<OutputInfo>::iterator it = mOutputs.begin(); it != mOutputs.end(); it++) {
			OutputInfo& info = *it;
			if (info.output.isEmpty()) {
				continue;
			}			
			list.push_back(info.output);
		}
	}

	bool PipelineOutputList::isValidId(PipelineOutputId id) {
		if (mOutputs.size() <= (size_t)id) {
			FFL_LOG_WARNING("invalid input id. ");
			return false;
		}

		return true;
	}
}