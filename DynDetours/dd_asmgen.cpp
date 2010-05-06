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
#include "dd_asmgen.h"

//========================================================================
// Namespaces we're going to use.
//========================================================================
using namespace AsmJit;

//========================================================================
// Constructor.
//========================================================================
CASMGenerator::CASMGenerator( CDetour* pDetour )
{
	// Make sure the detour is valid
	if( !pDetour )
	{
		printf("Detour instance wasn't valid!\n");
		return;
	}

	// Get the calling convention.
	eCallingConv conv = pDetour->GetInfo()->GetConv();

	// Figure out what to generate.
	switch( conv )
	{
		case Convention_CDECL:
			Generate_Cdecl(pDetour);
			break;
		
		case Convention_THIS:
			Generate_This(pDetour);
			break;
	}

}

//========================================================================
// Destructor
//========================================================================
CASMGenerator::~CASMGenerator()
{
	m_Assembler.free();
}

//========================================================================
// Generates code for a CDECL function.
//========================================================================
void CASMGenerator::Generate_Cdecl( CDetour* pDetour )
{
	// Get the handler function.
	void* pCallBack = pDetour->GetCallBack();

	// Make sure this is valid.
	if( !pCallBack )
	{
		printf("Couldn't generate CDECL asm! Callback was null!\n");
		return;
	}

	// Get the state variable instance.
	CFuncState* pState = pDetour->GetState();
	
	// Make sure this is valid.
	if( !pState )
	{
		printf("Generate_Cdecl: pState was NULL!\n");
		return;
	}

	// ------------------------------
	// Setup the precall.
	// ------------------------------
	// 1) Save ESP.
	// 2) Push the detour instance.
	// 3) Call it's callback.
	// 4) Restore the stack.
	// 5) Compare the return value.
	// 6) Jump to the appropriate label.
	// ------------------------------

	// mov m_ulOrigESP, esp
	m_Assembler.mov( dword_ptr_abs(&pState->m_ulOrigESP), esp );

	// push detour
	m_Assembler.push( imm((SysInt)pDetour) );

	// call callback
	m_Assembler.call( pCallBack );

	// add esp, 0x04
	m_Assembler.add( esp, imm(4) );

	// cmp eax, HOOK_ACTION_OVERRIDE
	m_Assembler.cmp( eax, imm(HOOK_ACTION_OVERRIDE) );

	// je m_Override.
	m_Assembler.je( &m_pOverride );

	// jmp m_PostCall
	m_Assembler.jmp( &m_pPostCall );

	// ------------------------------
	// Setup the Override.
	// ------------------------------
	// 1) Save the return value into
	//	our state variable.
	// 2) Return.
	// ------------------------------
	m_Assembler.bind( &m_pOverride );

	// mov m_pRetVal, eax
	m_Assembler.mov( dword_ptr_abs(&pState->m_pRetVAL), eax );

	// ret
	m_Assembler.ret();

	// ------------------------------
	// Setup the postcall.
	// ------------------------------
	// 1) Jump to the trampoline.
	// ------------------------------
	m_Assembler.bind( &m_pPostCall );
	
	// jmp m_pTrampoline.
	m_Assembler.jmp( pDetour->GetTrampoline() );
}

//========================================================================
// Generates code for a thiscall.
//========================================================================
void CASMGenerator::Generate_This( CDetour* pDetour )
{
	// Get the handler function.
	void* pCallBack = pDetour->GetCallBack();

	printf("Callback: %d\n", pCallBack);

	// Make sure this is valid.
	if( !pCallBack )
	{
		printf("Generate_This: pCallBack was null!\n");
		return;
	}

	// Get the state variable instance.
	CFuncState* pState = pDetour->GetState();

	// Make sure this is valid.
	if( !pState )
	{
		printf("Generate_This: pState was NULL!\n");
		return;
	}

	// ------------------------------
	// Setup the precall.
	// ------------------------------
	// 1) Save ESP.
	// 2) Save ECX.
	// 3) Push the detour instance.
	// 4) Call it's callback.
	// 5) Restore the stack.
	// 6) Restore ECX.
	// 7) Compare the return value.
	// 8) Jump to the appropriate label.
	// ------------------------------

	// mov m_ulOrigESP, esp
	m_Assembler.mov( dword_ptr_abs(&pState->m_ulOrigESP), esp );

	// mov m_ulOrigECX, ecx
	m_Assembler.mov( dword_ptr_abs(&pState->m_ulOrigECX), ecx );

	// push detour
	m_Assembler.push( imm((SysInt)pDetour) );

	// call callback
	m_Assembler.call( pCallBack );

	// add esp, 0x04
	m_Assembler.add( esp, imm(4) );

	// mov ecx, m_ulOrigECX
	m_Assembler.mov( ecx, dword_ptr_abs(&pState->m_ulOrigECX));

	// cmp eax, HOOK_ACTION_OVERRIDE
	m_Assembler.cmp( eax, imm(HOOK_ACTION_OVERRIDE) );

	// je m_Override.
	m_Assembler.je( &m_pOverride );

	// jmp m_PostCall
	m_Assembler.jmp( &m_pPostCall );

	// ------------------------------
	// Setup the Override.
	// ------------------------------
	// 1) Save the return value into
	//	our state variable.
	// 2) Return.
	// ------------------------------
	m_Assembler.bind( &m_pOverride );

	// mov m_pRetVal, eax
	m_Assembler.mov( dword_ptr_abs(&pState->m_pRetVAL), eax );

	// ret # of bytes on stack
	m_Assembler.ret();

	// ------------------------------
	// Setup the postcall.
	// ------------------------------
	// 1) Jump to the trampoline.
	// ------------------------------
	m_Assembler.bind( &m_pPostCall );

	// jmp m_pTrampoline.
	m_Assembler.jmp( pDetour->GetTrampoline() );

}