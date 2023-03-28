#include "pch.h"
#include "PacketAnalyzer.h"
#include "MainSystem.h"
#include "DBManager.h"
#include "LobbyManager.h"

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
		PacLeaveRoom(soc->GetNetId());
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
	memcpy(&login, packet, len);

	bool result = DBManager::GetInstance().LoginCheck(login.userId, login.userPw);

	if (result)
	{
		LobbyManager::GetInstance().PlayerLogIn(netId, login.userId);
	}

	MSG_S2C_LOGIN_RES loginRes;
	loginRes.packetId = static_cast<INT16>(TITLE_PACKET_ID::S2C_LOGIN_RES);
	loginRes.loginResult = result;
	loginRes.packetSize = sizeof(MSG_S2C_LOGIN_RES);

	MainSystem::GetInstance().Send(netId, &loginRes, loginRes.packetSize);
}

void PacketAnalyzer::PacDuplicateCheck(int netId, void* packet, int len)
{
	MSG_C2S_DUPLICATE_CHECK dupCheck;
	memcpy(&dupCheck, packet, len);

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
	memcpy(&signup, packet, len);

	bool result = DBManager::GetInstance().SignUp(signup.userId, signup.userPw);

	MSG_S2C_SIGNUP_RES signupRes;
	signupRes.packetId = static_cast<INT16>(TITLE_PACKET_ID::S2C_SIGNUP_RES);
	signupRes.signUpResult = result;
	signupRes.packetSize = sizeof(MSG_S2C_SIGNUP_RES);

	MainSystem::GetInstance().Send(netId, &signupRes, signupRes.packetSize);
}

void PacketAnalyzer::PacCreateRoom(int netId, void* packet, int len)
{

}

void PacketAnalyzer::PacEnterRoom(int netId, void* packet, int len)
{

}

void PacketAnalyzer::PacChat(int netId, void* packet, int len)
{

}

void PacketAnalyzer::PacLeaveRoom(int netId)
{

}

BLANCNET_END