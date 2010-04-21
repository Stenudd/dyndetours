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

#ifndef _DD_DETOURINFO_H
#define _DD_DETOURINFO_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"

//========================================================================
// The DetourInfo class.
//========================================================================
class CDetourInfo
{
	public:
		char*		 m_szParams;   /* Parameter type string.  */
		int*		 m_iOffsets;   /* Parameter offset array. */
		int			 m_nNumParams; /* Number of parameters. */
		eCallingConv m_eConv;      /* Calling convention of this function. */

	public:
		/* @brief Constructor. */
		CDetourInfo( char* szParams, eCallingConv conv );

		/* @brief Destructor. */
		~CDetourInfo( void );

		/* @brief Getter for param string.
		 * @return Returns a string containing the parameters of
		 *	 the hooked function.
		 */
		char* GetParams( void ) { return m_szParams; }

	   /* @brief Setter for param string.
		* @param szParams - The new parameters of the function.
		*/
		void SetParams( char* szParams );

		/* @brief Returns the number of parameters in this function.
		 * @return Integer containing the number of arguments in the target
		 *	function.
		 */
		int  GetNumParams( void ) { return m_nNumParams; }

		/* @brief Accessor for the parameter offsets.
		 * @return Pointer to an array containing integer values denoting
		 *	the offset of each parameter from ESP.
		 */
		int* GetOffsets( void ) { return m_iOffsets; }

		eCallingConv GetConv( void ) { return m_eConv; }

	private:
		void ComputeStackOffsets( void );

};


#endif // _DD_DETOURINFO_H
