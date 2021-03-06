/*
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_NetAnyAddrServer.c
 *  Created by zhufeifei(34008081@qq.com) on 2017/8/12.
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  当前机子的所有网卡创建服务器，参考android adb代码
 *
 */
#include "internalSocket.h"
#include <net/base/FFL_Net.h>

#define LISTEN_BACKLOG 4

SOCKET_STATUS FFL_socketAnyAddrServer(int port, int type,NetFD*fd){
    struct sockaddr_in addr;
    int s, n;
	FFL_zerop(&addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, type, 0);
	if (s < 0) {
		return FFL_ERROR_SOCKET_CREATE;
	}
	
    n = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR,(const char*) (&n), sizeof(n));
    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        FFL_socketClose(s);
        return FFL_ERROR_SOCKET_BIND;
    }

    if (type == SOCK_STREAM) {
        int ret;

        ret = listen(s, LISTEN_BACKLOG);
        if (ret < 0) {
			FFL_socketClose(s);
            return FFL_ERROR_SOCKET_LISTEN;
        }
    }

	*fd = s;
    return FFL_SOCKET_OK;
}

