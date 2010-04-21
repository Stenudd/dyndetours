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

#ifndef _DD_DETOURMAN_H
#define _DD_DETOURMAN_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"
#include "dd_detour.h"
#include <vector>

//========================================================================
// Namespaces we need.
//========================================================================
using namespace std;

#if 0
//========================================================================
// Simple detour tracking struct.
//========================================================================
struct Detour_t
{
	CDetour* pDetour;
	void*	 pTargetAddr;
};
#endif

//========================================================================
// Useful type definitions.
//========================================================================
typedef vector<CDetour *>::iterator LIST_ITER;

//========================================================================
// Detour Manager class
//========================================================================
class CDetourManager
{
	private:
		/* This vector will store a list of detours and their addresses. */
		vector<CDetour *> m_DetourList;

	public:

		/* @brief Destructor. Removes all detours and frees up memory
		 *	taken up by them.
		 */
		~CDetourManager( void );

		/* @brief Adds a detour to our internal list of detours.
		 * @param pTarget  - Pointer to a callback function.
		 * @param szParams - Parameter format string.
		 * @param conv     - Calling convention of hooked function.
		 * @return A detour instance for the target address.
		 */
		CDetour* Add_Detour( void* pTarget, char* szParams, eCallingConv conv );

		/* @brief Removes a detour at the target address.
		 * @param pTarget - The target address the detour is bound to.
		 * @return True if the detour was found and removed successfully.
		 */
		bool     Remove_Detour( void* pTarget );

		/* @brief Finds a detour in our list.
		 * @param pTarget - The address that the detour has hooked.
		 * @return A CDetour pointer that is tied to the target function.
		 */
		CDetour* Find_Detour( void* pTarget );
};

//========================================================================
// DetourManager singleton.
//========================================================================
extern CDetourManager* g_pDetourManager;

#endif // _DD_DETOURMAN_H