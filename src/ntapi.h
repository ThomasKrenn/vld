/***********************************************************************
* Visual Leak Detector - NT API Definitions
*
* Copyright (c) 2020-2021 Thomas Krenn
*
* Copyright (c) 2005-2014 VLD Team
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

#pragma once

#ifndef VLDBUILD
#error \
"This header should only be included by Visual Leak Detector when building it from source. \
Applications should never include this header."
#endif

#include <windows.h>

// Return code type used by LdrLoadDll.
typedef __success(return >= 0) LONG NTSTATUS;
#define STATUS_SUCCESS 0

// Unicode string structure used by NT APIs.
struct unicodestring_t {
    USHORT length;    // Length of the string.
    USHORT maxlength; // Length of the buffer.
    PWSTR  buffer;    // The buffer containing the string.
};

// Function pointer types for explicit dynamic linking with functions that can't
// be load-time linked (no import library is available for these).
typedef NTSTATUS (__stdcall *LdrLoadDll_t) (LPWSTR, PULONG, unicodestring_t *, PHANDLE);
typedef NTSTATUS (__stdcall *LdrLoadDllWin8_t) (DWORD_PTR, PULONG, unicodestring_t *, PHANDLE);
typedef LPVOID (__stdcall *RtlAllocateHeap_t) (HANDLE, DWORD, SIZE_T);
typedef BYTE (__stdcall *RtlFreeHeap_t) (HANDLE, DWORD, LPVOID);
typedef LPVOID (__stdcall *RtlReAllocateHeap_t) (HANDLE, DWORD, LPVOID, SIZE_T);

typedef NTSTATUS(NTAPI *LdrGetDllHandle_t) (PWSTR, PULONG, PUNICODE_STRING, PVOID *);
typedef NTSTATUS(NTAPI *LdrGetProcedureAddress_t)(PVOID, PANSI_STRING, ULONG, PVOID *);
typedef NTSTATUS(NTAPI *LdrUnloadDll_t)(PVOID);
typedef NTSTATUS(NTAPI *LdrLockLoaderLock_t)(ULONG, PULONG, PULONG_PTR);
typedef NTSTATUS(NTAPI *LdrUnlockLoaderLock_t)(ULONG, ULONG_PTR);

// Provide forward declarations for the NT APIs for any source files that
// include this header.
extern LdrLoadDll_t        LdrLoadDll;
extern LdrLoadDllWin8_t    LdrLoadDllWin8;
extern RtlAllocateHeap_t   RtlAllocateHeap;
extern RtlFreeHeap_t       RtlFreeHeap;
extern RtlReAllocateHeap_t RtlReAllocateHeap;

extern LdrGetDllHandle_t LdrGetDllHandle;
extern LdrGetProcedureAddress_t LdrGetProcedureAddress;
extern LdrUnloadDll_t LdrUnloadDll;
extern LdrLockLoaderLock_t LdrLockLoaderLock;
extern LdrUnlockLoaderLock_t LdrUnlockLoaderLock;
