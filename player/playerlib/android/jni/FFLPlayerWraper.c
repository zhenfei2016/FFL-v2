#include "FFLPlayerWraper.h"


static void Construct(struct FFLPlayerWraper* pthis);
static void Destruct(struct FFLPlayerWraper* pthis);
/* 设置绘制目标surface */
static void setNativeWindow(struct FFLPlayerWraper* pthis, ANativeWindow* pNativeWindow);
/*  播放，暂停，停止*/
static int Play(struct FFLPlayerWraper* pthis, const char* ip,int port);
static int Pause(struct FFLPlayerWraper* pthis);
static int Stop(struct FFLPlayerWraper* pthis);

static void MyPostMessage(struct FFLPlayerWraper* pthis,int what,int arg1,int age2);

static void Construct(struct FFLPlayerWraper* pthis)
{
	pthis->Construct=Construct;
	pthis->Destruct=Destruct;
	pthis->setNativeWindow= Construct;	
	pthis->postMessage=MyPostMessage;
	

	pthis->m_pCalljava=FFL_Allocate_CallJavaInterface();
}
static void Destruct(struct FFLPlayerWraper* pthis)
{
	Pipeline_Destroy(pthis->m_pPipeline);
	pthis->m_pPipeline = 0;

	if(pthis->m_pCalljava) {
		FFL_Free_CallJavaInterface(pthis->m_pCalljava);
		pthis->m_pCalljava=0;
	}
}

/* 设置绘制目标surface */
static void setNativeWindow(struct FFLPlayerWraper* pthis, ANativeWindow* pNativeWindow)
{
	//Pipeline_SetANativeWindow(pthis->m_pPipeline,pNativeWindow);
}


/*  播放，暂停，停止*/
static int Play(struct FFLPlayerWraper* pthis ,const char* ip,int port)
{
	/*  初始化调用java接口 */
	pthis->m_pCalljava->Init(pthis->m_pCalljava);

	Pipeline_Starup(pthis->m_pPipeline,ip,port);
	return 0;
}
static int Pause(struct FFLPlayerWraper* pthis)
{
	return 0;
}

static int Stop(struct FFLPlayerWraper* pthis)
{
	Pipeline_Shutdown(pthis->m_pPipeline);

	/*  初始化调用java接口 */
	pthis->m_pCalljava->Uninit(pthis->m_pCalljava);

	return 0;
}

static void MyPostMessage(struct FFLPlayerWraper* pthis,int what,int arg1,int age2)
{
	if(pthis->m_pCalljava!=0)
	{
		pthis->m_pCalljava->PostEvent(pthis->m_pCalljava,what,arg1,age2);
	}
}

FFL_IMPLEMENT_CLASS(FFLPlayerWraper, Construct)