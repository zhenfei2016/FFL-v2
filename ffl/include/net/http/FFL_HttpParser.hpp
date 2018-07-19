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
#ifndef _HTTP_NET_PARSER_HTTP_HPP_
#define _HTTP_NET_PARSER_HTTP_HPP_

#include <net/http/FFL_HttpRequest.hpp>
#include <net/http/FFL_HttpResponse.hpp>


namespace FFL {	
	class HttpParserImpl;	
	class HttpParser
	{	
	public:
		HttpParser();
		virtual ~HttpParser();
	public:		
		//
		//  stream :��ȡ���ݣ���������ͷ
		//  parser http���󣬳ɹ�����FFL_OK
		//
		status_t parseRequest(NetStreamReader* stream, HttpRequest* request);
		//
		//  parser httpӦ�𣬳ɹ�����FFL_OK
		//
		status_t parseResponse(NetStreamReader* stream, HttpResponse* response);
	private:	
		//
		//  ��ȡͷ��Ϣ
		//
		HttpHeader* getHeader() const {
			return mHeader;
		}
	private:
		HttpParserImpl* mImpl;
		HttpHeader* mHeader;
	};
}

#endif