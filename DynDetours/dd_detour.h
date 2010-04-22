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
#include "dd_detourinfo.h"  /* Need CDetourInfo class. */
#include "dd_callback.h"    /* Need ICallBack interface. */
#include "dd_definitions.h" /* Need detour definitions. */
#include "dd_funcstate.h"	/* Need CFuncState class. */
#include "ASMJit/AsmJit.h"  /* Need Assembler and Label classes. */
#include <vector>			/* Need vector class. */

//========================================================================
// Namespaces we need
//========================================================================
using namespace std;

//========================================================================
// This is the detour class.
//========================================================================
class CDetour
{
	private:
		// List of callbacks
		vector<ICallBack*> m_vecCallBacks;

		// Variables related to the hooked functions.
		unsigned char* m_pTarget;		/* Target function to hook. */
		unsigned char* m_pCallBack;		/* Function to callback from the hook. */
		void*		   m_pIntermediate; /* Custom constructed stub. */

		// Variables related to the number of bytes we saved.
		unsigned char* m_pSavedBytes;   /* Bytes saved from the original function. */
		int			   m_iSavedBytes;	/* Number of bytes we saved. */

		CDetourInfo*   m_pInfo;			/* Information about the detour. */
		CFuncState*	   m_pState;		/* Register save information. */

#if 0
		// Variables related to registers
		unsigned long  m_ulOrigESP;		/* ESP before calling the handler. */
		unsigned long  m_ulOrigRet;		/* Return address of target function. */
		unsigned long  m_ulOrigECX;		/* ECX before calling the handler. */
		void*		   m_ulRetBuffer;	/* Stores the return value if we're blocking a call. */
#endif

		// Variables related to detour status
		bool		   m_bInitialized;	/* True if everything was successfully setup. */

		// The assembler.
		AsmJit::Assembler m_Assembler;	/* Emits x86 ASM opcodes for our intermediate. */

		// Labels
		AsmJit::Label  m_PostCall;		/* Contains instructions for calling the trampoline. */
		AsmJit::Label  m_OverCall;		/* Contains instructions for overriding return type. */

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

		/* @brief Sets up the codestub the target will jmp to. */
		void Setup_PreCall( void );

		/* @brief Sets up code to handle calling the trampoline. */
		void Setup_PostCall( void );

		/* @brief Sets up code to handle overriding the return value. */
		void Setup_Override( void );

		/* @brief Saves the original bytes of the hooked function. */
		void Trampoline_Create( void ); 

		/* @brief Writes the saved bytes back to the original function. */
		void Trampoline_Write( void );

		/* @brief Writes a jump from the target function to the callback. */
		void Detour_Create( void );

		/* @brief Adds a callback to this detour.
		 * @param pCallBack - Language specific callback instance to add to our
		 *	internal list.
		 * @return True if the callback was successfully added.
		 */
		bool CallBack_Add( ICallBack* pCallBack );

		/* @brief Finds a callback by language name.
		 * @param szLanguage - Language the callback handles.
		 * @return ICallBack pointer which handles callbacks for the given
		 *	language.
		 */
		ICallBack* CallBack_Find( const char* szLanguage );

		/* @brief Processes all the callbacks stored in this detour.
		 * @return The highest priority hook structure.
		 */
		HookRes_t* Process_CallBacks( void );

		/* @brief Accessor for CDetourInfo instance.
		 * @return CDetourInfo pointer containing information about the detour.
		 */
		CDetourInfo* GetInfo( void ) { return m_pInfo; }

		/* @brief Returns the memory address of the target function.
		 * @return unsigned char pointer to the target function's address.
		 */
		unsigned char* GetTarget( void ) { return m_pTarget; }

		CFuncState* GetState( void ) { return m_pState; }
};

#endif // _DD_DETOUR_H