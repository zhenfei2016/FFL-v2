 /*
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_Net.c
 *  Created by zhufeifei(34008081@qq.com) on 2017/8/12.
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  网络socket公用函数
 *
 */

#include "internalSocket.h"
#include <net/base/FFL_Net.h>

static int gSocketInited=0;

void FFL_socketInit(){
	if (!gSocketInited) {
		SOCKET_SETUP();
		gSocketInited++;
	}
}
void FFL_socketUninit(){
	if (gSocketInited) {
		SOCKET_CLEANUP();
		gSocketInited=0;
	}
}

static void fdCloseExec(NetFD fd)
{
//#ifndef WIN32
//    int flags = fcntl(fd, F_GETFD);
//    flags |= FD_CLOEXEC;
//    fcntl(fd, F_SETFD, flags);
//#endif
}
/*
*  关闭socket
*/
void FFL_socketClose(NetFD fd) {
	FFL_SOCKET_CLOSE(fd);
}

/*
*  accept一个客户端上来
*/
SOCKET_STATUS FFL_socketAccept(int serverfd, NetFD* clientFd){
	int err = FFL_SOCKET_OK;
	int socketErr = 0;
	struct sockaddr_in addr;
	int addrlen = sizeof(struct sockaddr_in);
	int sockfd = accept(serverfd, (struct sockaddr *) &addr, (socklen_t*)&addrlen);
	if (sockfd < 0) {
		socketErr = SOCKET_ERRNO();
		FFL_LOG_WARNING("FFL_socketAccept error=%d", socketErr);
		//是否超时超时
		if (socketErr == SOCKET_AGAIN ||
			socketErr == SOCKET_ETIME ||
			socketErr == SOCKET_ECONNRESET) {
			err = FFL_ERROR_SOCKET_TIMEOUT;
		}else {
			err = FFL_ERROR_SOCKET_FAILED;
		}
	}

	if (clientFd) {
		*clientFd = sockfd;
	}
	return err;
}

/*
* 读 ,
* readed : 获取读取了数据量
* 失败返回，FFL_ERROR_SOCKET_XX
* 成功返回  FFL_ERROR_SUCCESS
* */
SOCKET_STATUS FFL_socketRead(NetFD fd, void* buffer, size_t size,size_t* readed){
	int socketError=0;
	int nbRead = recv(fd, buffer, size, 0);
	if (nbRead > 0) {
		if (readed) {
			*readed = nbRead;
		}
		return  FFL_SOCKET_OK;
	}

	if (nbRead < 0  )	{
		socketError =SOCKET_ERRNO();	
		FFL_LOG_WARNING("FFL_socketRead error=%d", socketError);
		//读写超时
		if (socketError == SOCKET_AGAIN ||
			socketError == SOCKET_ETIME ||
			socketError == SOCKET_ECONNRESET) {
			return FFL_ERROR_SOCKET_TIMEOUT;
		} else {
			return FFL_ERROR_SOCKET_READ_EX + socketError;
		}
	}

	/* 
	 *  服务端关闭了 
	 */
	if (nbRead == 0) {
		FFL_LOG_WARNING("FFL_socketRead nb_read=0 error=%d",SOCKET_ECONNRESET);
		return FFL_ERROR_SOCKET_READ_EX + SOCKET_ECONNRESET;
	}

	return FFL_ERROR_SOCKET_READ;
}

/*
 * 写 ,
 * writed : 写成功了多少数据
 * 失败返回，FFL_ERROR_SOCKET_XX
 * 成功返回  FFL_ERROR_SUCCESS
 * */
SOCKET_STATUS FFL_socketWrite(NetFD fd, void* buffer, size_t size,size_t* writed){
	int socketError=0;
	int nbWrite = send(fd, buffer, size, 0);
	if(nbWrite>0)
		if (writed)
			*writed = nbWrite;
		return  FFL_ERROR_SUCCESS;

	if (nbWrite < 0  )	{
		socketError=SOCKET_ERRNO();
		FFL_LOG_WARNING("FFL_socketWrite error=%d", socketError);
		/*
		 * 读写超时
	 	 */
		if (socketError == SOCKET_AGAIN ||
			socketError == SOCKET_ETIME ||
			socketError == SOCKET_ECONNRESET) {
			return FFL_ERROR_SOCKET_TIMEOUT;
		}
		else {
			return FFL_ERROR_SOCKET_WRITE_EX + socketError;
		}
	}

	return FFL_ERROR_SOCKET_WRITE;
}


/*
*  设置接收超时值
*/
SOCKET_STATUS FFL_socketSetRecvTimeout(NetFD fd, int64_t ms)
{
	int32_t sec = 0;
	int32_t usec = 0;

	if (ms != 0)
	{
		sec = (int32_t)(ms / 1000);
		usec = (int32_t)((ms % 1000)*1000);
	}

	struct timeval tv = { sec , usec };
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,(const char*)( &tv), sizeof(tv)) == -1)
	{
		return FFL_ERROR_SOCKET_FAILED;
	}
	return FFL_SOCKET_OK;
}

/*
*  设置发送超时值
*/
SOCKET_STATUS FFL_socketSetSendTimeout(NetFD fd, int64_t ms){
	int32_t sec = 0;
	int32_t usec = 0;

	if (ms != 0)
	{
		sec = (int32_t)(ms / 1000);
		usec = (int32_t)((ms % 1000)*1000);
	}

	struct timeval tv = { sec , usec };
	if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)(&tv), sizeof(tv)) == -1)
	{
		return FFL_ERROR_SOCKET_FAILED;
	}
	return FFL_SOCKET_OK;
}
/*
*  设置发送不延迟发送
*/
SOCKET_STATUS FFL_socketSetNodelay(NetFD fd, int yes)
{
#if defined(WIN32)
	int flag = (yes != 0) ? 1 : 0;
	int nSendBuf = 0;
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
#else
	int enable = 1;
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
#endif
	return FFL_SOCKET_OK;
}


//#if defined(MACOSX)
//int get_local_ip(const char *eth_inf, char *ip);
//#endif
//int FFL_socket_get_local_ip(char* ip)
//{
//    char hostname[128];
//    struct in_addr* addr;
//    char* local_ip;
//
//#if defined(MACOSX)
//    return get_local_ip("en0",ip);
//#endif
//    int ret = gethostname(hostname, sizeof(hostname));
//    if (ret == -1) {
//        return -1;
//    }
//
//    struct hostent *hent;
//    hent = gethostbyname(hostname);
//    if (0 == hent)
//    {
//        return -1;
//    }
//
//
//    addr = ((struct in_addr*)hent->h_addr);
//    local_ip= inet_ntoa(*addr);
//    if(local_ip)
//        strcpy(ip,local_ip);
//    return 0;
//
//    //ip = ntohl(((struct in_addr*)hent->h_addr)->s_addr);
//    //int i;
//    //for (i = 0; hent->h_addr_list[i]; i++)
//    //{
//    //	struct in_addr* addr = ((struct in_addr*)hent->h_addr_list[i]);
//    //	uint32_t u = ntohl(addr->s_addr);
//    //	std::cout << u << ":" << inet_ntoa(*addr)<< std::endl;
//    //}
//
//    ////printf("%s", inet_ntoa(in_addr));
//    //return 0;
//}
//
//
//#if defined(MACOSX)
//int get_local_ip(const char *eth_inf, char *ip)
//{
//    int sd;
//    struct sockaddr_in sin;
//    struct ifreq ifr;
//
//    sd = socket(AF_INET, SOCK_DGRAM, 0);
//    if (-1 == sd)
//    {
//        printf("socket error: %s\n", strerror(errno));
//        return -1;
//    }
//
//    strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);
//    ifr.ifr_name[IFNAMSIZ - 1] = 0;
//
//    // if error: No such device
//    if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)
//    {
//        printf("ioctl error: %s\n", strerror(errno));
//        close(sd);
//        return -1;
//    }
//
//    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
//    snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));
//
//    close(sd);
//    return 0;
//}

//#endif
