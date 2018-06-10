#include "atomic.h"
#include "FFL_Platform.h"
#include "FFL_Config.h"

#if defined(FFL_ATOMIC_STDCPP)
/*
*  ����ʹ��std���
*/
#include "atomic_stdcplus11.cpp"
#elif defined (WIN32)
#include "atomic_windows.cpp"
#else
#include "atomic_stdcplus11.cpp"
#endif
