/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Serializable   
*  Created by zhufeifei(34008081@qq.com) on 2018/03/10 
*
*  �������л��Ľӿ�
*/
#ifndef _FFL_SERILIZABLE_HPP_
#define _FFL_SERILIZABLE_HPP_

#include <ref/FFL_Ref.hpp>

namespace FFL{
   class ByteStream;
   class ISerializable{
   public:
	   virtual void serialization(ByteStream& stream)=0;
	   virtual status_t deserialization(ByteStream& stream)=0;	 
	};


   class Serializable : public ISerializable, public RefBase {
   public:
	   Serializable();
	   ~Serializable();

	   //
	   //  �������л���stream��
	   //
	   virtual void serialization(ByteStream& stream);
	   //
	   //  stream�з����е���ǰ����ʵ����
	   //
	   virtual status_t deserialization(ByteStream& stream);
   };
}


#endif

