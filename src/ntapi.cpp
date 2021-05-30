/***********************************************************************
* Visual Leak Detector - Global NT API Function Pointers
* Copyright (c) 2005-2014 VLD Team
*
* Copyright (c) 2020-2021 Thomas Krenn
*
* This file is part of VLD.
*
* VLD is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* VLD is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include "stdafx.h"
#define VLDBUILD
#include "ntapi.h"

// Global function pointers for explicit dynamic linking with NT APIs that can't
// be load-time linked (there is no import library available for these).
LdrLoadDll_t        LdrLoadDll;
LdrLoadDllWin8_t    LdrLoadDllWin8;
RtlAllocateHeap_t   RtlAllocateHeap;
RtlFreeHeap_t       RtlFreeHeap;
RtlReAllocateHeap_t RtlReAllocateHeap;

LdrGetDllHandle_t   LdrGetDllHandle;
LdrGetProcedureAddress_t LdrGetProcedureAddress;
LdrUnloadDll_t      LdrUnloadDll;
LdrLockLoaderLock_t LdrLockLoaderLock;
LdrUnlockLoaderLock_t LdrUnlockLoaderLock;