/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_PipelineOutputList.hpp
*  Created by zhufeifei(34008081@qq.com) on 2017/12/16
*
*  node������ӿڹ���
*
*/
#ifndef _FFL_PIPELINE_OUTPUT_LIST_HPP_
#define _FFL_PIPELINE_OUTPUT_LIST_HPP_

#include <pipeline/FFL_PipelineOutput.hpp>
#include <utils/FFL_Vector.hpp>

namespace FFL
{
	class Pipeline;
	class PipelineNode;
	class PipelineNodeHandler;

	class PipelineOutputList{
	public:
		PipelineOutputList(PipelineNode* node);
		~PipelineOutputList();

	public:
		//
		//  ����һ���µ�output
		//
		PipelineOutputId createOutput();
		//
		//  ɾ��һ��output
		//
		void destroyOutput(PipelineOutputId id);
		//
		//  ͨ��id����һ��input
		//
		sp<PipelineOutput> getOutput(PipelineOutputId id);
		
		void getOutputVector(Vector< sp<PipelineOutput> >& list);
	private:
		bool isValidId(PipelineOutputId id);
	protected:
		struct  OutputInfo
		{
			OutputInfo() {
				
			}
			sp<PipelineOutput> output;
		};

		Vector<OutputInfo> mOutputs;
		PipelineOutputId mNextOutputId;

		PipelineNode* mPipelineNode;
	};


}


#endif