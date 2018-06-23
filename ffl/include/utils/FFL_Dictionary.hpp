#include <utils/FFL_String.hpp>

namespace FFL{		
	class Dictionary {
	public:
		Dictionary();
		~Dictionary();
		//
		//  дһ��key/value
		//
		void setKey(const String& key, const String& value);
		void setKey(const char* key, const String& value);		

		//
		// ͨ��key��ȡֵ,�����Ƿ��ҵ������key
		//
		bool getKey(const String& key, String& value);
		bool getKey(const char* key, String& value);
		//
		// �Ƴ�һ��
		//
		bool removeKey(const char* key);
	};
	
}

