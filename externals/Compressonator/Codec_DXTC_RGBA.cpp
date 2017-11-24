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
//
//  File Name:   Codec_DXTC.cpp
//  Description: implementation of the CCodec_DXTC class
//
//////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Codec_DXTC.h"
#include "CompressonatorXCodec.h"
//#include "dxtc_v11_compress.h"

#define ConstructColour(r, g, b)  (((r) << 11) | ((g) << 5) | (b))

/* 
Channel Bits
*/
#define RG 5
#define GG 6
#define BG 5

void CompressRGBBlock(CMP_BYTE rgbBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[2], CODECFLOAT* pfChannelWeights, bool slow, bool bDXT1UseAlpha, CMP_BYTE nDXT1AlphaThreshold)
{
    /*
    ARGB Channel indexes
    */

    int RC = 0, GC = 1, BC = 2;

    CMP_BYTE nEndpoints[2][3][2];
    CMP_BYTE nIndices[2][BLOCK_SIZE_4X4];
    
    bool m_b3DRefinement = false;
    CMP_BYTE m_nRefinementSteps = 0;
    bool m_bUseSSE2 = false;

    if(slow)
    {
        double fError3 = CompRGBBlock((CMP_DWORD*)rgbBlock, BLOCK_SIZE_4X4, RG, GG, BG, nEndpoints[0], nIndices[0], 3, m_bUseSSE2, m_b3DRefinement, m_nRefinementSteps, pfChannelWeights, bDXT1UseAlpha, nDXT1AlphaThreshold);
        double fError4 = (fError3 == 0.0) ? FLT_MAX : CompRGBBlock((CMP_DWORD*)rgbBlock, BLOCK_SIZE_4X4, RG, GG, BG, nEndpoints[1], nIndices[1], 4, m_bUseSSE2, m_b3DRefinement, m_nRefinementSteps, pfChannelWeights, bDXT1UseAlpha, nDXT1AlphaThreshold);
        
        unsigned int nMethod = (fError3 <= fError4) ? 0 : 1;
        unsigned int c0 = ConstructColour((nEndpoints[nMethod][RC][0] >> (8-RG)), (nEndpoints[nMethod][GC][0] >> (8-GG)), (nEndpoints[nMethod][BC][0] >> (8-BG)));
        unsigned int c1 = ConstructColour((nEndpoints[nMethod][RC][1] >> (8-RG)), (nEndpoints[nMethod][GC][1] >> (8-GG)), (nEndpoints[nMethod][BC][1] >> (8-BG)));
        if(nMethod == 1 && c0 <= c1 || nMethod == 0 && c0 > c1)
            compressedBlock[0] = c1 | (c0<<16);
        else
            compressedBlock[0] = c0 | (c1<<16);
        
        compressedBlock[1] = 0;
        for(int i=0; i<16; i++)
            compressedBlock[1] |= (nIndices[nMethod][i] << (2*i));
    }
    else
    {
        CMP_BYTE nEndpoints[3][2];
        CMP_BYTE nIndices[BLOCK_SIZE_4X4];
        
        CompRGBBlock((CMP_DWORD*)rgbBlock, BLOCK_SIZE_4X4, RG, GG, BG, nEndpoints, nIndices, 4, m_bUseSSE2, m_b3DRefinement, m_nRefinementSteps, pfChannelWeights, bDXT1UseAlpha, nDXT1AlphaThreshold);
        
        unsigned int c0 = ConstructColour((nEndpoints[RC][0] >> (8-RG)), (nEndpoints[GC][0] >> (8-GG)), (nEndpoints[BC][0] >> (8-BG)));
        unsigned int c1 = ConstructColour((nEndpoints[RC][1] >> (8-RG)), (nEndpoints[GC][1] >> (8-GG)), (nEndpoints[BC][1] >> (8-BG)));
        if(c0 <= c1)
            compressedBlock[0] = c1 | (c0<<16);
        else
            compressedBlock[0] = c0 | (c1<<16);
        
        compressedBlock[1] = 0;
        for(int i=0; i<16; i++)
            compressedBlock[1] |= (nIndices[i] << (2*i));
    }
}

CODECFLOAT m_fBaseChannelWeights[3] = { 0.3086f, 0.6094f, 0.0820f };
CODECFLOAT m_fChannelWeights[3];

CODECFLOAT* CalculateColourWeightings(CMP_BYTE block[BLOCK_SIZE_4X4X4])
{
    float medianR = 0.0f, medianG = 0.0f, medianB = 0.0f;
    
    for(CMP_DWORD k=0; k<BLOCK_SIZE_4X4; k++)
    {
        CMP_DWORD R = (block[k] & 0xff0000) >> 16;
        CMP_DWORD G = (block[k] & 0xff00) >> 8;
        CMP_DWORD B = block[k] & 0xff;
        
        medianR += R;
        medianG += G;
        medianB += B;
    }
    
    medianR /= BLOCK_SIZE_4X4;
    medianG /= BLOCK_SIZE_4X4;
    medianB /= BLOCK_SIZE_4X4;
    
    // Now skew the colour weightings based on the gravity center of the block
    float largest = max(max(medianR, medianG), medianB);
    
    if(largest > 0)
    {
        medianR /= largest;
        medianG /= largest;
        medianB /= largest;
    }
    else
        medianR = medianG = medianB = 1.0f;
    
    // Scale weightings back up to 1.0f
    CODECFLOAT fWeightScale = 1.0f / (m_fBaseChannelWeights[0] + m_fBaseChannelWeights[1] + m_fBaseChannelWeights[2]);
    m_fChannelWeights[0] = m_fBaseChannelWeights[0] * fWeightScale;
    m_fChannelWeights[1] = m_fBaseChannelWeights[1] * fWeightScale;
    m_fChannelWeights[2] = m_fBaseChannelWeights[2] * fWeightScale;
    m_fChannelWeights[0] = ((m_fChannelWeights[0] * 3 * medianR) + m_fChannelWeights[0]) * 0.25f;
    m_fChannelWeights[1] = ((m_fChannelWeights[1] * 3 * medianG) + m_fChannelWeights[1]) * 0.25f;
    m_fChannelWeights[2] = ((m_fChannelWeights[2] * 3 * medianB) + m_fChannelWeights[2]) * 0.25f;
    fWeightScale = 1.0f / (m_fChannelWeights[0] + m_fChannelWeights[1] + m_fChannelWeights[2]);
    m_fChannelWeights[0] *= fWeightScale;
    m_fChannelWeights[1] *= fWeightScale;
    m_fChannelWeights[2] *= fWeightScale;
    
    return m_fChannelWeights;
}
