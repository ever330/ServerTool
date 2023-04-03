#pragma once

#include "PacketAnalyzer.h"
#include "PlayerInfo.h"

class LobbyManager
{
private:
	LobbyManager() {}
	LobbyManager(const LobbyManager&) {}
	LobbyManager& operator=(const LobbyManager&) {}
	~LobbyManager() {}

public:
	static LobbyManager& GetInstance()
	{
		static LobbyManager* instance = new LobbyManager();
		return *instance;
	}

public:
	void Init();
	void Finalize();
	void Update();
	void NewPlayer(int netId, std::wstring id, myNet::LoginData& modelData);
	void PlayerInLobby(int netId, std::shared_ptr<PlayerInfo> player);
	std::shared_ptr<PlayerInfo> PlayerOutLobby(int netId);
	std::map<int, std::shared_ptr<PlayerInfo>> m_playerList;

	void PlayerModelSetting(int netId, myNet::LoginData& modelData);

	void NewRoomAnnounce(bool isPrivate, int roomNum, std::wstring roomName);

	void PlayerLogout(int netId);

private:
	std::list<int> m_logoutId;
	//void LobbyUserListSend();

private:
	//std::thread m_lobbyThread;
	//float m_delay;
	//clock_t m_timer;
};

