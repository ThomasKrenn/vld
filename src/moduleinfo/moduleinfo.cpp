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
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include "moduleinfo.hpp"

extern ImageDirectoryEntries g_Ide;

// FindOriginalImportDescriptor - Determines if the specified module imports the named import
//   from the named exporting module.
//
//  - importmodule (IN): Handle (base address) of the module to be searched to
//      see if it imports the specified import.
//
//  - exportmodulename (IN): ANSI string containing the name of the module that
//      exports the import to be searched for.
//
//  Return Value:
//
//    Returns pointer to descriptor.
//
IMAGE_IMPORT_DESCRIPTOR *FindOriginalImportDescriptor(HMODULE importmodule, LPCSTR exportmodulename)
{
   IMAGE_IMPORT_DESCRIPTOR *idte = NULL;
   IMAGE_SECTION_HEADER *section = NULL;
   ULONG size                    = 0;

   // Locate the importing module's Import Directory Table (IDT) entry for the
   // exporting module. The importing module actually can have several IATs --
   // one for each export module that it imports something from. The IDT entry
   // gives us the offset of the IAT for the module we are interested in.
   {
      idte = (IMAGE_IMPORT_DESCRIPTOR *)g_Ide.ImageDirectoryEntryToDataEx((PVOID)GetCallingModule((UINT_PTR)importmodule), TRUE,
         IMAGE_DIRECTORY_ENTRY_IMPORT, &size, &section);
   }

   if (idte == NULL) {
      // This module has no IDT (i.e. it imports nothing).
      return NULL;
   }
   while (idte->OriginalFirstThunk != 0x0) {
      PCHAR name = (PCHAR)R2VA(importmodule, idte->Name);
      if (_stricmp(name, exportmodulename) == 0) {
         // Found the IDT entry for the exporting module.
         break;
      }
      idte++;
   }
   if (idte->OriginalFirstThunk == 0x0) {
      // The importing module does not import anything from the exporting
      // module.
      return NULL;
   }
   return idte;
}

// FindPatch - Determines if the specified module has been patched to use the
//   specified replacement.
//
//  - importmodule (IN): Handle (base address) of the module to be searched to
//      see if it imports the specified replacement export.
//
//  - exportmodulename (IN): ANSI string containing the name of the module that
//      normally exports that import that would have been patched to use the
//      replacement export.
//
//  - replacement (IN): Address of the replacement, or destination, function or
//      variable to search for.
//
//  Return Value:
//
//    Returns TRUE if the module has been patched to use the specified
//    replacement export.
//
BOOL FindPatch(HMODULE importmodule, moduleentry_t *module)
{
   IMAGE_IMPORT_DESCRIPTOR *idte;

   idte = FindOriginalImportDescriptor(importmodule, module->exportModuleName);
   if (idte == NULL)
      return FALSE;

   int i               = 0;
   patchentry_t *entry = module->patchTable;
   while (entry->importName) {
      LPCVOID replacement = entry->replacement;

      IMAGE_THUNK_DATA *iate;

      // Locate the replacement's IAT entry.
      iate = (IMAGE_THUNK_DATA *)R2VA(importmodule, idte->FirstThunk);
      while (iate->u1.Function != 0x0) {
         if (iate->u1.Function == (DWORD_PTR)replacement) {
            // Found the IAT entry for the replacement. This patch has been
            // installed.
            return TRUE;
         }
         iate++;
      }
      entry++;
      i++;
   }

   // The module does not import the replacement. The patch has not been
   // installed.
   return FALSE;
}

// IsModulePatched - Checks to see if any of the imports listed in the specified
//   patch table have been patched into the specified importmodule.
//
//  - importmodule (IN): Handle (base address) of the module to be queried to
//      determine if it has been patched.
//
//  - patchtable (IN): An array of patchentry_t structures specifying all of the
//      import patches to search for.
//
//  - tablesize (IN): Size, in entries, of the specified patch table.
//
//  Return Value:
//
//    Returns TRUE if at least one of the patches listed in the patch table is
//    installed in the importmodule. Otherwise returns FALSE.
//
BOOL IsModulePatched(HMODULE importmodule, moduleentry_t patchtable[], UINT tablesize)
{
   // Loop through the import patch table, individually checking each patch
   // entry to see if it is installed in the import module. If any patch entry
   // is installed in the import module, then the module has been patched.
   BOOL found = FALSE;
   for (UINT index = 0; index < tablesize; index++) {
      moduleentry_t *entry = &patchtable[index];
      found                = FindPatch(importmodule, entry);
      if (found) {
         // Found one of the listed patches installed in the import module.
         return TRUE;
      }
   }

   // No patches listed in the patch table were found in the import module.
   return FALSE;
}

// FindImport - Determines if the specified module imports the named import
//   from the named exporting module.
//
//  - importmodule (IN): Handle (base address) of the module to be searched to
//      see if it imports the specified import.
//
//  - exportmodule (IN): Handle (base address) of the module that exports the
//      import to be searched for.
//
//  - exportmodulename (IN): ANSI string containing the name of the module that
//      exports the import to be searched for.
//
//  - importname (IN): ANSI string containing the name of the import to search
//      for. May be an integer cast to a string if the import is exported by
//      ordinal.
//
//  Return Value:
//
//    Returns TRUE if the module imports to the specified import. Otherwise
//    returns FALSE.
//
BOOL FindImport(HMODULE importmodule, HMODULE exportmodule, LPCSTR exportmodulename, LPCSTR importname)
{
   IMAGE_IMPORT_DESCRIPTOR *idte;
   IMAGE_THUNK_DATA *iate;
   FARPROC import;

   DbgTrace(L"dbghelp32.dll %i: FindImport - ImageDirectoryEntryToDataEx\n", GetCurrentThreadId());
   idte = FindOriginalImportDescriptor(importmodule, exportmodulename);
   if (idte == NULL)
      return FALSE;

   // Get the *real* address of the import. If we find this address in the IAT,
   // then we've found that the module does import the named import.
   import = GetProcAddress(exportmodule, importname);

   // Perhaps the named export module does not actually export the named import?
   //assert(import != NULL);   - on my Windows 7 x64, VS 9 SP1, Win x32 project assertion failure will cause new DLL loading, and then infinite loop of calls to findimport() and stack overflow. Maybe it's caused by antivirus, nevertheless this solution fixes infinite loop
   if (import == NULL)      // - instead of assert
   {
      OutputDebugStringW(__FUNCTIONW__ L"(" __FILEW__ L") : import == NULL\n");
      if (::IsDebuggerPresent())
         __debugbreak();

      return FALSE;
   }

   // Locate the import's IAT entry.
   iate = (IMAGE_THUNK_DATA *)R2VA(importmodule, idte->FirstThunk);
   while (iate->u1.Function != 0x0) {
      if (iate->u1.Function == (DWORD_PTR)import) {
         // Found the IAT entry. The module imports the named import.
         return TRUE;
      }
      iate++;
   }

   // The module does not import the named import.
   return FALSE;
}
