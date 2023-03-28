#include "pch.h"
#include "LobbyManager.h"

void LobbyManager::Update()
{

}

void LobbyManager::PlayerLogIn(int netId, std::string id)
{
	std::shared_ptr<PlayerInfo> newPlayer = std::make_shared<PlayerInfo>(netId, id);
	m_playerList.insert({ netId, newPlayer });
}

void LobbyManager::PlayerLogOut(int netId)
{
	if (m_playerList.find(netId) != m_playerList.end())
	{
		m_playerList.erase(netId);
	}
}
