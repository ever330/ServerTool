#include "pch.h"
#include "WinSocManager.h"

bool WinSocManager::Init()
{
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_wsd))
	{
		return false;
	}

	return true;
}

void WinSocManager::CleanUp()
{
	WSACleanup();
}