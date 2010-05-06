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

	// Store calling convention
	m_eConv = conv;

	SetParams( szParams );
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
	m_nNumParams = 0;

	// NOTE: On windows, thiscalls do not push
	// the this pointer onto the stack.
	char* ch = m_szParams;

	// Loop through each character
	while( *ch != ')' && *ch != '\0' )
	{
		// Figure out what it is..
		switch(*ch)
		{
			// Void, don't increment.
			case 'v':
				break;

			// Otherwise, accept it.
			default:
				m_nNumParams++;
		}	

		ch++;
	}

	// Compute stack offsets
	ComputeStackOffsets();
}

//========================================================================
// Destructor
//========================================================================
CDetourInfo::~CDetourInfo()
{
	// Free up memory
	if( m_szParams )
	{
		delete m_szParams;
	}

	if( m_iOffsets )
	{
		delete m_iOffsets;
	}
}

//========================================================================
// Computes stack offsets
//========================================================================
void CDetourInfo::ComputeStackOffsets()
{
	// Skip this if no parameters
	if( m_nNumParams == 0 )
	{
		m_iOffsets = NULL;
		return;
	}

	// Allocate space for offset array
	m_iOffsets = new int[m_nNumParams];

	// NOTE: On windows, thiscalls do not push
	// the this pointer onto the stack. However,
	// the first char in a thiscall format string will
	// always be a 'p' signifying the thispointer.
	char* ch = m_szParams;

	int curOffset = 0;	 // The current slot in the offset array we're on.
	int offsetTotal = 0; // Setting this to 4 accounts for the return address.

	// Loop through each parameter.
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
