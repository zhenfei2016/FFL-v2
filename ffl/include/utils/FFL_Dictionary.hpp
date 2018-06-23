#include <utils/FFL_String.hpp>

namespace FFL{		
	class Dictionary {
	public:
		Dictionary();
		~Dictionary();
		//
		//  写一条key/value
		//
		void setKey(const String& key, const String& value);
		void setKey(const char* key, const String& value);		

		//
		// 通过key获取值,返回是否找到了这个key
		//
		bool getKey(const String& key, String& value);
		bool getKey(const char* key, String& value);
		//
		// 移除一条
		//
		bool removeKey(const char* key);
	};
	
}

