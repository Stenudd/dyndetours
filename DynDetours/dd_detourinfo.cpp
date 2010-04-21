/**
* ======================================================
* DynDetours
* Copyright (C) 2009 Deniz Sezen
* All rights reserved.
* ======================================================
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from 
* the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, 
* including commercial applications, and to alter it and redistribute it 
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not 
* claim that you wrote the original software. If you use this software in a 
* product, an acknowledgment in the product documentation would be 
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*
*/

//========================================================================
// Includes
//========================================================================
#include "dd_detourinfo.h"
#include <stdio.h>
#include <string.h>

//========================================================================
// Constructor
//========================================================================
CDetourInfo::CDetourInfo( char* szParams, eCallingConv conv )
{
	if( !szParams )
	{
		m_szParams = NULL;
		m_iOffsets = NULL;
		return;
	}

	SetParams( szParams );

	// Store calling convention
	m_eConv = conv;
}

//========================================================================
// Sets the parameters of the detour.
//========================================================================
void CDetourInfo::SetParams( char* szParams )
{
	// Allocate space
	m_szParams = new char[strlen(szParams) + 1];

	// Copy the param format string
	strcpy_s(m_szParams, strlen(szParams) + 1, szParams);
	
	// Calculate the number of parameters
	m_nNumParams = strlen(szParams) - 2;

	// Compute stack offsets
	ComputeStackOffsets();

	printf("Number of parameters: %d\n", m_nNumParams);
}

//========================================================================
// Destructor
//========================================================================
CDetourInfo::~CDetourInfo()
{
	// Free up memory
	if( m_szParams )
		delete m_szParams;

	if( m_iOffsets )
		delete m_iOffsets;
}

//========================================================================
// Computes stack offsets
//========================================================================
void CDetourInfo::ComputeStackOffsets()
{
	// Allocate space for offset array
	m_iOffsets = new int[m_nNumParams];

	// Loop through each parameter.
	char *ch = m_szParams;

	int curOffset = 0;
	int offsetTotal = 0; // Setting this to 4 accounts for the return address.

	while( *ch != '\0' && *ch != ')' )
	{
		switch( *ch )
		{
			case 'i':
			case 'p':
			case 'f':
			case 'b':
			case 'S':
				offsetTotal += sizeof(int);
				m_iOffsets[curOffset] = offsetTotal;
				break;
			case 'c':
				offsetTotal += sizeof(char);
				m_iOffsets[curOffset] = offsetTotal;
				break;
		}

		printf("m_iOffsets[%d]: %d\n", curOffset, m_iOffsets[curOffset]);

		// Go to the next offset
		curOffset++;
		ch++;
	}
}
