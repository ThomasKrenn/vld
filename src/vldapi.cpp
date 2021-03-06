/***********************************************************************
* Visual Leak Detector - Exported APIs
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

#include "stdafx.h"

#define VLDBUILD          // Declares that we are building Visual Leak Detector.
#include "vldint.h"       // Provides access to the Visual Leak Detector internals.
#include "vldheap.h"      // Provides internal new and delete operators.

// Imported global variables.
extern VisualLeakDetector g_vld;

////////////////////////////////////////////////////////////////////////////////
//
//  Visual Leak Detector APIs - see vldapi.h for each function's details.
//

extern "C" {

__declspec(dllexport) bool VLDDisable()
{
   return g_vld.DisableLeakDetection();
}

__declspec(dllexport) void VLDEnable()
{
   g_vld.EnableLeakDetection();
}

__declspec(dllexport) void VLDRestore()
{
   g_vld.RestoreLeakDetectionState();
}

__declspec(dllexport) void VLDGlobalDisable()
{
   g_vld.GlobalDisableLeakDetection();
}

__declspec(dllexport) void VLDGlobalEnable()
{
   g_vld.GlobalEnableLeakDetection();
}

__declspec(dllexport) UINT VLDReportLeaks()
{
   return (UINT)g_vld.ReportLeaks();
}

__declspec(dllexport) UINT VLDReportThreadLeaks(UINT threadId)
{
   return (UINT)g_vld.ReportThreadLeaks(threadId);
}

__declspec(dllexport) UINT VLDGetLeaksCount()
{
   return (UINT)g_vld.GetLeaksCount();
}

__declspec(dllexport) UINT VLDGetThreadLeaksCount(UINT threadId)
{
   return (UINT)g_vld.GetThreadLeaksCount(threadId);
}

__declspec(dllexport) void VLDMarkAllLeaksAsReported()
{
   g_vld.MarkAllLeaksAsReported();
}

__declspec(dllexport) void VLDMarkThreadLeaksAsReported(UINT threadId)
{
   g_vld.MarkThreadLeaksAsReported(threadId);
}

__declspec(dllexport) void VLDRefreshModules()
{
   g_vld.RefreshModules();
}

__declspec(dllexport) void VLDEnableModule(HMODULE module)
{
   g_vld.EnableModule(module);
}

__declspec(dllexport) void VLDDisableModule(HMODULE module)
{
   g_vld.DisableModule(module);
}

__declspec(dllexport) UINT VLDGetOptions()
{
   return g_vld.GetOptions();
}

__declspec(dllexport) void VLDGetReportFilename(WCHAR *filename)
{
   g_vld.GetReportFilename(filename);
}

__declspec(dllexport) void VLDSetOptions(UINT option_mask, SIZE_T maxDataDump, UINT maxTraceFrames)
{
   g_vld.SetOptions(option_mask, maxDataDump, maxTraceFrames);
}

__declspec(dllexport) int VLDSetReportHook(int mode, VLD_REPORT_HOOK pfnNewHook)
{
   return g_vld.SetReportHook(mode, pfnNewHook);
}

__declspec(dllexport) void VLDSetModulesList(CONST WCHAR *modules, BOOL includeModules)
{
   g_vld.SetModulesList(modules, includeModules);
}

__declspec(dllexport) BOOL VLDGetModulesList(WCHAR *modules, UINT size)
{
   return g_vld.GetModulesList(modules, size);
}

__declspec(dllexport) void VLDSetReportOptions(UINT option_mask, CONST WCHAR *filename)
{
   g_vld.SetReportOptions(option_mask, filename);
}

__declspec(dllexport) int VLDResolveCallstacks()
{
   return g_vld.ResolveCallstacks();
}

/// Internal function for tests. Not safe to use because Vld own returned string
__declspec(dllexport) const wchar_t *VldInternalGetAllocationCallstack(void *alloc, BOOL showInternalFrames)
{
   return g_vld.GetAllocationResolveResults(alloc, showInternalFrames);
}
}
