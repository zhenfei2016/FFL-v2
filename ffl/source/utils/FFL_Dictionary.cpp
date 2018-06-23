#include <utils/FFL_Dictionary.hpp>

namespace FFL{		
	Dictionary::Dictionary(){}
	Dictionary::~Dictionary(){}

	//
	//  写一条key/value
	//
	void Dictionary::setKey(const String& key, const String& value){
	
	}
	void Dictionary::setKey(const char* key, const String& value) {
	
	}
	//
	// 通过key获取值,返回是否找到了这个key
	//
	bool Dictionary::getKey(const String& key, String& value) {
		return false;
	}
	bool Dictionary::getKey(const char* key, String& value) {
		return false;
	}

	//
	// 移除一条
	//
	bool Dictionary::removeKey(const char* key) {
		return false;
	}
}

