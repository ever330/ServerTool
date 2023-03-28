#pragma once
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
	void Update();
	void PlayerLogIn(int netId, std::string id);
	void PlayerLogOut(int netId);
	//std::vector<std::shared_ptr<PlayerInfo>> m_playerList;
	std::map<int, std::shared_ptr<PlayerInfo>> m_playerList;
};

