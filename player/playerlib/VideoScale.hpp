/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoScale.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/12 
 *  
 *  ��Ƶ���ţ�ת���
 *
*/

#ifndef _VIDEO_SCALE_HPP_
#define _VIDEO_SCALE_HPP_
#include "NodeBase.hpp"
#include "FFMpeg.hpp"

namespace player {

	class SDL2Module;
	class VideoScale : public NodeBase {
	public:
		struct TextureFormat
		{
			TextureFormat()
			{
				mWidht = 0;
				mHeight = 0;
				mFormat = AV_PIX_FMT_NONE;
			}

			int32_t mWidht;
			int32_t mHeight;
			AVPixelFormat mFormat;
		};
	public:
		VideoScale();
		~VideoScale();
	protected:
		//
		//  ����ͼƬ����������
		//  src: Դ��ʽ
		//  dst: Ŀ���ʽ
		//
		bool createSws(TextureFormat* src, TextureFormat* dst);
		//
		//  ɾ������������
		//
		void destroySws();
	public:
		//
		//  ��ʼ����ͼƬ
		//
		bool scaleVideo(
			const uint8_t** srcPix, int* srcLinesize, int height,
			uint8_t** dstPix, int* dstLinesize);
		
	public:
		//
		//  �ɹ�������node
		//
		virtual void onCreate();

		//
		//   �ⲿsetDataInputʱ����ô˺�����������Ӧconn
		//
		FFL::sp<FFL::PipelineConnector > onCreateConnector(
			const OutputInterface& output,
			const InputInterface& input, void* userdata);
	protected:
		//
		// ������յ�����Ϣ���������false��ʾ�����Ϣû�д���
		// ����true��ʾ�����Ϣ�����ˣ���Ϣ������������msg->consume();
		//
		virtual bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);	
	private:	

		//
		//  ��ǰ��sws����ʽ
		//
		TextureFormat mSourceFormat;
		TextureFormat mDestFormat;
		SwsContext* mSwsCtx;
	};
}

#endif