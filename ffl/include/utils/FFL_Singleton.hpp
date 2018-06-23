/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_Singleton.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/22 
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  
 *  ����ģ��
 *  example:
 *  .hpp
 *  class A : public Singleton<A>{
 *    public:
 *       void test();
 *  }
 *  
 *  .cpp
 *  FFL_IMPLEMENT_SINGLETON(A);
 *  void A::test(){}
 *
 *  usage:
 *  A::getInstance().test();
 *  
*/
#ifndef _FFL_SINGLE_INSTANCE_HPP_
#define _FFL_SINGLE_INSTANCE_HPP_

#include <FFL.h>
#include <utils/FFL_Utils.hpp>
#include <thread/FFL_Mutex.hpp>

namespace FFL{	
	//
	//  ���е����๫�õ�һ����
	//
	CMutex& getSingletonLock();

	template < typename T >
	class Singleton {
		protected:
			Singleton() {			
			}
			~Singleton() {}
		public:
			//
			//  ��ȡ����
			//
			static T &getInstance(){
				if (sInstance != NULL) {
					return *sInstance;
				}				

				CMutex::Autolock l(getSingletonLock());
				if (sInstance) {
					return *sInstance;
				}
				sInstance = new T();				
				return *sInstance;
			}
			//
			//  �ͷŵ������󣬲�������������
			//  ���һ��Ҫ������������Ҫȷ�� getInstance()��õĵ�������û����ʹ���У�getInstance()û�����ڵ�����
			//  
			//
			static void deleteInstance() {				
				CMutex::Autolock l(getSingletonLock());
				if (sInstance) {
					delete sInstance;
					sInstance = NULL;
				}
			}
		protected:
			static T* sInstance;				
			DISABLE_COPY_CONSTRUCTORS(Singleton);
	};	
   #define FFL_IMPLEMENT_SINGLETON(ClassName) ClassName* FFL::Singleton< ClassName >::sInstance=0
	
}
#endif

