/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Commandline.h   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/19 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ���������а�������
*
*/
#ifndef _FFL_COMMANDLINE_HPP_
#define _FFL_COMMANDLINE_HPP_

#include <FFL.h>
#ifdef  __cplusplus
extern "C" {
#endif

/*
* �����в���ѡ��
*/
typedef struct CmdOption {
	char* mName;		
	void(*fun)(const char* value);
	char* nHelp;	
}CmdOption;

/*
*   ����������
*   argc����������
*   argv����������
*   opts��֧�ֵ��������飬��null��β
*   size��opts����Ĵ���
*   �����и�ʽ  --cmd=12344
*   ����������opts�е�λ�ã�û�ҵ�����-1
*/
int FFL_parseCommnadline(int argc, char** argv, CmdOption* opts,int size);

#ifdef  __cplusplus
}
#endif

#endif
