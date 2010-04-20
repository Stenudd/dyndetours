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

//==================================================================
// Includes
//==================================================================
#include "dd_bytearray.h"
#include "dd_memutils.h"
#include "dd_asm.h"

//==================================================================
// Constructor
//==================================================================
CByteArray::CByteArray( void *pMem, int size )
{
	m_codeBytes = NULL;
	m_curIndex = 0;

	if( !pMem )
	{
		printf("Null area of memory passed in!");
		return;
	}

	m_totalSize = size;
	m_codeBytes = (unsigned char *)pMem;
	m_bExisting = true;
	SetMemPatchable(m_codeBytes, m_totalSize);
}

//==================================================================
// Constructor
//==================================================================
CByteArray::CByteArray( int size )
{
	// Allocate space
	m_codeBytes = (unsigned char *)malloc( size );

	if( m_codeBytes )
	{
		m_totalSize = size;

		// Fill with nops
		fill_nop(m_codeBytes, size);

		// Allow patching / execution
		SetMemPatchable(m_codeBytes, size);

		// Set index to 0
		m_curIndex = 0;

		// This was not an existing piece of memory.
		m_bExisting = false;
	}

	else
	{
		printf("Couldn't malloc space for CByteArray!\n");
		return;
	}

}

//==================================================================
// Destructor
//==================================================================
CByteArray::~CByteArray()
{
	if( !m_bExisting )
		// Free up space
		free(m_codeBytes);
}

//==================================================================
// Operator overload
//==================================================================
void CByteArray::operator +=(unsigned char b)
{
	// We can't overflow the array!
	if( m_curIndex + sizeof(unsigned char) > m_totalSize )
		return;

	m_codeBytes[m_curIndex] = b;
	m_curIndex += 1;
}

//==================================================================
// Operator overload
//==================================================================
void CByteArray::operator += (unsigned long l)
{
	// We can't overflow the array!
	if( (m_curIndex + sizeof(unsigned long)) > m_totalSize )
		return;

	*(void **)(&m_codeBytes[m_curIndex]) = (void *)l;
	m_curIndex += 4;
}

//==================================================================
// Copies bytes from src to this array.
//==================================================================
void CByteArray::CopyBytes(unsigned char *pSource, int i)
{
	if( !pSource )
	{
		printf("Invalid source passed in.\n");
	}

	// Copy the bytes
	copy_bytes(pSource, m_codeBytes, i);

	// Increment index by i.
	m_curIndex += i;

}

//==================================================================
// Injects a jump in the byte array
//==================================================================
void CByteArray::InjectJmp(unsigned char* pJmpTo)
{
	// Inject the jump.
	inject_jmp(m_codeBytes + m_curIndex, pJmpTo);

	// Increment index
	m_curIndex += 5; // E9 XX XX XX XX
}