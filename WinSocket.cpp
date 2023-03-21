#include "pch.h"
#include "WinSocket.h"

BLANCNET_BEGIN

bool WinSocket::Open(int iProtocol)
{
	if (iProtocol == IPPROTO_TCP)
	{
		m_soc = WSASocket(AF_INET, SOCK_STREAM, iProtocol, NULL, NULL, WSA_FLAG_OVERLAPPED);
	}
	else if (iProtocol == IPPROTO_UDP)
	{
		m_soc = WSASocket(AF_INET, SOCK_DGRAM, iProtocol, NULL, NULL, WSA_FLAG_OVERLAPPED);
	}

	if (!IsValid()) return false;

	return true;
}

bool WinSocket::Bind(SOCKADDR_IN& localAddr)
{
	if (IsValid())
	{
		if (SOCKET_ERROR == bind(m_soc, (SOCKADDR*)(&localAddr), sizeof(SOCKADDR_IN)))
			return false;

		m_localAddr = localAddr;
	}

	return true;
}

bool WinSocket::EventSelect(long lNetWorkEvents)
{
	if (m_hEvent == WSA_INVALID_EVENT)
	{
		if ((m_hEvent = WSACreateEvent()) == WSA_INVALID_EVENT) return false;
	}

	if (WSAEventSelect(m_soc, m_hEvent, lNetWorkEvents) == SOCKET_ERROR) return false;

	return true;
}

void WinSocket::Close()
{
	closesocket(m_soc);
	m_soc = INVALID_SOCKET;
}

void WinSocket::Shutdown(int how /*= SD_BOTH*/)
{
	shutdown(m_soc, how);
}

BLANCNET_END