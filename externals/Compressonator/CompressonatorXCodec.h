//===============================================================================
// Copyright (c) 2007-2016  Advanced Micro Devices, Inc. All rights reserved.
// Copyright (c) 2004-2006 ATI Technologies Inc.
//===============================================================================
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
//
//  File Name:   ATIXCodec.h
//  Description: performs the DXT-style block compression
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _ATIXCODEC_H_
#define _ATIXCODEC_H_

/*------------------------------------------------------------------------------------
void CompRGBBlock(CMP_DWORD* block_32,    [IN]  array of the 3 component color vectors (888)
                  CMP_DWORD block_dxtc[2],                [OUT] compressed data in DXT1 for mat
                  int _NbrClrs = BLOCK_SIZE_4X4,            [IN]  actuall number of elements in the array
                  bool _bUseSSE2 = true);                    [IN]  switch to SSE2 implementation
------------------------------------------------------------------------------------*/
CODECFLOAT CompRGBBlock(CMP_DWORD* block_32, 
                        CMP_WORD dwBlockSize, 
                        CMP_BYTE nRedBits, 
                        CMP_BYTE nGreenBits, 
                        CMP_BYTE nBlueBits, 
                        CMP_BYTE nEndpoints[3][2],
                        CMP_BYTE* pcIndices,
                        CMP_BYTE dwNumPoints,
                        bool _bUseSSE2,
                        bool b3DRefinement,
                        CMP_BYTE nRefinementSteps,
                        CODECFLOAT* _pfChannelWeights = NULL,
                        bool _bUseAlpha = false, 
                        CMP_BYTE _nAlphaThreshold = 128);

#endif
