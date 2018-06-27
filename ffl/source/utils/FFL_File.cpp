/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_File.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/20
*  https://github.com/zhenfei2016/FFL-v2.git
*  文件操作类
*
*/

#include <utils/FFL_File.hpp>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/uio.h>
#endif
#include <fcntl.h>

enum OpenFileMode{
	MODE_OPEN,
	MODE_APPEND,
	MODE_ALWAYS_CREATE,
};


struct FileHandle{
#ifdef _WIN32
	HANDLE fd;
#else
	int fd;
#endif
};

#ifdef WIN32
FileHandle* createFile(const char* path, OpenFileMode mode) {
	DWORD openMode = OPEN_EXISTING;
	if (mode == MODE_OPEN) {
		openMode = OPEN_EXISTING;
	}else if (mode == MODE_ALWAYS_CREATE) {
		openMode = CREATE_ALWAYS;
	}else {

	}

	HANDLE h=::CreateFileA(path, 
		GENERIC_READ| GENERIC_WRITE, 
		FILE_SHARE_WRITE |FILE_SHARE_READ,
		NULL,
		openMode,
		NULL,NULL);

	if (h != INVALID_HANDLE_VALUE) {
		FileHandle* handle = new FileHandle();
		handle->fd = h;

		if (MODE_APPEND) {

		}
		return handle;
	}
	return NULL;
}
void closeFile(FileHandle* fd) {
	if (fd) {
		::CloseHandle(fd->fd);
		delete fd;
	}
}

int writeFile(FileHandle* fd,void* data,int32_t size) {
	if (fd) {
		DWORD dwWrited = 0;
		if (::WriteFile(fd->fd, data, size, &dwWrited, NULL)) {
			return (int)dwWrited;
		}
	}
	return 0;
}
int readFile(FileHandle* fd, uint8_t* buf, int32_t size) {
	if (fd) {
		DWORD dwReaded = 0;
		if (::ReadFile(fd->fd, buf, size, &dwReaded, NULL)) {
			return (int)dwReaded;
		}
	}
	return 0;
}
#else
FileHandle* createFile(const char* path, OpenFileMode mode) {
      return NULL;
}
void closeFile(FileHandle* fd) {
}

int writeFile(FileHandle* fd,void* data,int32_t size) {
    return 0;
}
int readFile(FileHandle* fd, uint8_t* buf, int32_t size) {
    return 0;
}
#endif

namespace FFL {
	File::File() {
		mFd = NULL;
	}
	File::~File() {
		close();		
	}
	status_t File::open(const String& path){		
		if (isOpened()) {
			return FFL_FILE_ALREADY_OPENED;			
		}

		open(path.c_str(), MODE_OPEN);
		mPath = path;
		return FFL_OK;
	}
	//
	//  追加模式打开文件，FFL_OK成功
	//  path:文件绝对路径
	//
	status_t File::openAppend(const String& path) {
		if (isOpened()) {
			return FFL_FILE_ALREADY_OPENED;
		}

		open(path.c_str(), MODE_APPEND);
		mPath = path;
		return FFL_OK;
	}
	//
	// 创建文件,文件已经存在的情况下覆盖原文件
	//
	status_t File::create(const String& path) {
		if (isOpened()) {
			return FFL_FILE_ALREADY_OPENED;
		}

		open(path.c_str(), MODE_ALWAYS_CREATE);
		mPath = path;
		return FFL_OK;
	}
	//
	//  打开文件，FFL_OK成功
	//  path:文件绝对路径
	//
	status_t File::open(const char* path, int mode) {
		mFd=createFile(path,(OpenFileMode)mode);
		return mFd != NULL ? FFL_OK:FFL_FILE_OPEN_FAILED;
	}

	void File::close(){
        if (mFd ==NULL) {
			return;
		}

		closeFile((FileHandle*)mFd);
		mFd = NULL;
	}

	bool File::isOpened() const{
		return mFd !=NULL;
	}
	//
	//  写数据到文件中
	//  buf:缓冲区地址
	//  count:缓冲区大小
	//  pWrite:实质上写了多少数据
	//  返回错误码  ： FFL_OK表示成功
	//
	status_t File::write(void* buf, size_t count, size_t* pWrite){
		int ret = FFL_OK;

		int nWrited;
		if ((nWrited = writeFile((FileHandle*)mFd, buf, count)) < 0) {
			ret = FFL_FILE_WRITE_FAILED;
			return ret;
		}

		if (pWrite != NULL) {
			*pWrite = nWrited;
		}

		return ret;
	}

	status_t File::writeVec(const BufferVec* bufVec, int count, size_t* pWrite){
		int ret = FFL_OK;

		size_t nWrited = 0;
		for (int i = 0; i < count; i++) {
			const BufferVec* buf = bufVec + i;
			size_t n = 0;
			if ((ret = write(buf->data, buf->size, &n)) != FFL_OK) {
				return ret;
			}
			nWrited += n;
		}

		if (pWrite) {
			*pWrite = nWrited;
		}

		return ret;
	}

	//
	//  读数据到缓冲区
	//  buf:缓冲区地址
	//  count:需要读的大小
	//  pReaded:实质上读了多少数据
	//  返回错误码  ： FFL_OK表示成功
	//
	status_t File::read(uint8_t* buf, size_t count, size_t* pReaded) {
		int ret = FFL_OK;

		int nReaded;
		if ((nReaded = readFile((FileHandle*)mFd, buf, count)) < 0) {
			ret = FFL_FILE_WRITE_FAILED;
			return ret;
		}

		if (pReaded != NULL) {
			*pReaded = nReaded;
		}

		return ret;
	}

	//
	//  文件是否创建了
	//
	bool fileIsExist(const char* path) {
		return true;
	}
		
}

