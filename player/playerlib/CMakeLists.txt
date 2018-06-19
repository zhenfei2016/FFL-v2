cmake_minimum_required(VERSION 3.4.1)

PROJECT(playerLib)

INCLUDE_DIRECTORIES(../../ffl/include/ ../third/FFmpeg3.3.5/output_android_static/include/ )
LINK_DIRECTORIES(../third/FFmpeg3.3.5/output_android_static/)
IF (WIN32)
INCLUDE_DIRECTORIES(../../ffl/include/ ../third/FFmpeg3.3.5/output_win_dll/include/ ../third/SDL2/SDL2/)
LINK_DIRECTORIES( ../third/FFmpeg3.3.5/output_win_dll/bin/ ../third/SDL2)
ELSEIF (APPLE)
INCLUDE_DIRECTORIES(../../ffl/include/ ../ffl/include/ /Users/zhufeifei/opensource/ffmpeg/output-so/include/ /Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/usr/local/include)
LINK_DIRECTORIES( /Users/zhufeifei/opensource/ffmpeg/output-so/lib/ /Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux" )
INCLUDE_DIRECTORIES(../../ffl/include/ ../third/FFmpeg3.3.5/output_android_static/include/ )
LINK_DIRECTORIES(../third/FFmpeg3.3.5/output_android_static/)
ENDIF ()



SET(LOCAL_SRC_FILES   
   FFMpeg.cpp;FFMpeg.hpp
   AVDeviceCreator.cpp;AVDeviceCreator.hpp
   Player.cpp;Player.hpp
   PlayerEvent.hpp;PlayerEvent.cpp
   PlayerConstant.hpp
   Statistic.hpp;Statistic.cpp   
      
   RenderWindow.hpp;RenderWindow.cpp      
   NodeBase.cpp;NodeBase.hpp
   IReader.hpp
   NodeReader.cpp;NodeReader.hpp
   StreamManager.cpp;StreamManager.hpp
   NodeFFMpegInputFile.cpp;NodeFFMpegInputFile.hpp   
   NodeFFMpegDecoder.hpp;NodeFFMpegDecoder.cpp
   NodeFFMpegVideoDecoder.hpp;NodeFFMpegVideoDecoder.cpp
   NodeFFMpegAudioDecoder.hpp;NodeFFMpegAudioDecoder.cpp
   Stream.hpp;Stream.cpp
   Decoder.hpp;Decoder.cpp
   Composer.hpp;Composer.cpp  
   AudioUtils.hpp;AudioUtils.cpp 
   AudioFormat.hpp;AudioFormat.cpp
   AudioDevice.hpp;AudioDevice.cpp
   AudioStream.hpp;AudioStream.cpp
   AudioSample.hpp;AudioSample.cpp   
   AudioRender.hpp;AudioRender.cpp
   AudioResample.hpp;AudioResample.cpp
   AudioComposer.hpp;AudioComposer.cpp
   VideoComposer.hpp;VideoComposer.cpp
   VideoUtils.hpp;VideoUtils.cpp 
   VideoDevice.hpp;VideoDevice.cpp
   VideoStream.hpp;VideoStream.cpp
   VideoTexture.hpp;VideoTexture.cpp   
   VideoScale.hpp;VideoScale.cpp   
   VideoRender.hpp;VideoRender.cpp   
   Render.hpp;Render.cpp      
   FFMpegStream.hpp;FFMpegStream.cpp
   SDL2Module.hpp;SDL2Module.cpp   
   SDL2AudioDevice.hpp;SDL2AudioDevice.cpp   
   SDL2VideoDevice.hpp;SDL2VideoDevice.cpp
   MessageBase.hpp;MessageBase.cpp
   MessageFFMpegPacket.hpp;MessageFFMpegPacket.cpp
   MessageFFMpegFrame.hpp;MessageFFMpegFrame.cpp
   MessageSDL2Texture.hpp;MessageSDL2Texture.cpp
   TimestampExtrapolator.hpp;TimestampExtrapolator.cpp    
   ClockUpdater.hpp;ClockUpdater.cpp
   TimestampUtils.hpp;TimestampUtils.cpp
   )


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

add_library( playerLib STATIC ${LOCAL_SRC_FILES})

