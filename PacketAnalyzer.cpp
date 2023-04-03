#include "pch.h"
#include "PacketAnalyzer.h"
#include "MainSystem.h"
#include "DBManager.h"
#include "LobbyManager.h"
#include "RoomManager.h"

BLANCNET_BEGIN
PacketAnalyzer::PacketAnalyzer()
{

}

std::string PacketAnalyzer::PacAnalyze(std::shared_ptr<TcpSocket> soc, void* packet, int len)
{
	PACKET_BASE* header = (PACKET_BASE*)packet;

	switch (header->packetId)
	{
	case static_cast<INT16>(COMMON_PACKET_ID::C2S_HEART_BEAT):
	{
		PacHeartBeat(soc->GetNetId());
	}
	break;

	case static_cast<INT16>(TITLE_PACKET_ID::C2S_LOGIN):
	{
		PacLogin(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(TITLE_PACKET_ID::C2S_DUPLICATE_CHECK):
	{
		PacDuplicateCheck(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(TITLE_PACKET_ID::C2S_SIGNUP):
	{
		PacSignUp(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(LOBBY_PACKET_ID::C2S_CREATE_MODEL):
	{
		PacCreateModel(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(LOBBY_PACKET_ID::C2S_CREATE_ROOM):
	{
		PacCreateRoom(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(LOBBY_PACKET_ID::C2S_ENTER_ROOM):
	{
		PacEnterRoom(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(ROOM_PACKET_ID::C2S_CHAT):
	{
		PacChat(soc->GetNetId(), packet, len);
	}
	break;

	case static_cast<INT16>(ROOM_PACKET_ID::C2S_LEAVE_ROOM):
	{
		PacLeaveRoom(soc->GetNetId(), packet, len);
	}
	break;

	}

	std::string str = std::to_string(soc->GetNetId()) + "번 클라이언트에서" + std::to_string(header->packetId) + "번 패킷 수신";
	return str;
}

void PacketAnalyzer::PacHeartBeat(int netId)
{

}

void PacketAnalyzer::PacLogin(int netId, void* packet, int len)
{
	// todo : 우선 로그인부터 처리해보자
	MSG_C2S_LOGIN login;
	std::fill_n(login.userId, 128, 0);
	std::fill_n(login.userPw, 128, 0);
	memcpy(&login, packet, 8);
	memcpy(&login.userId, (char*)(packet)+8, login.idSize);
	memcpy(&login.userPw, (char*)(packet)+136, login.pwSize);

	LoginData loginData;

	bool result = DBManager::GetInstance().LoginCheck(login.userId, login.userPw, loginData);

	if (result)
	{
		LobbyManager::GetInstance().NewPlayer(netId, login.userId, loginData);
	}

	MSG_S2C_LOGIN_RES loginRes;
	loginRes.packetId = static_cast<INT16>(TITLE_PACKET_ID::S2C_LOGIN_RES);
	loginRes.loginResult = result;
	loginRes.modelExist = loginData.modelGender;
	loginRes.modelH = loginData.modelH;
	loginRes.modelU = loginData.modelU;
	loginRes.modelL = loginData.modelL;
	loginRes.packetSize = sizeof(MSG_S2C_LOGIN_RES);

	MainSystem::GetInstance().Send(netId, &loginRes, loginRes.packetSize);

	for(auto player = LobbyManager::GetInstance().m_playerList.begin(); player != LobbyManager::GetInstance().m_playerList.end(); player++)
	{
		MSG_S2C_NEW_LOBBY_USER user;
		user.packetId = (INT16)LOBBY_PACKET_ID::S2C_NEW_LOBBY_USER;
		wcscpy_s(user.userId, player->second->GetNickName().c_str());
		user.idSize = sizeof(user.userId);
		user.packetSize = sizeof(user);

		MainSystem::GetInstance().Send(netId, &user, user.packetSize);
	}

	for (auto room = RoomManager::GetInstance().m_roomList.begin(); room != RoomManager::GetInstance().m_roomList.end(); room++)
	{
		MSG_S2C_NEW_ROOM newRoom;
		newRoom.packetId = (INT16)LOBBY_PACKET_ID::S2C_NEW_LOBBY_USER;
		if (room->second->GetRoomPw() != L"")
		{
			newRoom.isPrivate = true;
		}
		else
		{
			newRoom.isPrivate = false;
		}
		newRoom.roomNum = room->first;
		wcscpy_s(newRoom.roomName, room->second->GetRoomName().c_str());
		newRoom.nameSize = sizeof(newRoom.roomName);
		newRoom.packetSize = sizeof(newRoom);

		MainSystem::GetInstance().Send(netId, &newRoom, newRoom.packetSize);
	}
}

void PacketAnalyzer::PacDuplicateCheck(int netId, void* packet, int len)
{
	MSG_C2S_DUPLICATE_CHECK dupCheck;
	std::fill_n(dupCheck.userId, 128, 0);
	memcpy(&dupCheck, packet, 6);
	memcpy(&dupCheck.userId, (char*)(packet)+6, dupCheck.idSize);

	bool result = DBManager::GetInstance().IdDuplicateCheck(dupCheck.userId);

	MSG_S2C_DUPLICATE_CHECK_RES checkRes;
	checkRes.packetId = static_cast<INT16>(TITLE_PACKET_ID::S2C_DUPLICATE_CHECK_RES);
	checkRes.checkResult = !result;
	checkRes.packetSize = sizeof(MSG_S2C_DUPLICATE_CHECK_RES);

	MainSystem::GetInstance().Send(netId, &checkRes, checkRes.packetSize);
}

void PacketAnalyzer::PacSignUp(int netId, void* packet, int len)
{
	MSG_C2S_SIGNUP signup;
	std::fill_n(signup.userId, 128, 0);
	std::fill_n(signup.userPw, 128, 0);
	memcpy(&signup, packet, 8);
	memcpy(&signup.userId, (char*)(packet)+8, signup.idSize);
	memcpy(&signup.userPw, (char*)(packet)+136, signup.pwSize);

	bool result = DBManager::GetInstance().SignUp(signup.userId, signup.userPw);

	MSG_S2C_SIGNUP_RES signupRes;
	signupRes.packetId = static_cast<INT16>(TITLE_PACKET_ID::S2C_SIGNUP_RES);
	signupRes.signUpResult = result;
	signupRes.packetSize = sizeof(MSG_S2C_SIGNUP_RES);

	MainSystem::GetInstance().Send(netId, &signupRes, signupRes.packetSize);
}

void PacketAnalyzer::PacCreateRoom(int netId, void* packet, int len)
{
	MSG_C2S_CREATE_ROOM createRoom;
	std::fill_n(createRoom.roomName, 128, 0);
	std::fill_n(createRoom.roomPw, 128, 0);
	memcpy(&createRoom, packet, 8);
	memcpy(&createRoom.roomName, (char*)(packet)+8, createRoom.nameSize);
	memcpy(&createRoom.roomPw, (char*)(packet)+136, createRoom.pwSize);
	
	std::shared_ptr<PlayerInfo> player = LobbyManager::GetInstance().PlayerOutLobby(netId);

	int roomNum = RoomManager::GetInstance().CreateRoom(player, createRoom.roomName, createRoom.roomPw);

	MSG_S2C_CREATE_ROOM_RES createRes;
	createRes.packetId = (INT16)LOBBY_PACKET_ID::S2C_CREATE_ROOM_RES;
	createRes.roomNum = roomNum;
	createRes.packetSize = sizeof(createRes);

	MainSystem::GetInstance().Send(netId, &createRes, createRes.packetSize);

	bool isPrivateRoom = false;

	if (sizeof(createRoom.roomPw) != 0)
		isPrivateRoom = true;

	LobbyManager::GetInstance().NewRoomAnnounce(isPrivateRoom, roomNum, createRoom.roomName);
}

void PacketAnalyzer::PacEnterRoom(int netId, void* packet, int len)
{
	MSG_C2S_ENTER_ROOM enterRoom;
	std::fill_n(enterRoom.roomPw, 128, 0);
	memcpy(&enterRoom, packet, 8);
	memcpy(&enterRoom.roomPw, (char*)(packet)+8, enterRoom.pwSize);

	std::shared_ptr<PlayerInfo> player = LobbyManager::GetInstance().PlayerOutLobby(netId);

	bool result = RoomManager::GetInstance().EnterRoom(enterRoom.roomNum, player, enterRoom.roomPw);

	if (!result)
	{
		LobbyManager::GetInstance().PlayerInLobby(netId, player);
	}

	MSG_S2C_ENTER_ROOM_RES enterRes;
	enterRes.packetId = (INT16)LOBBY_PACKET_ID::S2C_ENTER_ROOM_RES;
	enterRes.enterResult = result;
	enterRes.packetSize = sizeof(enterRes);

	MainSystem::GetInstance().Send(netId, &enterRes, enterRes.packetSize);
}

void PacketAnalyzer::PacChat(int netId, void* packet, int len)
{
	MSG_C2S_CHAT chatPac;
	std::fill_n(chatPac.userChat, 128, 0);
	memcpy(&chatPac, packet, 8);
	memcpy(&chatPac.userChat, (char*)(packet)+8, chatPac.chatSize);

	RoomManager::GetInstance().ChatToPlayer(netId, chatPac.roomNum, chatPac.userChat);
}

void PacketAnalyzer::PacLeaveRoom(int netId, void* packet, int len)
{
	MSG_C2S_LEAVE_ROOM leaveRoom;
	memcpy(&leaveRoom, packet, len);

	std::shared_ptr<PlayerInfo> leavePlayer = RoomManager::GetInstance().LeaveRoom(leaveRoom.roomNum, netId);

	if (leavePlayer == nullptr)
	{
		return;
	}

	LobbyManager::GetInstance().PlayerInLobby(netId, leavePlayer);
}

void PacketAnalyzer::PacCreateModel(int netId, void* packet, int len)
{
	MSG_C2S_CREATE_MODEL createModel;
	std::fill_n(createModel.userId, 128, 0);
	memcpy(&createModel, packet, 6);
	memcpy(&createModel.userId, (char*)(packet)+6, createModel.idSize);
	memcpy(&createModel.modelGender, (char*)(packet)+134, 8);


	LoginData loginData;
	loginData.modelGender = createModel.modelGender;
	loginData.modelH = createModel.modelH;
	loginData.modelU = createModel.modelU;
	loginData.modelL = createModel.modelL;

	LobbyManager::GetInstance().PlayerModelSetting(netId, loginData);

	bool result = DBManager::GetInstance().UserModelUpdate(createModel.userId, loginData);
}

BLANCNET_END