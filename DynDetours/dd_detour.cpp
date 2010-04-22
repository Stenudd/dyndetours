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
#include "dd_detour.h"
#include "dd_handler.h"
#include "dd_asm.h"
#include "dd_memutils.h"
#include "ASMJit/MemoryManager.h"
#include "ASMJit/Compiler.h"

//========================================================================
// Namespaces we'll need.
//========================================================================
using namespace AsmJit;

//========================================================================
// Constructor
//========================================================================
CDetour::CDetour( void* pTarget, void* pCallBack, char* szParams, 
				 eCallingConv conv )
{
	// Sanity checking.
	if( !pTarget || !szParams )
	{
		// Set default values.
		pTarget = pCallBack = szParams = NULL;
		m_bInitialized = false;
		return;
	}

	// Store the target
	m_pTarget = (unsigned char *)pTarget;

	// If we're given a callback, use it!
	if( pCallBack )
		m_pCallBack = (unsigned char *)pCallBack;
	
	// Otherwise, use the default.
	else
		m_pCallBack = (unsigned char *)(&DynHandler);

	// Instantiate the detour information
	m_pInfo = new CDetourInfo( szParams, conv );

	// Allocate space for the function state
	m_pState = new CFuncState();

	// ----------------------
	// Detour initialization!
	// ----------------------

	// Create the trampoline first
	Trampoline_Create();

	// Now apply the detour
	Detour_Create();

	// We should be initialized at this point
	m_bInitialized = true;
}

//========================================================================
// Destructor
//========================================================================
CDetour::~CDetour()
{
	if( m_bInitialized )
	{
		// Restore the original function
		Trampoline_Write();

		// Free up memory taken by the detour info
		if( m_pInfo )
		{
			delete m_pInfo;
		}

		// Free up memory taken up by the state.
		if( m_pState )
		{
			delete m_pState;
		}

		// Free memory taken up by the trampoline
		if( m_pSavedBytes )
		{
			delete m_pSavedBytes;
		}

		// Free up assembler memory
		m_Assembler.free();
	}
}

//========================================================================
// Creates the trampoline
//========================================================================
void CDetour::Trampoline_Create( void )
{
	// Figure out how many bytes we need to save
	// Skip past the prologue.
	m_iSavedBytes = copy_bytes(m_pTarget, NULL, 6);

	// Create the space for it
	// Need 5 bytes for a jump back to the target + 5.
	m_pSavedBytes = new unsigned char[m_iSavedBytes + 5];

	// Now copy the bytes
	copy_bytes(m_pTarget, m_pSavedBytes, m_iSavedBytes);

	// Inject a jump to the target
	WriteJMP(m_pSavedBytes+m_iSavedBytes, m_pTarget + m_iSavedBytes);
}

//========================================================================
// Writes the trampoline back to the original function
//========================================================================
void CDetour::Trampoline_Write( void )
{
	// Make sure we can write to the memory
	SetMemPatchable(m_pTarget, m_iSavedBytes);

	// Copy the bytes back
	copy_bytes(m_pSavedBytes, m_pTarget, m_iSavedBytes);
}

//========================================================================
// Creates and writes the detour to the target function.
//========================================================================
void CDetour::Detour_Create( void )
{
	if( !m_pTarget )
		return;

	Setup_PreCall();
	Setup_Override();
	Setup_PostCall();

	// Store the above buffer's address in our
	// intermediate pointer.
	m_pIntermediate = m_Assembler.make();

	// Inject a jump to the intermediate
	WriteJMP(m_pTarget, m_pIntermediate);
}

//========================================================================
// Sets up the precall assembler code.
//========================================================================
void CDetour::Setup_PreCall()
{
	//---------------------------------------
	// The Process:
	//	1) Save ESP
	//  1b) Save ECX if we're a thiscall.
	//	2) Save original return address.
	//	3) Push this pointer.
	//  4) Call the handler.
	//	5) Remove the thispointer.
	//  6) Cmp result with override.
	//  7) Jump if equal, to override.
	//  8) Otherwise, jump to trampoline.
	//---------------------------------------
	m_Assembler.mov(dword_ptr_abs(&m_pState->m_ulOrigESP), esp); // mov m_ulOrigESP, esp

#ifdef _WIN32
	//---------------------------------------
	// On Microsoft Windows, a thiscall puts
	// the this pointer into ECX.
	//---------------------------------------
	if( m_pInfo->GetConv() == Convention_THIS )
		m_Assembler.mov(dword_ptr_abs(&m_pState->m_ulOrigECX), ecx);
#endif

	//---------------------------------------
	// CallBack handling.
	//---------------------------------------
	m_Assembler.push(imm((SysInt)this));			       // push this
	m_Assembler.call(m_pCallBack);					       // call m_pCallBack

	//---------------------------------------
	// Stack restoration
	// This gets rid of the "this" pointer for
	// this class.
	//---------------------------------------
	m_Assembler.add( esp, imm(4) );					       // add esp, 4

	//---------------------------------------
	// Restore ECX if we're a thiscall.
	//---------------------------------------
#ifdef _WIN32
	if( m_pInfo->GetConv() == Convention_THIS )
		m_Assembler.mov(ecx, dword_ptr_abs(
						&m_pState->m_ulOrigECX)); // mov ecx, m_ulOrigECX
#endif

	//---------------------------------------
	// Return value handling.
	//---------------------------------------
	m_Assembler.cmp( eax, imm(HOOK_ACTION_OVERRIDE) );     // cmp eax, 1
	m_Assembler.je(&m_OverCall);					       // je m_OverrideLabel
	m_Assembler.jmp(&m_PostCall);					       // jmp m_Trampoline.
}

//========================================================================
// Sets up the postcall assembler code.
//========================================================================
void CDetour::Setup_PostCall()
{
	//---------------------------------------
	// The Process:
	//	1) Jump to trampoline.
	//---------------------------------------
	m_Assembler.bind(&m_PostCall);
	m_Assembler.jmp(m_pSavedBytes);
}

//========================================================================
// Sets up the override assembler code.
//========================================================================
void CDetour::Setup_Override()
{
	//---------------------------------------
	// The Process:
	//	1) Take result in retbuf, store in eax.
	//  2) Remove stack variables.
	//	3) Return.
	//---------------------------------------
	m_Assembler.bind(&m_OverCall);
	m_Assembler.mov( eax, dword_ptr_abs(&m_pState->m_pRetVAL));
	m_Assembler.ret();
}


//========================================================================
// Processes all the callbacks for this detour.
//========================================================================
HookRes_t* CDetour::Process_CallBacks()
{
	// Are we initialized?
	if( !m_bInitialized )
		return NULL;

	// We will store the highest priority hook in this
	// variable.
	HookRes_t* pHighest = NULL;
	HookRes_t* pCurRes = NULL;

	// Loop through all the callbacks in the vector.
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// Get the ICallBack instance.
		ICallBack* pCallBack = m_vecCallBacks[i];

		// Sanity check
		if( !pCallBack )
			continue;

		// Tell it to process its callbacks.
		pCurRes = pCallBack->ProcessCallBack( this );

		// Something went wrong
		if( !pCurRes )
		{
			// Move on to the next callback.
			continue;
		}
		
		// Is pHighest initialized?
		if( !pHighest )
		{
			// If not, set the current result to it.
			pHighest = pCurRes;

			// And iterate to the next callback.
			continue;
		}

		// Check priority
		if( pCurRes->action >= pHighest->action )
		{
			// Get rid of the memory from the old result.
			delete pHighest;

			// Set it to the new result.
			pHighest = pCurRes;
		}

		else
		{
			delete pCurRes;
		}
	}

	// Return our highest priority result.
	return pHighest;
}

//========================================================================
// Finds a callback by language.
//========================================================================
ICallBack* CDetour::CallBack_Find( const char* szLanguage )
{
	if( !szLanguage )
		return NULL;

	// Loop through each callback
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// Grab ICallBack instance
		ICallBack* pCallBack = m_vecCallBacks[i];

		// Is it valid?
		if( !pCallBack )
			continue;

		// If it is, check the language
		if( strcmp(szLanguage, pCallBack->GetLanguageName()) == 0 )
			return pCallBack;
	}

	// If we're here, we couldn't find it
	return NULL;
}

//========================================================================
// Adds a callback to this Detour's internal list.
//========================================================================
bool CDetour::CallBack_Add( ICallBack* pCallBack )
{
	if( !pCallBack )
		return false;

	// Does a callback for this language already
	// exist?
	if( CallBack_Find(pCallBack->GetLanguageName()) )
		return true;

	// Add it if it's not
	m_vecCallBacks.push_back(pCallBack);

	// We're done!
	return true;
}