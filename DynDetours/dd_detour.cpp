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

//========================================================================
// Constructor
//========================================================================
CDetour::CDetour( void* pTarget, void* pCallBack, char* szParams, 
				 eCallingConv conv )
{
	// Sanity checking.
	if( !pTarget || !szParams )
	{
		m_bInitialized = false;
		return;
	}

	// Store the target
	m_pTarget = (unsigned char *)pTarget;

	// If we're given a callback, use it!
	if( pCallBack )
	{
		m_pCallBack = (unsigned char *)pCallBack;
	}
	
	// Otherwise, use the default.
	else
	{
		m_pCallBack = (unsigned char *)(&DynHandler);
	}

	// Instantiate the detour information
	m_pInfo = new CDetourInfo( szParams, conv );

	// Instantiate storage for the function state
	// information class.
	m_pState = new CFuncState();

	// Instantiate the callback manager.
	m_pManager = new CCallBackManager();

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

		// Free up memory taken up by the callback
		// manager.
		if( m_pManager )
		{
			delete m_pManager;
		}

		// Free memory taken up by the trampoline
		if( m_pSavedBytes )
		{
			delete m_pSavedBytes;
		}

		// Free up memory taken up by the ASM generator.
		if( m_pAsmGenerator )
		{
			delete m_pAsmGenerator;
		}
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

	// Create a new ASMGenerator
	m_pAsmGenerator = new CASMGenerator( this );

	// Setup the intermediate
	m_pIntermediate = m_pAsmGenerator->GetCodeAddress();

	// Inject a jump to the intermediate
	WriteJMP(m_pTarget, m_pIntermediate);
}

//========================================================================
// Processes callbacks for this detour.
//========================================================================
HookRes_t* CDetour::ProcessCallBacks( void )
{
	// Make sure we're initialized.
	if( !m_bInitialized || !m_pManager )
	{
		return NULL;
	}

	// Have the CallBack manager do the horsework.
	return m_pManager->ProcessCallBacks( this );
}