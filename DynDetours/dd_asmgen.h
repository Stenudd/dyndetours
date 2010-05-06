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

#ifndef _DD_ASMGEN_H
#define _DD_ASMGEN_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"
#include "dd_detour.h"
#include "AsmJit/Assembler.h"

//========================================================================
// Forward declarations.
//========================================================================
class CDetour;

//========================================================================
// The ASM Generator class.
//========================================================================
class CASMGenerator
{
	private:
		AsmJit::Label m_pPostCall; // Code to handle the return value of the handler.
		AsmJit::Label m_pOverride; // Code to handle overriding the retval of the hooked
									// function.

		AsmJit::Assembler m_Assembler; // Sets up and generates ASM for labels.

	public:
		/* @brief Constructor.
		 * @param pDetour - The detour we're generating ASM for.
		 */
		CASMGenerator( CDetour* pDetour );

		/* @brief Destructor. */
		~CASMGenerator( void );

		/* @brief Accessor for the address of the generated ASM.
		 * @return A void pointer to the start of the generated ASM code.
		 */
		void* GetCodeAddress( void ) { return m_Assembler.make(); }
		

	private:
		// --------------------------------
		// ASM Generation functions!
		// --------------------------------

		/* @brief Generates Assembly code for a thiscall hook.
		 * @param pDetour - The detour we're generating ASM for.
		 */
		void Generate_This( CDetour* pDetour ); 
		
		/* @brief Generates Assembly code for a cdecl hook.
		 * @param pDetour - The detour we're generating ASM for.
		 */
		void Generate_Cdecl( CDetour* pDetour );
};

#endif // _DD_ASMGEN_H