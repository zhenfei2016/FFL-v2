#ifndef _JNIJAVA_OBJECT_HPP_
#define _JNIJAVA_OBJECT_HPP_

namespace android {
	class JavaClass;
	template<typename T>
	class JavaObject {
	public:
		JavaObject(T& javaClass,jobject javaObj):
				mJavaClass(javaClass),
				mJavaObj(NULL){
		}
		~JavaObject(){}

		//
		//  java层对象的对应的class
		//
		T& getClass() const {
			return mJavaClass;
		}
        //
		//  java层的对象
		//
		jobject getJavaObject() const{
			return  mJavaObj;
		}

	protected:
		T& mJavaClass;
		jobject mJavaObj;
	};
}
#endif
