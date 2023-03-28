#pragma once

BLANCNET_BEGIN

class WinSocManager
{
public:
	WinSocManager(const WinSocManager&) = delete;
	WinSocManager(WinSocManager&&) = delete;
	WinSocManager& operator=(const WinSocManager&) = delete;

	WinSocManager() {}
	~WinSocManager() {}

	bool Init();
	void CleanUp();

private:
	WSAData m_wsd = { 0, };
};

BLANCNET_END