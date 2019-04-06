/** @file
  Copyright (C) 2018, vit9696. All rights reserved.

  All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Library/OcTemplateLib.h>
#include <Library/OcSerializeLib.h>
#include <Library/OcMiscLib.h>
#include <Library/OcAppleKernelLib.h>

#include <sys/time.h>

/*
 clang -g -fsanitize=undefined,address -Wno-incompatible-pointer-types-discards-qualifiers -I../Include -I../../Include -I../../../MdePkg/Include/ -I../../../EfiPkg/Include/ -include ../Include/Base.h Prelinked.c ../../Library/OcXmlLib/OcXmlLib.c ../../Library/OcTemplateLib/OcTemplateLib.c ../../Library/OcSerializeLib/OcSerializeLib.c ../../Library/OcMiscLib/Base64Decode.c ../../Library/OcStringLib/OcAsciiLib.c ../../Library/OcMachoLib/CxxSymbols.c ../../Library/OcMachoLib/Header.c ../../Library/OcMachoLib/Relocations.c ../../Library/OcMachoLib/Symbols.c ../../Library/OcAppleKernelLib/PrelinkedContext.c ../../Library/OcAppleKernelLib/PrelinkedKext.c ../../Library/OcAppleKernelLib/KextPatcher.c ../../Library/OcMiscLib/DataPatcher.c ../../Library/OcAppleKernelLib/Link.c ../../Library/OcAppleKernelLib/Vtables.c ../../Library/OcAppleKernelLib/KernelReader.c ../../Library/OcCompressionLib/lzss/lzss.c ../../Library/OcCompressionLib/lzvn/lzvn.c ../../Tests/KernelTest/Lilu.c ../../Tests/KernelTest/Vsmc.c -o Prelinked

 for fuzzing:
 clang-mp-7.0 -DFUZZING_TEST=1 -g -fsanitize=undefined,address,fuzzer -Wno-incompatible-pointer-types-discards-qualifiers -I../Include -I../../Include -I../../../MdePkg/Include/ -I../../../EfiPkg/Include/ -include ../Include/Base.h Prelinked.c ../../Library/OcXmlLib/OcXmlLib.c ../../Library/OcTemplateLib/OcTemplateLib.c ../../Library/OcSerializeLib/OcSerializeLib.c ../../Library/OcMiscLib/Base64Decode.c ../../Library/OcStringLib/OcAsciiLib.c ../../Library/OcMachoLib/CxxSymbols.c ../../Library/OcMachoLib/Header.c ../../Library/OcMachoLib/Relocations.c ../../Library/OcMachoLib/Symbols.c ../../Library/OcAppleKernelLib/PrelinkedContext.c ../../Library/OcAppleKernelLib/PrelinkedKext.c ../../Library/OcAppleKernelLib/KextPatcher.c ../../Library/OcMiscLib/DataPatcher.c ../../Library/OcAppleKernelLib/Link.c ../../Library/OcAppleKernelLib/Vtables.c ../../Library/OcAppleKernelLib/KernelReader.c ../../Library/OcCompressionLib/lzss/lzss.c ../../Library/OcCompressionLib/lzvn/lzvn.c ../../Tests/KernelTest/Lilu.c ../../Tests/KernelTest/Vsmc.c -o Prelinked
 rm -rf DICT fuzz*.log ; mkdir DICT ; find /System/Library/Extensions/<< * >>/Contents/MacOS -type f -exec cp {} DICT \; UBSAN_OPTIONS='halt_on_error=1' ./Prelinked -jobs=4 DICT -rss_limit_mb=4096

 rm -rf Prelinked.dSYM DICT fuzz*.log Prelinked

 clang -DTEST_SLE=1 -g -O3 -fno-sanitize=undefined,address -Wno-incompatible-pointer-types-discards-qualifiers -I../Include -I../../Include -I../../../MdePkg/Include/ -I../../../EfiPkg/Include/ -include ../Include/Base.h Prelinked.c ../../Library/OcXmlLib/OcXmlLib.c ../../Library/OcTemplateLib/OcTemplateLib.c ../../Library/OcSerializeLib/OcSerializeLib.c ../../Library/OcMiscLib/Base64Decode.c ../../Library/OcStringLib/OcAsciiLib.c ../../Library/OcMachoLib/CxxSymbols.c ../../Library/OcMachoLib/Header.c ../../Library/OcMachoLib/Relocations.c ../../Library/OcMachoLib/Symbols.c ../../Library/OcAppleKernelLib/PrelinkedContext.c ../../Library/OcAppleKernelLib/PrelinkedKext.c ../../Library/OcAppleKernelLib/KextPatcher.c ../../Library/OcMiscLib/DataPatcher.c ../../Library/OcAppleKernelLib/Link.c ../../Library/OcAppleKernelLib/Vtables.c ../../Library/OcAppleKernelLib/KernelReader.c ../../Library/OcCompressionLib/lzss/lzss.c ../../Library/OcCompressionLib/lzvn/lzvn.c ../../Tests/KernelTest/Lilu.c ../../Tests/KernelTest/Vsmc.c  -o Prelinked

 for i in /System/Library/Extensions/<< * >>.kext ; do plist=$i/Contents/Info.plist ; kext="$i/Contents/MacOS/$(/usr/libexec/PlistBuddy -c 'Print CFBundleExecutable' "$plist")" ; echo "$kext $plist" ; ./Prelinked prelinkedkernel.unpack "$kext" "$plist" ; done

 /[^\n]+\nPassed.kext injected - 0x8[^\n]+
*/

STATIC CHAR8 KextInfoPlistData[] = {
  0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20, 0x76, 0x65,
  0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31,
  0x2E, 0x30, 0x22, 0x20, 0x65, 0x6E, 0x63, 0x6F,
  0x64, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x55, 0x54,
  0x46, 0x2D, 0x38, 0x22, 0x3F, 0x3E, 0x0D, 0x3C,
  0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45,
  0x20, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x20, 0x50,
  0x55, 0x42, 0x4C, 0x49, 0x43, 0x20, 0x22, 0x2D,
  0x2F, 0x2F, 0x41, 0x70, 0x70, 0x6C, 0x65, 0x2F,
  0x2F, 0x44, 0x54, 0x44, 0x20, 0x50, 0x4C, 0x49,
  0x53, 0x54, 0x20, 0x31, 0x2E, 0x30, 0x2F, 0x2F,
  0x45, 0x4E, 0x22, 0x20, 0x22, 0x68, 0x74, 0x74,
  0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E,
  0x61, 0x70, 0x70, 0x6C, 0x65, 0x2E, 0x63, 0x6F,
  0x6D, 0x2F, 0x44, 0x54, 0x44, 0x73, 0x2F, 0x50,
  0x72, 0x6F, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4C,
  0x69, 0x73, 0x74, 0x2D, 0x31, 0x2E, 0x30, 0x2E,
  0x64, 0x74, 0x64, 0x22, 0x3E, 0x0D, 0x3C, 0x70,
  0x6C, 0x69, 0x73, 0x74, 0x20, 0x76, 0x65, 0x72,
  0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E,
  0x30, 0x22, 0x3E, 0x0D, 0x3C, 0x64, 0x69, 0x63,
  0x74, 0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79,
  0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C,
  0x65, 0x49, 0x64, 0x65, 0x6E, 0x74, 0x69, 0x66,
  0x69, 0x65, 0x72, 0x3C, 0x2F, 0x6B, 0x65, 0x79,
  0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72, 0x69,
  0x6E, 0x67, 0x3E, 0x61, 0x73, 0x2E, 0x76, 0x69,
  0x74, 0x39, 0x36, 0x39, 0x36, 0x2E, 0x54, 0x65,
  0x73, 0x74, 0x44, 0x72, 0x69, 0x76, 0x65, 0x72,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x49, 0x6E, 0x66, 0x6F, 0x44, 0x69, 0x63, 0x74,
  0x69, 0x6F, 0x6E, 0x61, 0x72, 0x79, 0x56, 0x65,
  0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74,
  0x72, 0x69, 0x6E, 0x67, 0x3E, 0x36, 0x2E, 0x30,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x4E, 0x61, 0x6D, 0x65, 0x3C, 0x2F, 0x6B, 0x65,
  0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x43, 0x50, 0x55, 0x46,
  0x72, 0x69, 0x65, 0x6E, 0x64, 0x44, 0x61, 0x74,
  0x61, 0x50, 0x72, 0x6F, 0x76, 0x69, 0x64, 0x65,
  0x72, 0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79,
  0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C,
  0x65, 0x50, 0x61, 0x63, 0x6B, 0x61, 0x67, 0x65,
  0x54, 0x79, 0x70, 0x65, 0x3C, 0x2F, 0x6B, 0x65,
  0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x4B, 0x45, 0x58, 0x54,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x53, 0x68, 0x6F, 0x72, 0x74, 0x56, 0x65, 0x72,
  0x73, 0x69, 0x6F, 0x6E, 0x53, 0x74, 0x72, 0x69,
  0x6E, 0x67, 0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E,
  0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x31, 0x2E, 0x30, 0x2E, 0x30, 0x3C,
  0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x43,
  0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65, 0x56,
  0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3C, 0x2F,
  0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x31, 0x2E,
  0x30, 0x2E, 0x30, 0x3C, 0x2F, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x49, 0x4F, 0x4B, 0x69, 0x74,
  0x50, 0x65, 0x72, 0x73, 0x6F, 0x6E, 0x61, 0x6C,
  0x69, 0x74, 0x69, 0x65, 0x73, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x64, 0x69,
  0x63, 0x74, 0x3E, 0x0D, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x54, 0x65, 0x73, 0x74, 0x44,
  0x61, 0x74, 0x61, 0x50, 0x72, 0x6F, 0x76, 0x69,
  0x64, 0x65, 0x72, 0x3C, 0x2F, 0x6B, 0x65, 0x79,
  0x3E, 0x0D, 0x09, 0x09, 0x3C, 0x64, 0x69, 0x63,
  0x74, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E,
  0x64, 0x6C, 0x65, 0x49, 0x64, 0x65, 0x6E, 0x74,
  0x69, 0x66, 0x69, 0x65, 0x72, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x63,
  0x6F, 0x6D, 0x2E, 0x61, 0x70, 0x70, 0x6C, 0x65,
  0x2E, 0x64, 0x72, 0x69, 0x76, 0x65, 0x72, 0x2E,
  0x41, 0x70, 0x70, 0x6C, 0x65, 0x41, 0x43, 0x50,
  0x49, 0x50, 0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72,
  0x6D, 0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x49, 0x4F, 0x43, 0x6C, 0x61,
  0x73, 0x73, 0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E,
  0x0D, 0x09, 0x09, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x41, 0x70, 0x70, 0x6C,
  0x65, 0x41, 0x43, 0x50, 0x49, 0x43, 0x50, 0x55,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65,
  0x79, 0x3E, 0x49, 0x4F, 0x4E, 0x61, 0x6D, 0x65,
  0x4D, 0x61, 0x74, 0x63, 0x68, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x70,
  0x72, 0x6F, 0x63, 0x65, 0x73, 0x73, 0x6F, 0x72,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65,
  0x79, 0x3E, 0x49, 0x4F, 0x50, 0x72, 0x6F, 0x62,
  0x65, 0x53, 0x63, 0x6F, 0x72, 0x65, 0x3C, 0x2F,
  0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09,
  0x3C, 0x69, 0x6E, 0x74, 0x65, 0x67, 0x65, 0x72,
  0x3E, 0x31, 0x31, 0x30, 0x30, 0x3C, 0x2F, 0x69,
  0x6E, 0x74, 0x65, 0x67, 0x65, 0x72, 0x3E, 0x0D,
  0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x49, 0x4F, 0x50, 0x72, 0x6F, 0x76, 0x69, 0x64,
  0x65, 0x72, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09,
  0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x49, 0x4F, 0x41, 0x43, 0x50, 0x49, 0x50,
  0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72, 0x6D, 0x44,
  0x65, 0x76, 0x69, 0x63, 0x65, 0x3C, 0x2F, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09,
  0x09, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x4F,
  0x70, 0x65, 0x6E, 0x43, 0x6F, 0x72, 0x65, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09,
  0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57,
  0x6F, 0x72, 0x6C, 0x64, 0x21, 0x3C, 0x2F, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09,
  0x09, 0x3C, 0x2F, 0x64, 0x69, 0x63, 0x74, 0x3E,
  0x0D, 0x09, 0x3C, 0x2F, 0x64, 0x69, 0x63, 0x74,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x4E, 0x53, 0x48, 0x75, 0x6D, 0x61, 0x6E, 0x52,
  0x65, 0x61, 0x64, 0x61, 0x62, 0x6C, 0x65, 0x43,
  0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74,
  0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09,
  0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68,
  0x74, 0x20, 0xC2, 0xA9, 0x20, 0x32, 0x30, 0x31,
  0x39, 0x20, 0x76, 0x69, 0x74, 0x39, 0x36, 0x39,
  0x36, 0x2E, 0x20, 0x41, 0x6C, 0x6C, 0x20, 0x72,
  0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 0x72, 0x65,
  0x73, 0x65, 0x72, 0x76, 0x65, 0x64, 0x2E, 0x3C,
  0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x4F,
  0x53, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65, 0x52,
  0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x64, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x52,
  0x6F, 0x6F, 0x74, 0x3C, 0x2F, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x3C, 0x2F, 0x64,
  0x69, 0x63, 0x74, 0x3E, 0x0D, 0x3C, 0x2F, 0x70,
  0x6C, 0x69, 0x73, 0x74, 0x3E
};

extern UINT8 LiluKextData[];
extern UINT32 LiluKextDataSize;
extern CHAR8 LiluKextInfoPlistData[];
extern UINT32 LiluKextInfoPlistDataSize;
extern UINT8 VsmcKextData[];
extern UINT32 VsmcKextDataSize;
extern CHAR8 VsmcKextInfoPlistData[];
extern UINT32 VsmcKextInfoPlistDataSize;

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

uint8_t *readFile(const char *str, uint32_t *size) {
  FILE *f = fopen(str, "rb");

  if (!f) return NULL;

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  uint8_t *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;
  *size = fsize;

  return string;
}

STATIC
UINT8
IOAHCIBlockStoragePatchFind[] = {
  0x41, 0x50, 0x50, 0x4C, 0x45, 0x20, 0x53, 0x53, 0x44, 0x00
};

STATIC
UINT8
IOAHCIBlockStoragePatchReplace[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

STATIC
PATCHER_GENERIC_PATCH
IOAHCIBlockStoragePatch = {
  .Base    = NULL, // Symbolic patch
  .Find    = IOAHCIBlockStoragePatchFind,
  .Mask    = NULL,
  .Replace = IOAHCIBlockStoragePatchReplace,
  .ReplaceMask = NULL,
  .Size    = sizeof (IOAHCIBlockStoragePatchFind),
  .Count   = 1,
  .Skip    = 0
};

STATIC
UINT8
IOAHCIPortPatchFind[] = {
  0x45, 0x78, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C
};

STATIC
UINT8
IOAHCIPortPatchReplace[] = {
  0x49, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C
};

STATIC
PATCHER_GENERIC_PATCH
IOAHCIPortPatch = {
  .Base    = NULL, // For symbolic patch
  .Find    = IOAHCIPortPatchFind,
  .Mask    = NULL,
  .Replace = IOAHCIPortPatchReplace,
  .ReplaceMask = NULL,
  .Size    = sizeof (IOAHCIPortPatchFind),
  .Count   = 1,
  .Skip    = 0
};

STATIC
UINT8
DisableAppleHDAPatchReplace[] = {
  0x31, 0xC0, 0xC3 // xor eax, eax ; ret
};

STATIC
PATCHER_GENERIC_PATCH
DisableAppleHDAPatch = {
  .Base    = "__ZN20AppleHDACodecGeneric5probeEP9IOServicePi",
  .Find    = NULL,
  .Mask    = NULL,
  .Replace = DisableAppleHDAPatchReplace,
  .ReplaceMask = NULL,
  .Size    = sizeof (DisableAppleHDAPatchReplace),
  .Count   = 1,
  .Skip    = 0
};

STATIC
UINT8
DisableKernelLog[] = {
  0xC3
};

STATIC
PATCHER_GENERIC_PATCH
KernelPatch = {
  .Base    = "_IOLog",
  .Find    = NULL,
  .Mask    = NULL,
  .Replace = DisableKernelLog,
  .Size    = sizeof (DisableKernelLog),
  .Count   = 1,
  .Skip    = 0
};

STATIC
VOID
ApplyKextPatches (
  PRELINKED_CONTEXT  *Context
  )
{
#if 0
  EFI_STATUS       Status;
  PATCHER_CONTEXT  Patcher;

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.iokit.IOAHCIBlockStorage"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &IOAHCIBlockStoragePatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.iokit.IOAHCIBlockStorage - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.iokit.IOAHCIBlockStorage\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.iokit.IOAHCIBlockStorage - %r\n", Status));
  }

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.driver.AppleAHCIPort"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &IOAHCIPortPatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.driver.AppleAHCIPort - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.driver.AppleAHCIPort\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.driver.AppleAHCIPort - %r\n", Status));
  }

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.driver.AppleHDA"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &DisableAppleHDAPatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.driver.AppleHDA - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.driver.AppleHDA\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.driver.AppleHDA - %r\n", Status));
  }

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.driver.AppleHDAController"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherBlockKext (&Patcher);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to block com.apple.driver.AppleHDAController - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Block success com.apple.driver.AppleHDAController\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.driver.AppleHDAController - %r\n", Status));
  }
#endif
}

VOID
ApplyKernelPatches (
  IN OUT UINT8   *Kernel,
  IN     UINT32  Size
  )
{
  EFI_STATUS       Status;
  PATCHER_CONTEXT  Patcher;

  Status = PatcherInitContextFromBuffer (
    &Patcher,
    Kernel,
    Size
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &KernelPatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch kernel - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success kernel\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find kernel - %r\n", Status));
  }
}

#ifdef FUZZING_TEST
#define main no_main
#endif

static EFI_FILE_PROTOCOL nilFilProtocol;

UINT8  *Prelinked;
UINT32 PrelinkedSize;

EFI_STATUS
GetFileData (
  IN  EFI_FILE_PROTOCOL  *File,
  IN  UINT32             Position,
  IN  UINT32             Size,
  OUT UINT8              *Buffer
  )
{
  ASSERT (File == &nilFilProtocol);

  if ((UINT64) Position + Size > PrelinkedSize) {
    return EFI_INVALID_PARAMETER;
  }

  memcpy (&Buffer[0], &Prelinked[Position], Size);
  return EFI_SUCCESS;
}

EFI_STATUS
GetFileSize (
  IN  EFI_FILE_PROTOCOL  *File,
  OUT UINT32             *Size
  )
{
  ASSERT (File == &nilFilProtocol);
  *Size = PrelinkedSize;
  return EFI_SUCCESS;
}

int wrap_main(int argc, char** argv) {
  UINT32 AllocSize;
  PRELINKED_CONTEXT Context;
  const char *name = argc > 1 ? argv[1] : "/System/Library/PrelinkedKernels/prelinkedkernel";
  if ((Prelinked = readFile(name, &PrelinkedSize)) == NULL) {
    printf("Read fail\n");
    return -1;
  }

  AllocSize = MACHO_ALIGN (PrelinkedSize + 1*1024*1024);

  if (PrelinkedSize > 4 && *(UINT32 *)Prelinked == 0xbebafeca) {
    UINT8 *NewPrelinked = NULL;
    UINT32 NewPrelinkedSize = PrelinkedSize;
    EFI_STATUS Status = ReadAppleKernel (
      &nilFilProtocol,
      &NewPrelinked,
      &NewPrelinkedSize,
      &AllocSize,
      5992448
      );

    if (!EFI_ERROR (Status)) {
      free(Prelinked);
      Prelinked = NewPrelinked;
      PrelinkedSize = NewPrelinkedSize;
    } else {
      printf("Unpack fail\n");
      return -1;
    }
  } else {
    Prelinked = realloc (Prelinked, AllocSize);
    if (Prelinked == NULL) {
      printf("Realloc fail\n");
      abort();
      return -1;
    }
  }  

#if 0
  ApplyKernelPatches (Prelinked, PrelinkedSize);
#endif

  EFI_STATUS Status = PrelinkedContextInit (&Context, Prelinked, PrelinkedSize, AllocSize);

  if (!EFI_ERROR (Status)) {
    ApplyKextPatches (&Context);

    Status = PrelinkedInjectPrepare (&Context);
    if (EFI_ERROR (Status)) {
      printf("Prelink inject prepare error %zx\n", Status);
    }

#ifndef TEST_SLE
    Status = PrelinkedInjectKext (
      &Context,
      "/Library/Extensions/TestDriver.kext",
      KextInfoPlistData,
      sizeof (KextInfoPlistData),
      NULL,
      NULL,
      0
      );

    DEBUG ((DEBUG_WARN, "TestDriver.kext injected - %zx\n", Status));
#endif

    int c = 0;

    while (argc > 2) {
      UINT8  *TestData = LiluKextData;
      UINT32 TestDataSize = LiluKextDataSize;
      CHAR8  *TestPlist = LiluKextInfoPlistData;
      UINT32 TestPlistSize = LiluKextInfoPlistDataSize;

      if (argc > 2) {
        TestData = readFile(argv[2], &TestDataSize);
        if (TestData == NULL) {
          printf("Read data fail\n");
          abort();
          return -1;
        }
      }

      if (argc > 3) {
        TestPlist = (CHAR8*) readFile(argv[3], &TestPlistSize);
        if (TestPlist == NULL) {
          printf("Read plist fail\n");
          abort();
          return -1;
        }
      }

      char KextPath[64];
      snprintf(KextPath, sizeof(KextPath), "/Library/Extensions/Kex%d.kext", c);

      Status = PrelinkedInjectKext (
        &Context,
        KextPath,
        TestPlist,
        TestPlistSize,
        "Contents/MacOS/Kext",
        TestData,
        TestDataSize
        );

      DEBUG ((DEBUG_WARN, "%s injected - %r\n", argc > 2 ? "Passed.kext" : "Lilu.kext", Status));

      if (argc > 2) free(TestData);
      if (argc > 3) free(TestPlist);

      argc -= 2;
      argv += 2;
      c++;
    }

#ifndef TEST_SLE
    if (argc <= 2) {
      Status = PrelinkedInjectKext (
        &Context,
        "/Library/Extensions/VirtualSMC.kext",
        VsmcKextInfoPlistData,
        VsmcKextInfoPlistDataSize,
        "Contents/MacOS/VirtualSMC",
        VsmcKextData,
        VsmcKextDataSize
        );

      DEBUG ((DEBUG_WARN, "VirtualSMC.kext injected - %r\n", Status));
    }

    Status = PrelinkedInjectComplete (&Context);

    if (EFI_ERROR (Status)) {
      printf("Prelink inject complete error %zx\n", Status);
    }

    FILE *Fh = fopen("out.bin", "wb");

    if (Fh != NULL) {
      fwrite (Prelinked, Context.PrelinkedSize, 1, Fh);
      fclose(Fh);

      if (!EFI_ERROR (Status)) {
        printf("All good\n");
      }
    } else {
      printf("File error\n");
    }
#endif
    PrelinkedContextFree (&Context);
  } else {
    printf("Context creation error %zx\n", Status);
  }

  free(Prelinked);

  return 0;
}

INT32 LLVMFuzzerTestOneInput(CONST UINT8 *Data, UINTN Size) {
  UINT32 PrelinkedSize;
  UINT32 AllocSize;
  UINT8  *Prelinked;
  PRELINKED_CONTEXT Context;

  if (Size == 0) {
    return 0;
  }

  if ((Prelinked = readFile("prelinkedkernel.unpack", &PrelinkedSize)) == NULL) {
    printf("Read fail\n");
    return 0;
  }

  AllocSize = MACHO_ALIGN (PrelinkedSize + 64*1024*1024);

  Prelinked = realloc (Prelinked, AllocSize);
  if (Prelinked == NULL) {
    return 0;
  }

  EFI_STATUS Status = PrelinkedContextInit (&Context, Prelinked, PrelinkedSize, AllocSize);

  if (EFI_ERROR (Status)) {
    free (Prelinked);
    return 0;
  }

  Status = PrelinkedInjectPrepare (&Context);
  if (EFI_ERROR (Status)) {
    printf("Prelink inject prepare error %zx\n", Status);
    PrelinkedContextFree (&Context);
    free (Prelinked);
    return 0;
  }

  Status = PrelinkedInjectKext (
      &Context,
      "/Library/Extensions/Lilu.kext",
      LiluKextInfoPlistData,
      LiluKextInfoPlistDataSize,
      "Contents/MacOS/Lilu",
      Data,
      Size
      );

  PrelinkedInjectComplete (&Context);
  PrelinkedContextFree (&Context);
  free(Prelinked);

  return 0;
}

int main(int argc, char *argv[]) {
  for (size_t i = 0; i < 1; i++) {
    wrap_main(argc, argv);
  }
  return 0;
}