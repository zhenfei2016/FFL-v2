/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_List.hpp
*  Created by zhufeifei(34008081@qq.com) on ���� 2018.
*
*  String��װ����ֱ����std�⣬ ��һ�����Ʒ����Ժ���滻
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

