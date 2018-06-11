cmake_minimum_required(VERSION 3.4.1)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

if (MSVC)
add_definitions("/wd4819")
endif(MSVC)

ADD_SUBDIRECTORY(ffl)
ADD_SUBDIRECTORY(player)


