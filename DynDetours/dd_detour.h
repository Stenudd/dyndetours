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

#ifndef _DD_DETOUR_H
#define _DD_DETOUR_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h" /* Need detour definitions.            */
#include "dd_funcstate.h"	/* Need CFuncState class.              */
#include "dd_detourinfo.h"  /* Need CDetourInfo class.             */
#include "dd_callbackman.h" /* Need CCallBackManager class.        */
#include "dd_asmgen.h"		/* Needed for the CASMGenerator class. */

//========================================================================
// This is the detour class.
//========================================================================
class CDetour
{
	private:

		// Variables related to the hooked functions.
		unsigned char* m_pTarget;		/* Target function to hook. */
		unsigned char* m_pCallBack;		/* Function to callback from the hook. */
		void*		   m_pIntermediate; /* Custom constructed stub. */

		// Variables related to the number of bytes we saved.
		unsigned char* m_pSavedBytes;   /* Bytes saved from the original function. */
		int			   m_iSavedBytes;	/* Number of bytes we saved. */

		CDetourInfo*      m_pInfo;		/* Information about the detour. */
		CFuncState*	      m_pState;		/* Register save information. */
		CCallBackManager* m_pManager;	/* Stores all language callbacks for this detour. */

		// Variables related to detour status
		bool m_bInitialized;			/* True if everything was successfully setup. */

		// The ASM generator.
		CASMGenerator*	  m_pAsmGenerator; /* Generates ASM for this detour. */

	public:

		/* @brief Constructor.
		 * @param pTarget   - Function to hook.
		 * @param pCallBack - Function to callback to instead of the target.
		 * @param szParams  - String containing argument types of the original function.
		 * @param conv		- Calling convention of the target.
		 */
		CDetour( void* pTarget, void* pCallBack, char* szParams, 
			eCallingConv conv );

		/* @brief Destructor. */
		~CDetour( void );

		/* @brief Saves the original bytes of the hooked function. */
		void Trampoline_Create( void ); 

		/* @brief Writes the saved bytes back to the original function. */
		void Trampoline_Write( void );

		/* @brief Writes a jump from the target function to the callback. */
		void Detour_Create( void );
		
		/* @brief Calls all the callback functions registered to this detour.
		 * @return A HookRes_t structure containing the highest priority action
		 *	after calling all callbacks.
		 */
		HookRes_t* ProcessCallBacks( void );

		/* @brief Accessor for CDetourInfo instance.
		 * @return CDetourInfo pointer containing information about the detour.
		 */
		CDetourInfo* GetInfo( void ) { return m_pInfo; }

		/* @brief  Function state accessor.
		 * @return An instance of CFuncState containing register information
		 *	prior to calling any callback functions.
		 */
		CFuncState* GetState( void ) { return m_pState; }

		/* @brief CallBack Manager accessor.
		 * @return An instance of CCallBackManager containing all the
		 *	ICallBack instances registered to this detour.
		 */
		CCallBackManager* GetCallBackManager( void ) { return m_pManager; }

		/* @brief Returns the memory address of the target function.
		 * @return unsigned char pointer to the target function's address.
		 */
		unsigned char* GetTarget( void ) { return m_pTarget; }

		/* @brief Accessor for the handler function address.
		 * @return The address of the function to redirect the target to. */
		unsigned char* GetCallBack( void ) { return m_pCallBack; }

		/* @brief Accessor for the trampoline. 
		 * @return A pointer to the beginning of the trampoline.
		 */
		unsigned char* GetTrampoline( void ) { return m_pSavedBytes; }
};

#endif // _DD_DETOUR_H