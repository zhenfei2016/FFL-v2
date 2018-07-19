/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_HttpParser.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/07/19
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ����http���󣬷���header /body��
*/

#include <utils/FFL_String.hpp>
#include <utils/FFL_Dictionary.hpp>
#include <utils/FFL_ByteStream.hpp>
#include <net/http/FFL_HttpParser.hpp>
#include <net/FFL_NetStream.hpp>
#include "httpParserImpl.hpp"

namespace FFL{
	HttpParser::HttpParser(){
		mImpl = new HttpParserImpl();
		mImpl->init(HTTP_BOTH);
	}
	HttpParser::~HttpParser(){
		FFL_SafeFree(mImpl);
		mImpl = NULL;
	}
	//
	//  parser http���󣬳ɹ�����FFL_OK
	//
	status_t HttpParser::parseRequest(NetStreamReader* stream, HttpRequest* request) {
		return mImpl->parseRequest(stream,request);
	}
	//
	//  parser httpӦ�𣬳ɹ�����FFL_OK
	//
	status_t HttpParser::parseResponse(NetStreamReader* stream, HttpResponse* response) {
		return mImpl->parseResponse(stream,response);
	}

}