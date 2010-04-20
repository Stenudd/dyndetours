//========================================================================
// Includes
//========================================================================
#include "dd_detourman.h"

//========================================================================
// Global variables
//========================================================================
CDetourManager* g_pDetourManager = NULL;

//========================================================================
// Destructor
//========================================================================
CDetourManager::~CDetourManager()
{
	// Loop through each element in the vector and get rid of it.
	for( unsigned int i = 0; i < m_DetourList.size(); i++ )
	{
		// Get the detour instance.
		CDetour* pCurDetour = m_DetourList[i];

		// If it's valid, erase it
		if( pCurDetour )
			delete pCurDetour;
	}
}

//========================================================================
// Adds a detour.
//========================================================================
CDetour* CDetourManager::Add_Detour( void* pTarget, char* szParams, 
									eCallingConv conv )
{
	// We need both a target and a parameter string
	// to continue.
	if( !pTarget || !szParams )
		return NULL;

	// Do we already have a function with this address?
	CDetour* pDetour = Find_Detour(pTarget);
	
	// Return it if there already is one.
	if( pDetour )
	{
		return pDetour;
	}

	// Otherwise, we've got to make one
	pDetour = new CDetour( pTarget, NULL, szParams, conv );

	// Add them to the list
	m_DetourList.push_back(pDetour);

	// Return the newly created detour
	return pDetour;
}

//========================================================================
// Removes a detour.
//========================================================================
bool CDetourManager::Remove_Detour( void* pTarget )
{
	// Need at least a target address to find the detour instance.
	if( !pTarget )
	{
		return false;
	}

	// Loop through the list
	LIST_ITER i;
	for( i = m_DetourList.begin(); i != m_DetourList.end(); i++ )
	{
		// Did we find it?
		if( (*i)->GetTarget() == pTarget )
		{
			// Remove it from the vector
			m_DetourList.erase(i);

			// Done
			return true;
		}
	}

	// If we get here, we didn't find it.
	return false;
}

//========================================================================
// Finds a detour in the list.
//========================================================================
CDetour* CDetourManager::Find_Detour( void *pTarget )
{
	if( !pTarget )
	{
		return NULL;
	}

	// Loop through the list, see if we can find it
	for( unsigned int i = 0; i < m_DetourList.size(); i++ )
	{
		// Get the detour
		CDetour* pDetour = m_DetourList[i];

		// Is it valid?
		if( !pDetour )
			continue;

		// Is it the right one?
		if( pDetour->GetTarget() == pTarget )
			return pDetour;
	}

	// If we make it here, we can't find it.
	return NULL;
}