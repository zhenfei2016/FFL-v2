cmake_minimum_required(VERSION 3.4.1)

PROJECT(player)

IF (WIN32)
INCLUDE_DIRECTORIES(../playerlib ../../ffl/include/ ../third/FFmpeg3.3.5/output_win_dll/include/ ../third/SDL2/SDL2/)
LINK_DIRECTORIES( ../third/FFmpeg3.3.5/output_win_dll/bin/ ../third/SDL2)

ELSEIF (APPLE)
INCLUDE_DIRECTORIES(../playerlib  ../../ffl/include/ /Users/zhufeifei/opensource/ffmpeg/output-so/include/
/Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/usr/local/include)
LINK_DIRECTORIES( /Users/zhufeifei/opensource/ffmpeg/output-so/lib/
 /Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/)
ENDIF ()

SET(LOCAL_SRC_FILES
   main.cpp
   player.cpp)

   
if (MSVC)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(MSVC)



ADD_EXECUTABLE(player  ${LOCAL_SRC_FILES})


IF (WIN32)
TARGET_LINK_LIBRARIES(player playerLib FFL avcodec avdevice avfilter avformat avutil swscale swresample SDL2)
ELSEIF (APPLE)
TARGET_LINK_LIBRARIES(player playerLib FFL avcodec avdevice avfilter avformat avutil swscale swresample SDL2)
ELSEIF (ANDROID)
TARGET_LINK_LIBRARIES(player playerLib FFL libffmpeg.so)
ENDIF ()
