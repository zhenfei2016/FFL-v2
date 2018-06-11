cmake_minimum_required(VERSION 3.4.1)

PROJECT(playerLib)

IF (WIN32)
INCLUDE_DIRECTORIES(../../ffl/include/ ../third/FFmpeg3.3.5/output_win_dll/include/ ../third/SDL2/SDL2/)
LINK_DIRECTORIES( ../third/FFmpeg3.3.5/output_win_dll/bin/ ../third/SDL2)

ELSEIF (APPLE)
INCLUDE_DIRECTORIES(../../ffl/include/ ../ffl/include/ /Users/zhufeifei/opensource/ffmpeg/output-so/include/
/Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/usr/local/include)
LINK_DIRECTORIES( /Users/zhufeifei/opensource/ffmpeg/output-so/lib/
 /Users/zhufeifei/Library/Developer/Xcode/DerivedData/SDL-bwbfbcmldscsrehedezutogkvhhf/Build/Products/Debug/)
ENDIF ()



SET(LOCAL_SRC_FILES   
   FFMpeg.cpp;FFMpeg.hpp
   FFL_Player.cpp;FFL_Player.hpp
   FFL_PlayerEvent.hpp;FFL_PlayerEvent.cpp
   Statistic.hpp;Statistic.cpp
   FFL_Texture.hpp;FFL_Texture.cpp
   FFL_Sample.hpp;FFL_Sample.cpp
   FFL_Renderer.hpp;FFL_Renderer.cpp
   FFL_Window.hpp;FFL_Window.cpp   
   FFL_AudioDevice.hpp;FFL_AudioDevice.cpp
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
   VideoComposer.hpp;VideoComposer.cpp
   AudioComposer.hpp;AudioComposer.cpp
   AudioResample.hpp;AudioResample.cpp
   Render.hpp;Render.cpp
   VideoRender.hpp;VideoRender.cpp
   AudioRender.hpp;AudioRender.cpp
   FFMpegStream.hpp;FFMpegStream.cpp
   SDL2Module.hpp;SDL2Module.cpp
   VideoSDL2Render.hpp;VideoSDL2Render.cpp
   AudioSDL2Render.hpp;AudioSDL2Render.cpp

   MessageBase.hpp;MessageBase.cpp
   MessageFFMpegPacket.hpp;MessageFFMpegPacket.cpp
   MessageFFMpegFrame.hpp;MessageFFMpegFrame.cpp
   MessageSDL2Texture.hpp;MessageSDL2Texture.cpp
   TimestampExtrapolator.hpp;TimestampExtrapolator.cpp   
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

