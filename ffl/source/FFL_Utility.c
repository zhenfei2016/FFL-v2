/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Utility.c
*  Created by zhufeifei(34008081@qq.com) on 2018/06/10
*  https://github.com/zhenfei2016/FFL-v2.git
*
*/
#include <FFL.h>
#include "atomic/atomic.h"
static AtomicInt g_atomicId;
int FFL_generateId(){
	return atomicInc(&g_atomicId);
}