cmake_minimum_required(VERSION 3.4.1)

PROJECT(server)

INCLUDE_DIRECTORIES(
            ./			
            ../ffl/include/)

SET(LOCAL_SRC_FILES
   main.cpp;serverMain.cpp
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



ADD_EXECUTABLE(server  ${LOCAL_SRC_FILES})
TARGET_LINK_LIBRARIES(server FFL)

