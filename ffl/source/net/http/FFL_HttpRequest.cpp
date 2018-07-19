/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_HttpRequest.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/07/15
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  http����
*/
#include <net/http/FFL_HttpRequest.hpp>
#include <net/http/FFL_HttpResponse.hpp>
#include <net/http/FFL_HttpConnect.hpp>

namespace FFL {
	HttpRequest::HttpRequest(HttpConnect* conn):
		mConn(conn){
		reset();
	}
	HttpRequest::~HttpRequest() {		
	}
	void HttpRequest::reset() {
		mReader = NULL;
		mContentType="";
		mContentLength = 0;
		mKeepAlive = false;
	}
	//
	// ��ȡcontent����
	//
	void HttpRequest::getContentType(String& type) const {
		type = mContentType;
	}
	//
	// ��ȡcontent�ĳ���
	//
	int32_t HttpRequest::getContentLength() const {
		return mContentLength;
	}
	//
	//  �Ƿ�Keep_Alive
	//
	bool HttpRequest::isKeepAlive() const {
		return mKeepAlive;
	}
	//
	//  ���һ������
	//
	void HttpRequest::fill(const String& url, NetStreamReader* reader) {
		mUrl.parse(url);

		mReader = reader;
		mHeader.getKey("Content Type", mContentType);
		mHeader.getKeyInt32("Content Length", mContentLength,0);

		int32_t value=0;
		mHeader.getKeyInt32("Keep-alive", value, 0);
		mKeepAlive = value != 0;
	}

	//
	//  ����Ӧ��
	//
	sp<HttpResponse>  HttpRequest::createResponse() {
		HttpResponse* response = new HttpResponse(mConn);
		return response;
	}
}

