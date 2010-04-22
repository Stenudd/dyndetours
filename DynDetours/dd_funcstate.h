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

#ifndef _DD_FUNCSTATE_H
#define _DD_FUNCSTATE_H

//========================================================================
// Forward declarations.
//========================================================================
class CDetour;

//========================================================================
// This class is for function state information.
//========================================================================
class CFuncState
{
	// We want CDetour to be able to access our stuff.
	friend class CDetour;

	private:
		unsigned long m_ulOrigESP; // Stack pointer.
		unsigned long m_ulOrigECX; // Accumulator.
		unsigned long m_ulOrigRET; // Return address (ebp+4).
		void*		  m_pRetVAL;   // What to store as the return value.

	public:

		/* Accessor Functions. */
		unsigned long GetESP( void ) { return m_ulOrigESP; }
		unsigned long GetECX( void ) { return m_ulOrigECX; }
		unsigned long GetRET( void ) { return m_ulOrigRET; }
	
		void SetRetVal( void* retVal ) { m_pRetVAL = retVal; } 
};

#endif _DD_FUNCSTATE_H