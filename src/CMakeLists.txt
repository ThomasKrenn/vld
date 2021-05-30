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
 

add_library(vld SHARED "")

include("moduleinfo/moduleinfo.cmake")
include("fmt/fmt.cmake")


set(
 VLD_SRC
 "callstack.cpp"
 "dllspatches.cpp"
 "ntapi.cpp"
 "stdafx.cpp"
 "utility.cpp"
 "vld.cpp"
 "vldapi.cpp"
 "vldheap.cpp"
 "vld_hooks.cpp"
)

set(
 VLD_HDR
 "callstack.h"
 "criticalsection.h"
 "crtmfcpatch.h"
 "dbghelpint.h"
 "loaderlock.h"
 "map.h"
 "ntapi.h"
 "resource.h"
 "set.h"
 "stdafx.h"
 "tree.h"
 "vld.h"
 "vldallocator.h"
 "utility.h"
 "version.h"
 "vldheap.h"
 "vldint.h"
 "vld_def.h"
)

target_compile_definitions(vld
 PRIVATE
 	VLD_EXPORTS
	_USRDLL
   WIN64 
	_WINDOWS
   _UNICODE
	_WIN32_WINNT=0x0502
)

target_include_directories(vld
 PRIVATE
 ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(vld
PRIVATE
 ${VLD_HDR}
 ${VLD_SRC}
)

source_group("vld Header" FILES ${VLD_HDR})
source_group("vld Source" FILES ${VLD_SRC})
