/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_HttpRequest.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/15
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  http����
*/
#ifndef _FFL_NET_REQUEST_HTTP_HPP_
#define _FFL_NET_REQUEST_HTTP_HPP_


#include <FFL.h>
#include <ref/FFL_Ref.hpp>
#include <net/http/FFL_HttpHeader.hpp>
#include <net/http/FFL_HttpUrl.hpp>
#include <net/FFL_NetStream.hpp>


namespace FFL {
	class HttpConnect;
	class HttpResponse;
	class HttpRequest : public RefBase {
		friend class HttpParserImpl;
	public:
		HttpRequest(HttpConnect* conn);
		virtual ~HttpRequest();
		//
		// �����������
		//
		void reset();
		//
		// ��ȡcontent����
		//
		void getContentType(String& type) const;
		//
		// ��ȡcontent�ĳ���
		//
		int32_t getContentLength() const;
		//
		//  �Ƿ�Keep_Alive
		//
		bool isKeepAlive() const;

		const HttpUrl& getUrl() const {
			return mUrl;
		} 
	public:
		//
		//  ����Ӧ��
		//
		sp<HttpResponse>  createResponse();
	private:
		//
		//  ���һ������
		//
		void fill(const String& url,NetStreamReader* reader);
	private:
		//
		//  ����ͷ
		//
		NetHttpHeader mHeader;
		String mContentType;
		int32_t mContentLength;
		bool mKeepAlive;

		// parse uri to schema/server:port/path?query
		HttpUrl mUrl;	

		//
		//  ��ȡ�������ӿ�
		//
		NetStreamReader* mReader;
		//
		//  �Ǹ������ϵ�����
		//
		HttpConnect* mConn;
	};
}

#endif