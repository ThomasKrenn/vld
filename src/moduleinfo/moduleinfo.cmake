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

set(
  VLD_MODULEINFO_HDR
  
  ${CMAKE_CURRENT_LIST_DIR}/moduleinfo.hpp
)

set(
  VLD_MODULEINFO_SRC
  
  ${CMAKE_CURRENT_LIST_DIR}/moduleinfo.cpp

)


target_sources(vld
PRIVATE
  ${VLD_MODULEINFO_HDR}
  ${VLD_MODULEINFO_SRC}
)

source_group("moduleinfo Header" FILES ${VLD_MODULEINFO_HDR})
source_group("moduleinfo Source" FILES ${VLD_MODULEINFO_SRC})
