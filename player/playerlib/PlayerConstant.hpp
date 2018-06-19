/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  LogtagConstant.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/12 
 *  
 *  定义日志tag标志
 *
*/

#ifndef _LOG_TAG_CONSTANT_HPP_
#define _LOG_TAG_CONSTANT_HPP_

#define TAG_AUDIO "audio"

#define TAG_TIMESTAMP "timestamp"

//
//  丢帧的阈值us，大于5s后进行丢帧
//
#define SYNC_DROP_FRAME_THRESHOLD_US 5000000
//
//  需要进行同步的阈值，40ms， 25帧的情况下时间值
//  如果与主时钟相差大于40ms则需要进行同步
//
#define SYNC_CHANGE_SPEED_THRESHOLD_US 40000


#endif


