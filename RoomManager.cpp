#include "pch.h"
#include "RoomManager.h"

#include "MainSystem.h"

void RoomManager::Init()
{
	m_roomNum = 1;
}

void RoomManager::Update()
{
	for (auto iter = m_roomList.begin(); iter != m_roomList.end(); iter++)
	{
		if(iter->second->RoomPlayers() == 0)
		{
			m_roomList.erase(iter);
			m_emptyRoomNum.push(iter->first);
		}
	}
}

int RoomManager::CreateRoom(std::shared_ptr<PlayerInfo> playerInfo, std::wstring roomName, std::wstring roomPw)
{
	int roomNum = 0;
	if (m_emptyRoomNum.size() == 0)
	{
		roomNum = m_roomNum;
		m_roomNum++;
	}
	else
	{
		roomNum = m_emptyRoomNum.front();
		m_emptyRoomNum.pop();
	}
	std::shared_ptr<RoomInfo> newRoom = std::make_shared<RoomInfo>(roomNum, playerInfo, roomName, roomPw);
	m_roomList.insert({roomNum, newRoom});

	return roomNum;
}

std::shared_ptr<PlayerInfo> RoomManager::LeaveRoom(int roomNum, int netId)
{
	std::shared_ptr<RoomInfo> nowRoom = m_roomList.find(roomNum)->second;

	if (nowRoom->RoomPlayers() == 0)
		return nullptr;

	std::shared_ptr<PlayerInfo> leavePlayer = nowRoom->LeaveRoomPlayer(netId);

	if (nowRoom->RoomPlayers() == 0)
	{
		m_roomList.erase(roomNum);
		m_emptyRoomNum.push(roomNum);
	}
	else
	{
		MSG_S2C_LEAVE_ROOM_RES leaveRes;
		leaveRes.packetId = (INT16)ROOM_PACKET_ID::S2C_LEAVE_ROOM_RES;
		wcscpy_s(leaveRes.leavePlayer, leavePlayer->GetNickName().c_str());
		leaveRes.nameSize = sizeof(leaveRes.leavePlayer);
		leaveRes.packetSize = sizeof(leaveRes);

		for (auto roomPlayer : nowRoom->GetRoomPlayers())
		{
			myNet::MainSystem::GetInstance().Send(roomPlayer->GetNetId(), &leaveRes, leaveRes.packetSize);
		}
	}

	return leavePlayer;
}

bool RoomManager::EnterRoom(int roomNum, std::shared_ptr<PlayerInfo> playerInfo, std::wstring roomPw)
{
	std::shared_ptr<RoomInfo> nowRoom = m_roomList.find(roomNum)->second;

	if (nowRoom->GetRoomPw() != roomPw)
	{
		return false;
	}

	if (nowRoom->RoomPlayers() >= 4)
	{
		return false;
	}

	// 기존의 방 유저에게 전달할 새로 들어온 유저의 정보
	MSG_S2C_ROOM_USER newUser;
	newUser.packetId = (INT16)ROOM_PACKET_ID::S2C_ROOM_USER;
	newUser.modelGender = playerInfo->GetModelData().modelGender;
	newUser.modelH = playerInfo->GetModelData().modelH;
	newUser.modelU = playerInfo->GetModelData().modelU;
	newUser.modelL = playerInfo->GetModelData().modelL;
	wcscpy_s(newUser.userName, playerInfo->GetNickName().c_str());
	newUser.nameSize = sizeof(newUser.userName);
	newUser.packetSize= sizeof(newUser);

	for (auto roomPlayer : nowRoom->GetRoomPlayers())
	{
		// 새로운 유저에게 전달할 기존 방 유저들의 정보
		MSG_S2C_ROOM_USER roomUser;
		roomUser.packetId = (INT16)ROOM_PACKET_ID::S2C_ROOM_USER;
		roomUser.modelGender = roomPlayer->GetModelData().modelGender;
		roomUser.modelH = roomPlayer->GetModelData().modelH;
		roomUser.modelU = roomPlayer->GetModelData().modelU;
		roomUser.modelL = roomPlayer->GetModelData().modelL;
		wcscpy_s(roomUser.userName, roomPlayer->GetNickName().c_str());
		roomUser.nameSize = sizeof(roomUser.userName);
		roomUser.packetSize = sizeof(roomUser);

		myNet::MainSystem::GetInstance().Send(roomPlayer->GetNetId(), &newUser, newUser.packetSize);
		myNet::MainSystem::GetInstance().Send(playerInfo->GetNetId(), &roomUser, roomUser.packetSize);
	}

	nowRoom->SetRoomPlayer(playerInfo);
	return true;
}

void RoomManager::Finalize()
{

}

void RoomManager::ChatToPlayer(int netId, int roomNum, std::wstring chat)
{
	if (m_roomList.find(roomNum) == m_roomList.end())
		return;

	std::shared_ptr<RoomInfo> nowRoom = m_roomList.find(roomNum)->second;

	std::list<std::shared_ptr<PlayerInfo>> nowRoomPlayer = nowRoom->GetRoomPlayers();
	for (auto iter = nowRoomPlayer.begin(); iter != nowRoomPlayer.end(); iter++)
	{
		if (iter->get()->GetNetId() != netId)
		{
			MSG_S2C_CHAT chatPac;
			chatPac.packetId = (INT16)ROOM_PACKET_ID::S2C_CHAT;
			wcscpy_s(chatPac.chatUser, iter->get()->GetNickName().c_str());
			chatPac.idSize = sizeof(chatPac.chatUser);
			wcscpy_s(chatPac.echoChat, chat.c_str());
			chatPac.chatSize = sizeof(chatPac.echoChat);
			chatPac.packetSize = sizeof(chatPac);

			myNet::MainSystem::GetInstance().Send(iter->get()->GetNetId(), &chatPac, chatPac.packetSize);
		}
	}
}
