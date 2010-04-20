//////////////////////////////////////////////////////////////////////////
// File: dd_detourinfo.h
// Author: Deniz "your-name-here" Sezen
// Created: 7:45 PM, February 25, 2010
// Contributors: -
// Purpose: This file defines the detour info class which stores
//	attributes for a particular detour.
//////////////////////////////////////////////////////////////////////////
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