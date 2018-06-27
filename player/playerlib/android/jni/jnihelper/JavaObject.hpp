/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClass.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android jni帮助函数，通过java包名获取java层的类的一些信息
*
*/
#ifndef _JNIJAVA_OBJECT_HPP_
#define _JNIJAVA_OBJECT_HPP_

namespace android {
	class JavaClass;
	template<typename T>
	class JavaObject {
	public:
		JavaObject(T& javaClass,jobject javaObj):
				mJavaClass(javaClass),
				mJavaObj(NULL){
		}
		~JavaObject(){}

		//
		//  java层对象的对应的class
		//
		T& getClass() const {
			return mJavaClass;
		}
        //
		//  java层的对象
		//
		jobject getJavaObject() const{
			return  mJavaObj;
		}

	protected:
		T& mJavaClass;
		jobject mJavaObj;
	};
}
#endif
