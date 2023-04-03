#include "pch.h"
#include "LobbyManager.h"
#include "PacketDefine.h"
#include "MainSystem.h"
#include "RoomManager.h"

#pragma warning(disable : 4996)


void LobbyManager::Init()
{
	//m_delay = 5000.f;
	//m_timer = clock();
	RoomManager::GetInstance().Init();
}

void LobbyManager::Finalize()
{

}

void LobbyManager::Update()
{
	//if (clock() - m_timer >= m_delay)
	//{
	//	m_timer = clock();
	//	//LobbyUserListSend();
	//}
}

void LobbyManager::NewPlayer(int netId, std::wstring id, myNet::LoginData& modelData)
{
	MSG_S2C_NEW_LOBBY_USER newUser;
	newUser.packetId = (INT16)LOBBY_PACKET_ID::S2C_NEW_LOBBY_USER;
	wcscpy_s(newUser.userId, id.c_str());
	newUser.idSize = sizeof(newUser.userId);
	newUser.packetSize = sizeof(newUser);

	for (auto player : m_playerList)
	{
		myNet::MainSystem::GetInstance().Send(player.second->GetNetId(), &newUser, newUser.packetSize);
	}

	std::shared_ptr<PlayerInfo> newPlayer = std::make_shared<PlayerInfo>(netId, id, modelData);
	m_playerList.insert({ netId, newPlayer });
	m_logoutId.remove(netId);
}

void LobbyManager::PlayerInLobby(int netId, std::shared_ptr<PlayerInfo> player)
{
	m_playerList.insert({ netId, player });
}

std::shared_ptr<PlayerInfo> LobbyManager::PlayerOutLobby(int netId)
{
	if (m_playerList.size() == 0)
		return nullptr;

	std::shared_ptr<PlayerInfo> nowPlayer;

	if (m_playerList.find(netId) != m_playerList.end())
	{
		nowPlayer = m_playerList.find(netId)->second;
	}
	else
	{
		nowPlayer = nullptr;
	}

	return nowPlayer;
}

void LobbyManager::PlayerModelSetting(int netId, myNet::LoginData& modelData)
{
	for (auto player : m_playerList)
	{
		if (player.second->GetNetId() == netId)
		{
			player.second->SetModelData(modelData);
		}
	}
}

void LobbyManager::NewRoomAnnounce(bool isPrivate, int roomNum, std::wstring roomName)
{
	MSG_S2C_NEW_ROOM newRoom;
	newRoom.packetId = (INT16)LOBBY_PACKET_ID::S2C_NEW_ROOM;
	newRoom.isPrivate = isPrivate;
	newRoom.roomNum = roomNum;
	wcscpy_s(newRoom.roomName, roomName.c_str());
	newRoom.nameSize = sizeof(newRoom.roomName);
	newRoom.packetSize = sizeof(newRoom);

	for (auto player : m_playerList)
	{
		myNet::MainSystem::GetInstance().Send(player.second->GetNetId(), &newRoom, newRoom.packetSize);
	}
}

void LobbyManager::PlayerLogout(int netId)
{
	bool idCheck = false;
	for (auto iter = m_logoutId.begin(); iter != m_logoutId.end(); iter++)
	{
		if (*iter == netId)
		{
			idCheck = true;
		}
	}
	if (idCheck)
		return;

	std::shared_ptr<PlayerInfo> logoutPlayer = m_playerList.find(netId)->second;

	MSG_S2C_DELETE_LOBBY_USER deleteUser;
	deleteUser.packetId = (INT16)LOBBY_PACKET_ID::S2C_DELETE_LOBBY_USER;
	if (m_playerList.find(netId) == m_playerList.end())
	{
		std::map<int, std::shared_ptr<RoomInfo>> roomInfo = RoomManager::GetInstance().m_roomList;

		if (roomInfo.size() == 0)
			return;

		for (auto iter = roomInfo.begin(); iter != roomInfo.end(); iter++)
		{
			for (auto player = iter->second->GetRoomPlayers().begin(); player != iter->second->GetRoomPlayers().end(); player++)
			{
				std::shared_ptr<PlayerInfo> nowPlayer = *player;
				if (nowPlayer->GetNetId() == netId)
				{
					wcscpy_s(deleteUser.userId, player->get()->GetNickName().c_str());
					deleteUser.idSize = sizeof(deleteUser.userId);
					deleteUser.packetSize = sizeof(deleteUser);
					iter->second->GetRoomPlayers().erase(player);
				}
			}
		}
	}
	else
	{
		wcscpy_s(deleteUser.userId, logoutPlayer->GetNickName().c_str());
		deleteUser.idSize = sizeof(deleteUser.userId);
		deleteUser.packetSize = sizeof(deleteUser);
		m_playerList.erase(netId);
	}
	myNet::MainSystem::GetInstance().Broadcast(&deleteUser, deleteUser.packetSize);
	m_logoutId.push_back(netId);
}

//void LobbyManager::LobbyUserListSend()
//{
//	if (m_playerList.size() != 0)
//	{
//		MSG_S2C_LOBBY_USER lobbyUser;
//		lobbyUser.packetId = static_cast<INT16>(LOBBY_PACKET_ID::S2C_LOBBY_USER);
//		lobbyUser.packetSize = sizeof(MSG_S2C_LOBBY_USER);
//		lobbyUser.lobbyUser[0] = { 0, };
//		lobbyUser.userCount = static_cast<INT16>(m_playerList.size());
//
//		for (auto iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
//		{
//			StrCatW(lobbyUser.lobbyUser, iter->second->GetNickName().c_str());
//		}
//
//		myNet::MainSystem::GetInstance().Broadcast(&lobbyUser, lobbyUser.packetSize);
//	}
//}
