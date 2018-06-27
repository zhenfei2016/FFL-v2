/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_List.hpp
*  Created by zhufeifei(34008081@qq.com) on 三月 2018.
*
*  String封装，先直接用std库， 换一个名称方便以后的替换
*
*/
#ifndef _FFL_STRING_HPP_
#define _FFL_STRING_HPP_

#include <FFL.h>
#include <string>

namespace FFL {
	
	typedef std::string String;

	void formatString(String& str, const char *fmt, ...);
	int64_t String2int64_t(String& str);
	int32_t String2int32_t(String& str);	
}


#endif

