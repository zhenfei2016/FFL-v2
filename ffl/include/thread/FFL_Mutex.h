/*
 *  FFL_mutex.h
 *  FFL
 *
 *  Created by zhufeifei on 2017/8/12.
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *  基于sdl库代码修改的  
*/
#ifndef _FFL_MUTEX_H_
#define _FFL_MUTEX_H_

#include "FFL_Core.h"

#ifdef  __cplusplus
extern "C" {
#endif

	struct FFL_semaphore_sys;
	typedef struct FFL_semaphore_sys FFL_sem;

	extern DECLSPEC FFL_sem *FFL_CALL FFL_CreateSemaphore(uint32_t initial_value);
	extern DECLSPEC void FFL_CALL FFL_DestroySemaphore(FFL_sem * sem);
	extern DECLSPEC int FFL_CALL FFL_SemWait(FFL_sem * sem);
	extern DECLSPEC int FFL_CALL FFL_SemTryWait(FFL_sem * sem);
	extern DECLSPEC int FFL_CALL FFL_SemWaitTimeout(FFL_sem * sem, uint32_t ms);
	extern DECLSPEC int FFL_CALL FFL_SemPost(FFL_sem * sem);
	extern DECLSPEC int32_t FFL_CALL FFL_SemValue(FFL_sem * sem);




	struct FFL_mutex_sys;
	typedef struct FFL_mutex_sys FFL_mutex;
	FFL_mutex*  FFL_CreateMutex(void);
	void        FFL_DestroyMutex(FFL_mutex *mutex);
	int         FFL_LockMutex(FFL_mutex *mutex);
	int         FFL_TryLockMutex(FFL_mutex * mutex);
	int         FFL_UnlockMutex(FFL_mutex *mutex);

	struct FFL_cond_sys;
	typedef struct FFL_cond_sys FFL_cond;
	FFL_cond   *FFL_CreateCond(void);
	void        FFL_DestroyCond(FFL_cond *cond);
	void        FFL_DestroyCondP(FFL_cond **cond);
	int         FFL_CondSignal(FFL_cond *cond);
	int         FFL_CondBroadcast(FFL_cond *cond);
	int         FFL_CondWaitTimeout(FFL_cond *cond, FFL_mutex *mutex, uint32_t ms);
	int         FFL_CondWait(FFL_cond *cond, FFL_mutex *mutex);

#ifdef  __cplusplus
}
#endif

#endif

