/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Commandline.c
*  Created by zhufeifei(34008081@qq.com) on 2018/06/19
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  ���������а�������
*
*/
#include <FFL_Commandline.h>
#include "getopt/getopt.h"

static struct option* alloc_getopt_Option(CmdOption* opts, int size) {	
	struct option* optionList = FFL_mallocz((size +1)* sizeof(struct option) );
	for (int i = 0; i < size; i++) {
		if (!opts[i].mName) {
			opts[i].mName = "__dummy__";
			continue;
		};

		optionList[i].name = FFL_strdup(opts[i].mName);
		optionList[i].has_arg = optional_argument;
		optionList[i].flag = NULL;
		optionList[i].val = 0;		
	}
	return optionList;
}

static void free_getopt_Option(struct option* opts) {
	struct option* opt = opts;
	while (opt->name)
	{
		FFL_free((void *)opt->name);
		opt++;
	}

	FFL_free(opts);
}

/*
*   ����������
*   argc����������
*   argv����������
*   opts��֧�ֵ��������飬��null��β
*   size��opts����Ĵ���
*   �����и�ʽ  --cmd=12344
*   ����������opts�е�λ�ã�û�ҵ�����-1
*/
int FFL_parseCommnadline(int argc,char** argv,CmdOption* opts, int size) {
	struct option* longOpt=alloc_getopt_Option(opts, size);
	
	int cmdIndex = -1;
	int optionIndex=-1;
	int opt = -1;
	optind = 1;	
	while ((opt = getopt_long_only(argc, argv, "", longOpt, &optionIndex)) != -1)
	{
		if (optionIndex != -1) {
			cmdIndex = optionIndex;
			if (opts[cmdIndex].fun) {
				opts[cmdIndex].fun(optarg);
			}
		}
		optionIndex = -1;
	}
	free_getopt_Option(longOpt);

	return cmdIndex;
}