#pragma once

//BLANCNET_BEGIN

namespace myNet{

class WinSocket
{
public:
	WinSocket() {}
	~WinSocket()
	{
		if (m_soc != INVALID_SOCKET) closesocket(m_soc);
		if (m_hEvent != WSA_INVALID_EVENT) WSACloseEvent(m_hEvent);
	}

	SOCKET GetSocHandle() { return m_soc; }
	void SetSocHandle(SOCKET soc) { m_soc = soc; }
	bool IsValid() { return ((INVALID_SOCKET == m_soc) ? false : true); }

	HANDLE GetEventHandle() { return m_hEvent; }

	bool Open(int iProtocol);
	bool Bind(SOCKADDR_IN& localAddr);

	bool EventSelect(long lNetWorkEvents);

	void Close();
	void Shutdown(int how = SD_BOTH);

protected:
	SOCKET m_soc = INVALID_SOCKET;
	HANDLE m_hEvent = WSA_INVALID_EVENT;
	SOCKADDR_IN m_localAddr = { 0, };

private:
	WinSocket(const WinSocket&) = delete;
	WinSocket(WinSocket&&) = delete;
	WinSocket& operator=(const WinSocket&) {}
};
}
//BLANCNET_END