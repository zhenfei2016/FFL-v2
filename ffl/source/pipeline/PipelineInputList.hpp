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

#ifndef _FFL_PIPELINEINPUT_LIST_HPP_
#define _FFL_PIPELINEINPUT_LIST_HPP_

#include <pipeline/FFL_PipelineInput.hpp>
#include <utils/FFL_Vector.hpp>

namespace FFL
{
	class Pipeline;
	class PipelineNode;
	class PipelineNodeHandler;

	class PipelineInputList{
	public:
		PipelineInputList(PipelineNode* node);
		~PipelineInputList();	

	public:
		//
		//  ����һ���µ�input
		//
		PipelineInputId createInput(const char* name, sp<PipelineInputHandler> handler);
		//
		//  ɾ��һ��input
		//
		void destroyInput(PipelineInputId id);
		//
		//  ͨ��id����һ��input
		//
		sp<PipelineInput> getInput(PipelineInputId id);
		//
		//  ��ȡ���е���Ч��input
		//
		enum InutStatus {			
			STARTED=0x1,
			STOPED=0x2,
			ALL = 0x03,
		};
		void getInputVector(Vector< sp<PipelineInput> >& list, InutStatus status=ALL);
	private:
		bool isValidId(PipelineInputId id);
	protected:
		struct  InputInfo
		{
			InputInfo() {
			
			}
			sp<PipelineInput> input;			
		};

		Vector<InputInfo> mInputs;
		PipelineInputId mNextInputId;

		PipelineNode* mPipelineNode;
	};


}


#endif
