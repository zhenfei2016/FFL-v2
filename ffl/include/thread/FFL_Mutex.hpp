/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Mutex   
*  Created by zhufeifei(34008081@qq.com) on 2017/11/25 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  锁，条件变量包装
*/
#ifndef _FFL_MUTEX_HPP_
#define _FFL_MUTEX_HPP_

#include <FFL.h>
#include <thread/FFL_Mutex.h>


namespace FFL {

	class CCondition;

	class CMutex
	{
	public:		
		CMutex();
		~CMutex();
		
		status_t    lock();
		void        unlock();		
		status_t    tryLock();

	public:
		class Autolock
		{
		public:
			inline Autolock(CMutex& mutex) : mLock(mutex) { mLock.lock(); }
			inline Autolock(CMutex* mutex) : mLock(*mutex) { mLock.lock(); }
			inline ~Autolock() { mLock.unlock(); }
		private:
			CMutex& mLock;
		};
	private:
		friend class CCondition;
		FFL_mutex* m_mutex;

		DISABLE_COPY_CONSTRUCTORS(CMutex);
	};


	class CCondition
	{
	public:
		CCondition();
		~CCondition();
		
		status_t wait(CMutex& mutex);
		status_t waitRelative(CMutex& mutex, uint32_t ms);
		
		void signal();				
		void broadcast();
	private:	
		FFL_cond* m_cond;
		DISABLE_COPY_CONSTRUCTORS(CCondition);
	};
}; 
   
#endif
