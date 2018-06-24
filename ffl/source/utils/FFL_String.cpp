#include <utils/FFL_String.hpp>

namespace FFL{		
	static const int32_t MAX_LOG_LENGTH = 4096;
	void formatString(String& str, const char *fmt, ...){	
		char buf[MAX_LOG_LENGTH] = "";
		va_list args;
		va_start(args, fmt);				
		vsnprintf(buf, MAX_LOG_LENGTH - 1, fmt, args);
		va_end(args);

		str.clear();
		str.append(buf);
	}

	int64_t String2int64_t(String& str){
		return 0;
	}

	int32_t String2int32_t(String& str) {
		return 0;
	}
}

