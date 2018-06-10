#include <sys/time.h>

/*
 * gStarted���Ƿ��ʼ���� 
 * gStartUs����ʼ����ʱ��ĳ�ʼֵ
 */
static int gStarted=0;
static struct timeval gStartUs;


static void internalTimeInit()
{
	if (gStarted == 0) 
	{
		gettimeofday(&gStartUs, NULL);
		gStarted = 1;
	}
}

static int64_t internalGetUs()
{
	struct timeval now;
	internalTimeInit();

	gettimeofday(&now, NULL);
	int64_t nowUs = (int64_t)((now.tv_sec - gStartUs.tv_sec) * 1000 * 1000 + 
		(now.tv_usec - gStartUs.tv_usec));	
	return nowUs;
}

inline void internalGetTimeString(char* s) {
	int64_t current = (int64_t)internalGetUs();

	time_t ts = gStartTime + (current / 1000000);
	int tus = current % 1000000;

	struct tm* t = 0;
	t = localtime(&ts);
	sprintf(s,
		"%4d%02d%02d%02d%02d%02d:%03d:%03d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec,
		tus / 1000, tus % 1000);
}
