//=====================================================================
// Copyright (c) 2007-2014    Advanced Micro Devices, Inc. All rights reserved.
// Copyright (c) 2004-2006    ATI Technologies Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//=====================================================================

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <string>
#include <stdint.h>

namespace CMP
{

// Basic types.
typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::int32_t LONG;
typedef std::int32_t BOOL;
typedef size_t DWORD_PTR;
typedef unsigned int UINT;

}

typedef CMP::DWORD          CMP_DWORD;         ///< A 32-bit unsigned integer format.
typedef CMP::WORD           CMP_WORD;          ///< A 16-bit unsigned integer format.
typedef CMP::BYTE           CMP_BYTE;          ///< An 8-bit unsigned integer format.
typedef char                CMP_CHAR;          ///< An 8-bit signed   char    format.
typedef float               CMP_FLOAT;         ///< A 32-bit signed   float   format.
typedef short               CMP_HALF;          ///< A 16-bit Half format.
typedef long                CMP_LONG;
typedef CMP::BOOL           CMP_BOOL;          ///< A 32-bit integer boolean format.
typedef CMP::DWORD_PTR      CMP_DWORD_PTR;

#define BLOCK_SIZE_4X4      16
#define BLOCK_SIZE_4X4X4    64

typedef float CODECFLOAT;

using namespace std;

#define UNREFERENCED_PARAMETER(P)          (P)

//#define USE_DBGTRACE                                //  Show messages via Win Debug 
//#define BC7_DEBUG_TO_RESULTS_TXT                    //  Send debug info to a results text file
//#define DXT5_COMPDEBUGGER                           //  Remote connect data to Comp Debugger views
//#define BC6H_COMPDEBUGGER                           //  Remote connect data to Comp Debugger views
//#define BC7_COMPDEBUGGER                            //  Remote connect data to Comp Debugger views
//#define BC6H_NO_OPTIMIZE_ENDPOINTS                  //  Turn off BC6H optimization of endpoints - useful for debugging quantization and mode checking
//#define BC6H_DEBUG_TO_RESULTS_TXT                   //  Generates a Results.txt file on exe working directory; MultiThreading is turned off for debuging to text file
//#define BC6H_DECODE_DEBUG                           //  Enables debug info on decoder

#define ENABLE_MAKE_COMPATIBLE_API                    //  Byte<->Float to make all source and dest compatible
//#define USE_GT

// V2.4 / V2.5 features and changes
#define USE_OLD_SWIZZLE                            //  Remove swizzle flag and abide by CMP_Formats

// To Be enabled in future releases
// #define ARGB_32_SUPPORT                            // Enables 32bit Float channel formats
// #define SUPPORT_ETC_ALPHA                          // for ATC_RGB output enable A
// #define SHOW_PROCESS_MEMORY                        // display available CPU process memory
// #define USE_COMPUTE                                 // Enable compute code for compression

#endif // !COMMON_H
