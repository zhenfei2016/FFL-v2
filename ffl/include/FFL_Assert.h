/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Assert   
*  Created by zhufeifei(34008081@qq.com) on 2017/7/12.
*  https://github.com/zhenfei2016/FFL-v2.git
*
*/
#ifndef _FFL_ASSERT_H_
#define _FFL_ASSERT_H_
#include <assert.h>
#include "FFL_Core.h"
#include "FFL_Log.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define FFL_ASSERT(expr) assert((expr))

#define FFL_ASSERT_LOG(expr,message) \
    {if ( !(expr) )                    \
    {                                  \
        FFL_LOG_ERROR("assert: %s",message);\
        assert((expr));\
    }}           


#ifdef  __cplusplus
}
#endif

#endif
