#include <utils/FFL_Dictionary.hpp>

namespace FFL{		
	Dictionary::Dictionary(){}
	Dictionary::~Dictionary(){}

	//
	//  дһ��key/value
	//
	void Dictionary::setKey(const String& key, const String& value){
	
	}
	void Dictionary::setKey(const char* key, const String& value) {
	
	}
	//
	// ͨ��key��ȡֵ,�����Ƿ��ҵ������key
	//
	bool Dictionary::getKey(const String& key, String& value) {
		return false;
	}
	bool Dictionary::getKey(const char* key, String& value) {
		return false;
	}

	//
	// �Ƴ�һ��
	//
	bool Dictionary::removeKey(const char* key) {
		return false;
	}
}

