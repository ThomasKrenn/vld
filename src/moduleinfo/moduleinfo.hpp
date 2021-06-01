/***********************************************************************
* Visual Leak Detector - Loaded Modules State
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
* https://www.gnu.org/licenses
*
***********************************************************************/

#pragma once

#include "./stdafx.h"

#define VLDBUILD      // Declares that we are building Visual Leak Detector.

#include "./vldallocator.h"
#include "./utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <string>

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, vldallocator<wchar_t>> vldstring;

typedef enum tag_module_state {
   MOD_UNLOADED  = 0,
   MOD_NEWMODULE = 1,
   MOD_PATCHED   = 2,
   MOD_REATTACH  = 3      // unloaded -> reattaching
} MODULE_STATE;

// This structure stores information, primarily the virtual address range, about
// a given module and can be used with the Set template because it supports the
// '<' operator (sorts by virtual address range).
class moduleinfo_t
{
 public:
   moduleinfo_t()
      : _addrLow(0), _addrHigh(0), _flags(0)
   {}
   ~moduleinfo_t()
   {}

   BOOL operator<(const class moduleinfo_t &other) const
   {
      if (_addrHigh < other._addrLow) {
         return TRUE;
      } else {
         return FALSE;
      }
   }

   BOOL operator==(const class moduleinfo_t &other) const
   {
      if (_addrLow != other._addrLow
          || _addrHigh != other._addrHigh
          || _flags != other._flags
          || name != other.name) {
         return FALSE;
      } else {
         return TRUE;
      }
   }

#define VLD_MODULE_EXCLUDED 0x1      //   If set, this module is excluded from leak detection.
#define VLD_MODULE_SYMBOLSLOADED 0x2      //   If set, this module's debug symbols have been loaded.
   vldstring name;      // The module's name (e.g. "kernel32.dll").
   vldstring path;      // The fully qualified path from where the module was loaded.

   SIZE_T addrLow() const
   {
      return _addrLow;
   }
   SIZE_T addrHigh() const
   {
      return _addrHigh;
   }
   UINT32 flags() const
   {
      return _flags;
   }

   DWORD modulesize() const
   {
      return (DWORD)(_addrHigh - _addrLow) + 1;
   }

   void setAddrLow(SIZE_T addrLow)
   {
      _addrLow = addrLow;
   }

   void setAddrHigh(SIZE_T addrHigh)
   {
      _addrHigh = addrHigh;
   }

   void setFlags(UINT32 flags)
   {
      _flags = flags;
   }

   void setAddr(SIZE_T addrLow, SIZE_T addrHigh)
   {
      _addrLow  = addrLow;
      _addrHigh = addrHigh - 1;
   }

   void reportState()
   {
      if (_flags & VLD_MODULE_EXCLUDED) {
         Report(L"VLD: %-30s | Module excluded\n", name.c_str());
      } else if (_flags & VLD_MODULE_SYMBOLSLOADED) {      // || (moduleimageinfo.SymType == SymExport))
         Report(L"VLD: %-30s | Symbols loaded.\n", name.c_str());
      } else {
         Report(L"VLD: %-30s | No Symbols\n", name.c_str());
      }
   }

 private:
   SIZE_T _addrLow;       // Lowest address within the module's virtual address space (i.e. base address).
   SIZE_T _addrHigh;      // Highest address within the module's virtual address space (i.e. base + size).
   UINT32 _flags;         // Module flags:
};

BOOL IsModulePatched(HMODULE importmodule, moduleentry_t patchtable[], UINT tablesize);
BOOL FindImport(HMODULE importmodule, HMODULE exportmodule, LPCSTR exportmodulename, LPCSTR importname);
BOOL FindPatch(HMODULE importmodule, moduleentry_t *module);
