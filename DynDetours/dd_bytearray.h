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
#ifndef _DD_BYTEARRAY_H
#define _DD_BYTEARRAY_H

//==================================================================
// Bytes array class.
//==================================================================
class CByteArray
{
private:
	unsigned char* m_codeBytes; // Pointer to where all the ASM bytes
	// will go.

	int			   m_curIndex;  // Keeps track of where the next byte
	// will go in the array.

	int			   m_totalSize;	// Total size m_codeBytes occupies.

	bool		   m_bExisting;	// If true, we don't free the space
	// pointed to by m_codeBytes because
	// we didn't allocate it.

public:
	// ====================================
	// Can make a blank byte
	// array...
	// ====================================
	CByteArray( int size );

	// ====================================
	// Or point one to an existing
	// blob of memory.
	// ====================================
	CByteArray( void *pMem, int size );

	~CByteArray( void );

	// ====================================
	// Overloaded operators.
	// ====================================
	void operator += (unsigned char);
	void operator += (unsigned long);

	// ====================================
	// Copies iSize bytes from pSource to 
	// wherever m_curIndex points to in
	// m_codeBytes. Auto increments the 
	// index.
	// ====================================
	void CopyBytes(unsigned char* pSource, int iSize);

	// ====================================
	// Injects a jump to pJmpTo to
	// wherever m_curIndex points to in
	// the array.
	// ====================================
	void InjectJmp(unsigned char* pJmpTo);

	// ====================================
	// Returns a pointer to the start
	// of the code stub.
	// ====================================
	void* GetArray( void ) { return (void *)m_codeBytes; }

	// ====================================
	// Returns wherever our indexer is
	// currently pointing to.
	// ====================================
	int	  GetIndex( void ) { return m_curIndex; }
};

#endif // _DD_BYTEARRAY_H