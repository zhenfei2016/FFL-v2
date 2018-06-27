cmake_minimum_required(VERSION 3.4.1)

PROJECT(FFLPlayer)

INCLUDE_DIRECTORIES(
            ../../
            ../../../../ffl/include/
            ../../../third/FFmpeg3.3.5/output_android_static/include/ )
LINK_DIRECTORIES(
           ../../../third/FFmpeg3.3.5/output_android_static/)

SET(LOCAL_SRC_FILES
   jnihelper/JavaClass.hpp;jnihelper/JavaClass.cpp
   jnihelper/JavaObject.hpp;jnihelper/JavaObject.cpp
   
   NativeFFLPlayer.cpp;NativeFFLPlayer.hpp
   JavaFFLPlayer.cpp;JavaFFLPlayer.hpp
   JavaAudiotrack.cpp;JavaAudiotrack.hpp
   JNImain.cpp)


add_library( FFLPlayer SHARED ${LOCAL_SRC_FILES} )
find_library(log-lib  log )
target_link_libraries( FFLPlayer playerLib  android ${log-lib} libffmpeg.so FFL  EGL GLESv2)

