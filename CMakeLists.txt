#***********************************************************************
#
# Copyright (c) 2020-2021 Thomas Krenn
#
# This file is part of VLD.
#
# VLD is free software. You can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# version 2.1 as published by the Free Software Foundation.
#
# VLD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# https://www.gnu.org/licenses/
#
#**********************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(VLD)


message("------------------------------------------" )
message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}" )
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}" )
message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}" )
message("------------------------------------------" )


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeBuild")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER CMakeMoc)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)



# Add Subfolders
add_subdirectory(src)






