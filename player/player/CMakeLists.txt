cmake_minimum_required(VERSION 3.4.1)

PROJECT(player)

IF (WIN32)
INCLUDE_DIRECTORIES(
            ./
			../playerlib
            ../../ffl/include/			
            ../../third/FFmpeg3.3.5/include/
            ../../third/SDL2/include/ )
LINK_DIRECTORIES(
            ../../third/FFmpeg3.3.5/output_win_dll/bin/
            ../../third/SDL2/lib/window/ )
ELSEIF(IOS_PLATFORM)
INCLUDE_DIRECTORIES(
          ./
          ../playerlib
          ../../ffl/include/
          ../../third/FFmpeg3.3.5/include/
          ../../third/SDL2/include/ )
LINK_DIRECTORIES(
          ../../third/FFmpeg3.3.5/output_ios_static/lib/
          ../../third/SDL2/lib/ios/Debug-iphonesimulator/)
ELSEIF (APPLE)
INCLUDE_DIRECTORIES(
            ./
			../playerlib
            ../../ffl/include/
            ../../third/FFmpeg3.3.5/include/
            ../../third/SDL2/include/ )
LINK_DIRECTORIES(
            ../../third/FFmpeg3.3.5/output_mac_dylib/lib/
            ../../third/SDL2/lib/mac/ )
ELSEIF(ANDROID)
INCLUDE_DIRECTORIES(
            ./
            ../../ffl/include/
            ../../third/FFmpeg3.3.5/include/ )
LINK_DIRECTORIES(
            ../../third/FFmpeg3.3.5/output_android_static/)

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
ELSEIF (IOS_PLATFORM)
TARGET_LINK_LIBRARIES(player playerLib FFL avcodec avdevice avfilter avformat avutil swscale swresample SDL2)
ELSEIF (APPLE)
TARGET_LINK_LIBRARIES(player playerLib FFL avcodec avdevice avfilter avformat avutil swscale swresample SDL2)
ELSEIF (ANDROID)
TARGET_LINK_LIBRARIES(player playerLib FFL libffmpeg.so)
ENDIF ()



IF (WIN32)
INSTALL(TARGETS player RUNTIME DESTINATION  ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin
        LIBRARY DESTINATION  ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/libs
        ARCHIVE DESTINATION  ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/libs)

INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/avcodec-57.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/avdevice-57.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/avfilter-6.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/avformat-57.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/avutil-55.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/postproc-54.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/swresample-2.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/FFmpeg3.3.5/output_win_dll/bin/swscale-4.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )
INSTALL(FILES  ../../third/SDL2/lib/window/SDL2.dll DESTINATION ${PROJECT_SOURCE_DIR}/../../ide/mfc2015Player/bin/ )

ENDIF()

